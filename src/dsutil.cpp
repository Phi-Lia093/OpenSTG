//-----------------------------------------------------------------------------
// File: dsutil.cpp
//
// Desc: Wave reading (CWaveFile) and SDL3 playback (CSoundManager, CSound,
//       CStreamingSound) for the game's sound effects and BGM.
//
//       This file used to be Microsoft's DirectSound DSUtil framework. The
//       playback layer is now SDL3:
//
//         - every CSound keeps its PCM in memory and owns one audio stream,
//           opened with SDL_OpenAudioDeviceStream on the default playback
//           device. A get-callback pulls samples out of the sound whenever the
//           device needs them, which is what makes several effects play at
//           once without any explicit mixing code.
//         - CStreamingSound does the same for BGM, except that its samples are
//           not in memory; the callback reads the next chunk out of the
//           CWaveFile (thbgm.dat / th07.dat) instead.
//
//       The archive reading is unchanged: CWaveFile::ResetFile still computes
//       file positions from the ThBgmFormat fields, so the on-disk ZWAV layout
//       is treated exactly as before.
//-----------------------------------------------------------------------------
#define STRICT
#include "dsutil.hpp"
#include "SoundPlayer.hpp"
#include "dxutil.hpp"
#include <math.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

// 16-bit PCM frames -> a multiplier. The game speaks in hundredths of a
// decibel, the scale the old DirectSound buffer volume used, so convert:
// -100 dBFS is "silent" here (volume -5000 and below).
static f32 DsVolumeToLinear(i32 volume)
{
    if (volume <= -5000)
    {
        return 0.0f;
    }
    if (volume >= 0)
    {
        return 1.0f;
    }
    return (f32)pow(10.0, (f64)volume / 2000.0);
}

// Compile-time check that the callback mixers only ever see the formats
// DsWaveFormatToSdl() can produce.
#define SDLSND_ASSERT_PCM_FORMAT(f) \
    ((f) == SDL_AUDIO_S16LE || (f) == SDL_AUDIO_U8)
// The SDL format that matches a WAVE header without any conversion. Returns
// false for something that cannot be fed straight into a stream.
static bool DsWaveFormatToSdl(const WAVEFORMATEX *fx, SDL_AudioFormat *outFormat)
{
    if (!fx)
    {
        return false;
    }
    if (fx->wFormatTag != WAVE_FORMAT_PCM)
    {
        // WAVE_FORMAT_EXTENSIBLE (0xFFFE) and everything compressed is left to
        // SDL: the game only ships plain PCM.
        return false;
    }

    if (fx->wBitsPerSample == 8)
    {
        *outFormat = SDL_AUDIO_U8;
        return true;
    }
    if (fx->wBitsPerSample == 16)
    {
        *outFormat = SDL_AUDIO_S16LE;
        return true;
    }
    return false;
}

static void DsMakeSpec(const WAVEFORMATEX *fx, SDL_AudioFormat format,
                       SDL_AudioSpec *spec)
{
    spec->format = format;
    spec->channels = (int)fx->nChannels;
    spec->freq = (int)fx->nSamplesPerSec;
}

// Bytes of PCM that make up one sample frame of the stream.
static u32 DsFrameSize(const SDL_AudioSpec *spec)
{
    return (u32)SDL_AUDIO_BYTESIZE(spec->format) * (u32)spec->channels;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::CSoundManager()
// Desc: Constructs the class
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045c6f0 FOLDED
CSoundManager::CSoundManager()
{
    this->audioDevice = 0;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::~CSoundManager()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045c710
CSoundManager::~CSoundManager()
{
    this->Release();
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::Initialize()
// Desc: Initializes the SDL audio subsystem. This function must be called
//       before any others.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045c740
HRESULT CSoundManager::Initialize(HWND hWnd, DWORD dwCoopLevel,
                                  DWORD dwPrimaryChannels, DWORD dwPrimaryFreq,
                                  DWORD dwPrimaryBitRate)
{
    this->Release();

    // The window handle and cooperative level are DirectSound concepts; SDL
    // opens the default playback device instead.
    (void)hWnd;
    (void)dwCoopLevel;

    // SDL_InitSubSystem is reference counted and safe to call again: the video
    // subsystem is already up by the time the game gets here, and this adds
    // (or reuses) audio.
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        SDL_Log("audio: SDL_InitSubSystem(SDL_INIT_AUDIO) failed: %s",
                SDL_GetError());
        return E_FAIL;
    }

    // HRESULT success is S_OK == 0, so this has to be tested with FAILED() rather than
    // the logical-not a plain boolean would use.
    if (FAILED(SetPrimaryBufferFormat(dwPrimaryChannels, dwPrimaryFreq,
                                      dwPrimaryBitRate)))
    {
        SDL_Log("audio: SetPrimaryBufferFormat failed: %s", SDL_GetError());
        return E_FAIL;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::SetPrimaryBufferFormat()
// Desc: Checks that the device SDL would pick can play what the game asks for.
//       The explicit format is no longer set on a primary buffer; the request
//       is recorded so the per-sound streams know what to open.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045c7d0
HRESULT CSoundManager::SetPrimaryBufferFormat(DWORD dwPrimaryChannels,
                                              DWORD dwPrimaryFreq,
                                              DWORD dwPrimaryBitRate)
{
    SDL_AudioSpec spec;

    if (!SDL_WasInit(SDL_INIT_AUDIO))
    {
        return CO_E_NOTINITIALIZED;
    }

    SDL_zero(spec);
    spec.format = SDL_AUDIO_S16LE;
    spec.channels = (int)dwPrimaryChannels;
    spec.freq = (int)dwPrimaryFreq;
    if (dwPrimaryBitRate == 8)
    {
        spec.format = SDL_AUDIO_U8;
    }
    if (spec.channels <= 0)
    {
        spec.channels = 2;
    }
    if (spec.freq <= 0)
    {
        spec.freq = 44100;
    }

    // Nothing to bind yet; this just proves the subsystem is usable. SDL
    // converts to whatever the hardware wants, so checking the device list is
    // as much as is needed here.
    this->audioDevice = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::Release()
// Desc: Drops the SDL audio subsystem again.
//-----------------------------------------------------------------------------
void CSoundManager::Release()
{
    if (this->audioDevice != 0 && SDL_WasInit(SDL_INIT_AUDIO))
    {
        // Every stream is closed by the time this runs (SoundPlayer::Release
        // deletes the sounds first), so there is nothing left holding the
        // device. Quitting the subsystem is reference counted with the video
        // init that main.cpp/the renderer did.
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
    this->audioDevice = 0;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateStreaming()
// Desc: Opens a ZWAV archive entry as a streaming sound.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045c8e0
HRESULT CSoundManager::CreateStreaming(CStreamingSound **ppStreamingSound,
                                       LPCSTR strWaveFileName,
                                       DWORD dwCreationFlags,
                                       GUID guid3DAlgorithm,
                                       DWORD dwNotifyCount, DWORD dwNotifySize,
                                       ThBgmFormat *pzwf)
{
    CWaveFile *pWaveFile;

    // DirectSound buffer flags and notification parameters: meaningless to
    // SDL, which pulls data on its own schedule.
    (void)dwCreationFlags;
    (void)guid3DAlgorithm;
    (void)dwNotifyCount;
    (void)dwNotifySize;

    if (!this->IsValid())
    {
        return CO_E_NOTINITIALIZED;
    }

    pWaveFile = new CWaveFile();
    if (FAILED(pWaveFile->Open(strWaveFileName, pzwf, WAVEFILE_READ)))
    {
        delete pWaveFile;
        return E_FAIL;
    }

    *ppStreamingSound =
        new CStreamingSound(pWaveFile, pzwf, SDLSND_STREAM_CHUNK_SIZE);
    (*ppStreamingSound)->m_dwPriority = 0;

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSoundManager::CreateStreamingFromMemory()
// Desc: Opens a preloaded BGM blob as a streaming sound.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045cc30
HRESULT CSoundManager::CreateStreamingFromMemory(
    CStreamingSound **ppStreamingSound, u8 *pbData, ULONG ulDataSize,
    ThBgmFormat *pzwf, DWORD dwCreationFlags, GUID guid3DAlgorithm,
    DWORD dwNotifyCount, DWORD dwNotifySize)
{
    CWaveFile *pWaveFile;

    (void)dwCreationFlags;
    (void)guid3DAlgorithm;
    (void)dwNotifyCount;
    (void)dwNotifySize;

    // STRING: TH07 0x0049548c
    utils::DebugPrint("StreamingSound Create \r\n");
    if (!this->IsValid())
    {
        return CO_E_NOTINITIALIZED;
    }

    pWaveFile = new CWaveFile();
    pWaveFile->OpenFromMemory(pbData, ulDataSize, pzwf, 0);

    *ppStreamingSound =
        new CStreamingSound(pWaveFile, pzwf, SDLSND_STREAM_CHUNK_SIZE);
    (*ppStreamingSound)->m_dwPriority = 0;

    // STRING: TH07 0x00495480
    utils::DebugPrint("Success \r\n");

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSound::CSound()
// Desc: Constructs the class. The PCM is copied, so the caller may release
//       whatever buffer it passed in.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045cf50
CSound::CSound(u8 *pbData, DWORD dwDataSize, DWORD dwNumBuffers,
               CWaveFile *pWaveFile, const WAVEFORMATEX *pFormat)
{
    SDL_AudioFormat format;
    u8 *copy;
    i32 i;

    this->m_pAudioStream = NULL;
    SDL_zero(this->m_streamSpec);
    this->m_pData = NULL;
    this->m_dataSize = 0;
    this->m_pWaveFile = pWaveFile;
    this->m_dwNumBuffers = dwNumBuffers > 0 ? dwNumBuffers : 1;
    this->m_iCurFadeoutProgress = 0;
    this->m_iTotalFadeout = 0;
    this->m_dwIsFadingOut = 0;
    this->m_dwPriority = 0;
    this->m_dwFlags = 0;
    this->unused_28 = 0;
    this->unused_2c = 0;
    this->m_bIsPlaying = FALSE;
    this->m_voices = new Voice[this->m_dwNumBuffers];
    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        this->m_voices[i].position = 0;
        this->m_voices[i].length = 0;
        this->m_voices[i].gain = 1.0f;
        this->m_voices[i].active = false;
        this->m_voices[i].startedAt = 0;
    }
    this->m_soundVolume = 0;
    this->m_lock = SDL_CreateMutex();
    this->m_mixEpoch = SDL_GetTicks();

    // Keep our own copy of the samples: the caller frees its file buffer as
    // soon as LoadSound returns.
    copy = NULL;
    if (pbData && dwDataSize)
    {
        copy = (u8 *)malloc(dwDataSize);
        if (copy)
        {
            memcpy(copy, pbData, dwDataSize);
            this->m_pData = copy;
            this->m_dataSize = dwDataSize;
        }
    }

    if (pFormat)
    {
        this->ReopenStream(pFormat);
    }
    else
    {
        SDL_AudioSpec spec;
        SDL_zero(spec);
        spec.format = SDL_AUDIO_S16LE;
        spec.channels = 2;
        spec.freq = 44100;
        this->m_streamSpec = spec;
    }
}

//-----------------------------------------------------------------------------
// Name: CSound::~CSound()
// Desc: Destroys the class
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d2c0
CSound::~CSound()
{
    // Destroying the stream unbinds it from the device and closes the device
    // that SDL_OpenAudioDeviceStream opened, so the audio callbacks stop here.
    if (this->m_pAudioStream)
    {
        SDL_DestroyAudioStream(this->m_pAudioStream);
    }
    this->m_pAudioStream = NULL;
    if (this->m_lock)
    {
        SDL_DestroyMutex(this->m_lock);
    }
    this->m_lock = NULL;
    if (this->m_pData)
    {
        free((void *)this->m_pData);
    }
    this->m_pData = NULL;
    this->m_dataSize = 0;
    delete[] this->m_voices;
    this->m_voices = NULL;
    delete this->m_pWaveFile;
    this->m_pWaveFile = NULL;
}

//-----------------------------------------------------------------------------
// Name: CSound::SetVolume()
// Desc: Sets how loud this sound plays, in hundredths of a decibel, the same
//       units the old DirectSound buffer volume used. Applied per voice when the
//       samples are mixed.
//-----------------------------------------------------------------------------
void CSound::SetVolume(i32 dwVolume)
{
    this->m_soundVolume = dwVolume;
}

//-----------------------------------------------------------------------------
// Name: CSound::ReopenStream()
// Desc: (Re)creates the audio stream with the format of the WAV being played,
//       so the device never has to guess.
//-----------------------------------------------------------------------------
void CSound::ReopenStream(const WAVEFORMATEX *format)
{
    SDL_AudioFormat sdlFormat;
    SDL_AudioSpec spec;
    SDL_AudioStream *stream;
    i32 i;

    if (!format || !DsWaveFormatToSdl(format, &sdlFormat))
    {
        return;
    }
    if (format->nChannels == 0 || format->nChannels > 8 ||
        format->nSamplesPerSec == 0)
    {
        return;
    }

    DsMakeSpec(format, sdlFormat, &spec);

    SDL_LockMutex(this->m_lock);
    if (this->m_pAudioStream)
    {
        SDL_DestroyAudioStream(this->m_pAudioStream);
        this->m_pAudioStream = NULL;
    }
    this->m_streamSpec = spec;
    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        this->m_voices[i].active = false;
        this->m_voices[i].position = 0;
        this->m_voices[i].length = 0;
    }
    this->m_bIsPlaying = FALSE;
    SDL_UnlockMutex(this->m_lock);

    // SDL_OpenAudioDeviceStream takes the stream's own format as the spec and
    // leaves the device paused until the first Play().
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec,
                                       CSound::StreamGetCallback, this);
    if (!stream)
    {
        SDL_Log("audio: SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        return;
    }

    SDL_LockMutex(this->m_lock);
    this->m_pAudioStream = stream;
    SDL_UnlockMutex(this->m_lock);
}

//-----------------------------------------------------------------------------
// Name: CSound::StreamGetCallback()
// Desc: SDL asks for more samples; mix the active voices into the stream.
//       Runs on whichever thread drives the audio device.
//-----------------------------------------------------------------------------
void SDLCALL CSound::StreamGetCallback(void *userdata, SDL_AudioStream *stream,
                                       int additional_amount,
                                       int total_amount)
{
    (void)total_amount;
    ((CSound *)userdata)->StreamGet(stream, additional_amount);
}

void CSound::StreamGet(SDL_AudioStream *stream, int additional_amount)
{
    u8 *scratch;

    if (!stream || additional_amount <= 0 || this->FadeOutShouldSkip())
    {
        return;
    }

    scratch = (u8 *)malloc((size_t)additional_amount);
    if (!scratch)
    {
        return;
    }
    memset(scratch, 0, (size_t)additional_amount);
    this->MixVoices(scratch, (u32)additional_amount, this->m_streamSpec);
    SDL_PutAudioStreamData(stream, scratch, additional_amount);
    free(scratch);
}

// True when a fade-out scheduled by FadeOut() has run its course. Called from
// the audio thread, so the counters are only read.
bool CSound::FadeOutShouldSkip()
{
    return this->m_dwIsFadingOut != 0 && this->m_iCurFadeoutProgress <= 0;
}

//-----------------------------------------------------------------------------
// Name: CSound::MixVoices()
// Desc: Adds every active voice into the 8/16-bit PCM buffer, scaled by the
//       voice gain and the sound volume.
//-----------------------------------------------------------------------------
void CSound::MixVoices(u8 *buffer, u32 bytes, const SDL_AudioSpec &spec)
{
    u32 frameSize;
    u32 frames;
    u32 frame;
    i32 i;
    f32 master;
    Uint64 elapsed;

    if (!this->m_pData || this->m_dataSize == 0)
    {
        return;
    }

    frameSize = DsFrameSize(&spec);
    if (frameSize == 0 || !SDLSND_ASSERT_PCM_FORMAT(spec.format))
    {
        return;
    }
    frames = bytes / frameSize;
    master = DsVolumeToLinear(this->m_soundVolume);
    // Timestamped voices are placed by when they started, not by how many
    // samples have already been mixed: SDL only calls this when the device wants
    // data, so mixing alone would stretch a short effect over a long idle gap.
    elapsed = 0;
    if (spec.freq > 0)
    {
        elapsed = SDL_GetTicks() - this->m_mixEpoch;
    }

    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        Voice *voice = &this->m_voices[i];
        u32 start = voice->position;
        f32 gain;

        if (!voice->active)
        {
            continue;
        }

        if (voice->startedAt)
        {
            Uint64 sinceStart = SDL_GetTicks() - voice->startedAt;
            sinceStart = sinceStart > elapsed ? sinceStart - elapsed : 0;
            start = (u32)(sinceStart * (Uint64)spec.freq *
                          (Uint64)frameSize / 1000u);
            start -= start % frameSize;
            if (start >= voice->length)
            {
                voice->active = false;
                continue;
            }
        }

        gain = voice->gain * master;
        for (frame = 0; frame < frames; frame++)
        {
            u32 at = start + frame * frameSize;
            u32 out = frame * frameSize;

            if (at >= voice->length || at + frameSize > this->m_dataSize)
            {
                break;
            }

            if (spec.format == SDL_AUDIO_S16LE)
            {
                for (i32 channel = 0; channel < spec.channels; channel++)
                {
                    i16 sample = *(const i16 *)(this->m_pData + at + channel * 2);
                    // Halve what is already there so a pile-up of effects
                    // cannot clip instantly.
                    i32 mixed = *(i16 *)(buffer + out + channel * 2) / 2;
                    mixed += (i32)((f32)sample * gain);
                    if (mixed > 32767)
                    {
                        mixed = 32767;
                    }
                    else if (mixed < -32768)
                    {
                        mixed = -32768;
                    }
                    *(i16 *)(buffer + out + channel * 2) = (i16)mixed;
                }
            }
            else if (spec.format == SDL_AUDIO_U8)
            {
                for (i32 channel = 0; channel < spec.channels; channel++)
                {
                    // U8 PCM is centred on 128.
                    i32 sample = (i32)this->m_pData[at + channel] - 128;
                    i32 mixed = ((i32)buffer[out + channel] - 128) / 2;
                    mixed += (i32)((f32)sample * gain);
                    if (mixed > 127)
                    {
                        mixed = 127;
                    }
                    else if (mixed < -128)
                    {
                        mixed = -128;
                    }
                    buffer[out + channel] = (u8)(mixed + 128);
                }
            }
        }

        if (!voice->startedAt)
        {
            voice->position = start + frames * frameSize;
            if (voice->position >= voice->length)
            {
                voice->active = false;
            }
        }
    }

    this->m_mixEpoch += (Uint64)frames * 1000u / (Uint64)(spec.freq > 0 ? spec.freq : 1);
}

//-----------------------------------------------------------------------------
// Name: CSound::Pump()
// Desc: Refill the stream after its contents were consumed. Left as a no-op:
//       the get-callback does this, and queueing here as well would play every
//       sound twice.
//-----------------------------------------------------------------------------
u32 CSound::Pump()
{
    return 0;
}

//-----------------------------------------------------------------------------
// Name: CSound::StopVoices()
// Desc: Silences every voice of this sound.
//-----------------------------------------------------------------------------
void CSound::StopVoices()
{
    i32 i;

    SDL_LockMutex(this->m_lock);
    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        this->m_voices[i].active = false;
        this->m_voices[i].position = 0;
        this->m_voices[i].length = 0;
        this->m_voices[i].startedAt = 0;
    }
    this->m_bIsPlaying = FALSE;
    SDL_UnlockMutex(this->m_lock);
}

//-----------------------------------------------------------------------------
// Name: CSound::CountVoices()
// Desc: How many voices are currently sounding.
//-----------------------------------------------------------------------------
u32 CSound::CountVoices()
{
    u32 count = 0;
    i32 i;

    SDL_LockMutex(this->m_lock);
    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        if (this->m_voices[i].active)
        {
            count++;
        }
    }
    SDL_UnlockMutex(this->m_lock);
    return count;
}

//-----------------------------------------------------------------------------
// Name: CSound::IsAudioPending()
// Desc: True while this sound still has samples queued for the device.
//-----------------------------------------------------------------------------
bool CSound::IsAudioPending()
{
    bool pending;

    SDL_LockMutex(this->m_lock);
    pending = this->m_pAudioStream != NULL &&
              SDL_GetAudioStreamQueued(this->m_pAudioStream) > 0;
    SDL_UnlockMutex(this->m_lock);
    return pending;
}

//-----------------------------------------------------------------------------
// Name: CSound::Play()
// Desc: Starts a new voice.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d760
HRESULT CSound::Play(DWORD dwPriority, DWORD dwFlags)
{
    CStreamingSound *streaming;
    i32 slot;
    i32 i;
    u32 frameSize;

    if (!this->m_pAudioStream || !this->m_pData || this->m_dataSize == 0)
    {
        return CO_E_NOTINITIALIZED;
    }

    frameSize = DsFrameSize(&this->m_streamSpec);
    if (frameSize == 0)
    {
        return E_FAIL;
    }

    SDL_LockMutex(this->m_lock);

    // First idle voice, otherwise recycle the one that is furthest along.
    slot = -1;
    for (i = 0; i < (i32)this->m_dwNumBuffers; i++)
    {
        if (!this->m_voices[i].active)
        {
            slot = i;
            break;
        }
        if (slot < 0 || this->m_voices[i].position > this->m_voices[slot].position)
        {
            slot = i;
        }
    }
    if (slot < 0)
    {
        SDL_UnlockMutex(this->m_lock);
        return E_FAIL;
    }

    this->m_voices[slot].position = 0;
    this->m_voices[slot].length = this->m_dataSize - this->m_dataSize % frameSize;
    this->m_voices[slot].gain = 1.0f;
    this->m_voices[slot].active = true;
    // One-shot effects get a start time; streaming sounds have none.
    streaming = dynamic_cast<CStreamingSound *>(this);
    if (streaming == NULL)
    {
        this->m_voices[slot].startedAt = SDL_GetTicks();
    }
    else
    {
        this->m_voices[slot].startedAt = 0;
    }

    this->m_dwIsFadingOut = 0;
    this->m_iCurFadeoutProgress = 0;
    this->m_iTotalFadeout = 0;
    this->m_bIsPlaying = TRUE;
    this->m_dwPriority = dwPriority;
    this->m_dwFlags = dwFlags;
    this->unused_2c = 0;

    SDL_UnlockMutex(this->m_lock);

    if (SDL_AudioStreamDevicePaused(this->m_pAudioStream))
    {
        SDL_ResumeAudioStreamDevice(this->m_pAudioStream);
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSound::Stop()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d860
u32 CSound::Stop()
{
    this->StopVoices();
    this->m_dwIsFadingOut = 0;

    SDL_LockMutex(this->m_lock);
    if (this->m_pAudioStream)
    {
        // Whatever is already queued is still played; dropping it here would
        // cut sound effects off and make the BGM restart in the middle of a
        // sample.
        SDL_PauseAudioStreamDevice(this->m_pAudioStream);
        SDL_ClearAudioStream(this->m_pAudioStream);
    }
    SDL_UnlockMutex(this->m_lock);
    return 0;
}

//-----------------------------------------------------------------------------
// Name: CSound::Pause()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d910
HRESULT CSound::Pause()
{
    if (!this->m_pAudioStream)
    {
        return CO_E_NOTINITIALIZED;
    }

    this->m_bIsPlaying = FALSE;
    SDL_PauseAudioStreamDevice(this->m_pAudioStream);
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSound::Unpause()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d960
HRESULT CSound::Unpause()
{
    if (!this->m_pAudioStream)
    {
        return CO_E_NOTINITIALIZED;
    }

    this->m_bIsPlaying = TRUE;
    SDL_ResumeAudioStreamDevice(this->m_pAudioStream);
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSound::Reset()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d9b0
HRESULT CSound::Reset()
{
    if (!this->m_pAudioStream)
    {
        return CO_E_NOTINITIALIZED;
    }

    this->Stop();
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CSound::FadeOut()
// Desc: Ramp the sound down over `duration` frames (UpdateFadeOut counts them)
//       and silence it when the ramp reaches zero.
//-----------------------------------------------------------------------------
void CSound::FadeOut(f32 duration)
{
    this->m_dwIsFadingOut = 1;
    this->m_iCurFadeoutProgress = (i32)(duration * 60.0f);
    this->m_iTotalFadeout = this->m_iCurFadeoutProgress;
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::CStreamingSound()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045da20
CStreamingSound::CStreamingSound(CWaveFile *pWaveFile, ThBgmFormat *pFormat,
                                 DWORD dwNotifySize)
    : CSound(NULL, 0, 1, pWaveFile,
             pFormat ? &pFormat->format : (const WAVEFORMATEX *)NULL)
{
    this->m_dwLastPlayPos = 0;
    this->m_dwPlayProgress = 0;
    this->m_dwNotifySize = dwNotifySize;
    this->m_dwNextWriteOffset = 0;
    this->m_bFillNextNotificationWithSilence = FALSE;
    this->m_hNotifyEvent = NULL;
    this->m_bIsLocked = FALSE;
    // BGM plays from the queue only: its data is produced by the BGM thread at
    // whatever rate that thread gets to run, so it must never be re-timed
    // against the wall clock the way the one-shot effects are.
    this->m_voices[0].startedAt = 0;
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::~CStreamingSound()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045dab0
CStreamingSound::~CStreamingSound()
{
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::UpdateFadeOut()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045dad0
HRESULT CStreamingSound::UpdateFadeOut()
{
    if (this->m_dwIsFadingOut)
    {
        if (--this->m_iCurFadeoutProgress <= 0)
        {
            this->m_dwIsFadingOut = 0;
            this->Stop();
            return S_FALSE;
        }
        // The original mapped this to the buffer volume in hundredths of a
        // decibel; keep the same curve for the BGM.
        this->SetVolume(this->m_iCurFadeoutProgress * 5000 / this->m_iTotalFadeout -
                        5000);
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::InitSoundBuffers()
// Desc: Recreates the stream (the original recreated the DirectSound buffers,
//       which was needed after the device was lost).
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045d060
HRESULT CStreamingSound::InitSoundBuffers()
{
    SDL_AudioStream *stream;

    if (!this->m_pWaveFile || !this->m_pWaveFile->m_pzwf)
    {
        return E_FAIL;
    }

    this->m_bIsPlaying = FALSE;
    SDL_LockMutex(this->m_lock);
    if (this->m_pAudioStream)
    {
        SDL_DestroyAudioStream(this->m_pAudioStream);
        this->m_pAudioStream = NULL;
    }
    SDL_UnlockMutex(this->m_lock);

    this->ReopenStream(&this->m_pWaveFile->m_pzwf->format);
    stream = this->m_pAudioStream;
    return stream ? S_OK : E_FAIL;
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::HandleWaveStreamNotification()
// Desc: Top up the stream from the archive. Called from the BGM thread, so
//       this is the one place that reads the file.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045db60
HRESULT CStreamingSound::HandleWaveStreamNotification(BOOL bLoopedPlay)
{
    u8 *chunk;
    u32 want;
    DWORD got;
    DWORD chunkUsed;

    if (!this->m_pAudioStream || !this->m_pWaveFile)
    {
        return CO_E_NOTINITIALIZED;
    }

    if (this->m_bFillNextNotificationWithSilence)
    {
        return S_OK;
    }

    if (SDL_GetAudioStreamQueued(this->m_pAudioStream) >= SDLSND_STREAM_MAX_QUEUED)
    {
        // Already plenty buffered; try again on the next tick.
        return S_OK;
    }

    want = this->m_dwNotifySize;
    if (want == 0)
    {
        want = SDLSND_STREAM_CHUNK_SIZE;
    }

    chunk = (u8 *)malloc(want);
    if (!chunk)
    {
        return E_OUTOFMEMORY;
    }

    if (FAILED(this->m_pWaveFile->Read(chunk, want, &got)))
    {
        // STRING: TH07 0x00495384
        utils::DebugPrint("error : m_pWaveFile->Read in HandleWaveStreamNotification\r\n");
        free(chunk);
        return E_FAIL;
    }

    if (got < want)
    {
        if (!bLoopedPlay)
        {
            this->m_bFillNextNotificationWithSilence = TRUE;
        }
        else
        {
            DWORD readSoFar = got;

            while (readSoFar < want)
            {
                if (FAILED(this->m_pWaveFile->ResetFile(true)))
                {
                    // STRING: TH07 0x00495340
                    utils::DebugPrint("error : m_pWaveFile->ResetFile in HandleWaveStreamNotification\r\n");
                    break;
                }

                if (FAILED(this->m_pWaveFile->Read(chunk + readSoFar,
                                                   want - readSoFar, &got)))
                {
                    // STRING: TH07 0x00495300
                    utils::DebugPrint("error : m_pWaveFile->Read(+) in HandleWaveStreamNotification\r\n");
                    break;
                }
                if (got == 0)
                {
                    break;
                }
                readSoFar += got;
            }
            got = readSoFar;
        }
    }

    if (got > 0)
    {
        if (!SDL_PutAudioStreamData(this->m_pAudioStream, chunk, (int)got))
        {
            SDL_Log("audio: SDL_PutAudioStreamData failed: %s", SDL_GetError());
        }

        chunkUsed = got;
        this->m_dwLastPlayPos = chunkUsed;
        this->m_dwPlayProgress += chunkUsed;
        this->m_dwNextWriteOffset += chunkUsed;
    }
    free(chunk);

    if (this->m_bFillNextNotificationWithSilence)
    {
        // The track has ended; stop once what is already queued has played.
        if (!this->IsAudioPending())
        {
            this->m_bIsPlaying = FALSE;
            if (this->m_pAudioStream)
            {
                SDL_PauseAudioStreamDevice(this->m_pAudioStream);
            }
        }
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CStreamingSound::Reset()
// Desc: Rewind the current track and drop anything queued.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045df50
HRESULT CStreamingSound::Reset()
{
    if (!this->m_pAudioStream || !this->m_pWaveFile)
    {
        return CO_E_NOTINITIALIZED;
    }

    this->m_dwLastPlayPos = 0;
    this->m_dwPlayProgress = 0;
    this->m_dwNextWriteOffset = 0;
    this->m_bFillNextNotificationWithSilence = FALSE;
    this->m_pWaveFile->ResetFile(false);
    SDL_ClearAudioStream(this->m_pAudioStream);
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::CWaveFile()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e020
CWaveFile::CWaveFile()
{
    memset(&this->m_ck, 0, sizeof(this->m_ck));
    memset(&this->m_ckRiff, 0, sizeof(this->m_ckRiff));
    memset(&this->m_mmioinfo, 0, sizeof(this->m_mmioinfo));
    this->m_pzwf = NULL;
    this->h_mmio = NULL;
    this->m_dwSize = 0;
    this->m_bIsReadingFromMemory = FALSE;
    this->m_dwFlags = 0;
    this->m_pbData = NULL;
    this->m_pbDataCur = NULL;
    this->m_ulDataSize = 0;
    this->m_hWaveFile = INVALID_HANDLE_VALUE;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::~CWaveFile()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e060
CWaveFile::~CWaveFile()
{
    Close();
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::Open()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e080
HRESULT CWaveFile::Open(LPCSTR strFileName, ThBgmFormat *pzwf, DWORD dwFlags)
{
    this->m_dwFlags = dwFlags;
    this->m_bIsReadingFromMemory = FALSE;

    if (this->m_dwFlags == WAVEFILE_READ)
    {
        if (!strFileName)
        {
            return E_INVALIDARG;
        }

        // STRING: TH07 0x00495294
        utils::DebugPrint("Streaming File Open %s\r\n", strFileName);
        this->m_hWaveFile = CreateFileA(
            strFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
        if (this->m_hWaveFile == INVALID_HANDLE_VALUE)
        {
            return E_FAIL;
        }

        this->m_pzwf = pzwf;
        if (!pzwf)
        {
            CloseHandle(this->m_hWaveFile);
            this->m_hWaveFile = INVALID_HANDLE_VALUE;
            return E_INVALIDARG;
        }
        ResetFile(false);
        this->m_dwSize = this->m_ck.cksize;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::Reopen()
// Desc: Point the file at another ZWAV entry (another track).
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e130
HRESULT CWaveFile::Reopen(ThBgmFormat *pzwf)
{
    if (this->m_bIsReadingFromMemory)
    {
        return E_FAIL;
    }

    if (this->m_hWaveFile == INVALID_HANDLE_VALUE)
    {
        return E_FAIL;
    }

    if (!pzwf)
    {
        return E_INVALIDARG;
    }

    this->m_pzwf = pzwf;
    ResetFile(false);
    this->m_dwSize = this->m_ck.cksize;
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::OpenFromMemory()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e190
HRESULT CWaveFile::OpenFromMemory(u8 *pbData, ULONG ulDataSize,
                                  ThBgmFormat *pzwf, DWORD dwFlags)
{
    this->m_pzwf = pzwf;
    this->m_ulDataSize = ulDataSize;
    this->m_pbData = pbData;
    this->m_pbDataCur = this->m_pbData;
    this->m_bIsReadingFromMemory = TRUE;

    if (dwFlags != WAVEFILE_READ)
    {
        return E_NOTIMPL;
    }

    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::GetSize()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e1f0
DWORD CWaveFile::GetSize()
{
    return this->m_dwSize;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::ResetFile()
// Desc: Seek to the start of the entry (or to its loop point). This is the
//       thbgm.dat layout logic and is unchanged from the DirectSound version.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e210
HRESULT CWaveFile::ResetFile(bool bLoop)
{
    DWORD unk;

    if (this->m_bIsReadingFromMemory)
    {
        if (!this->m_pzwf)
        {
            return E_INVALIDARG;
        }
        this->m_pbDataCur = this->m_pbData;
        if (this->m_pzwf->totalLength > 0)
        {
            this->m_ulDataSize = this->m_pzwf->totalLength;
        }
        if (bLoop && this->m_pzwf->introLength > 0)
        {
            this->m_pbDataCur += this->m_pzwf->introLength;
        }
    }
    else
    {
        if (!this->m_hWaveFile || this->m_hWaveFile == INVALID_HANDLE_VALUE)
        {
            return CO_E_NOTINITIALIZED;
        }
        if (!this->m_pzwf)
        {
            return E_INVALIDARG;
        }
        if (bLoop && this->m_pzwf->introLength > 0)
        {
            unk = SetFilePointer(this->m_hWaveFile,
                                 g_SoundPlayer.bgmSeekOffset +
                                     this->m_pzwf->introLength +
                                     this->m_pzwf->startOffset,
                                 NULL, FILE_BEGIN);
            this->m_ck.cksize = this->m_pzwf->totalLength - this->m_pzwf->introLength;
        }
        else
        {
            unk = SetFilePointer(this->m_hWaveFile,
                                 g_SoundPlayer.bgmSeekOffset +
                                     this->m_pzwf->startOffset,
                                 NULL, FILE_BEGIN);
            this->m_ck.cksize = this->m_pzwf->totalLength;
        }
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::Read()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e360
HRESULT CWaveFile::Read(u8 *pBuffer, DWORD dwSizeToRead, DWORD *pdwSizeRead)
{
    DWORD bytesRead;
    DWORD sizeToRead;

    if (this->m_bIsReadingFromMemory)
    {
        if (!this->m_pbDataCur)
        {
            return CO_E_NOTINITIALIZED;
        }
        if (pdwSizeRead)
        {
            *pdwSizeRead = 0;
        }

        if ((DWORD)(this->m_pbDataCur + dwSizeToRead) >
            (DWORD)(this->m_pbData + this->m_ulDataSize))
        {
            dwSizeToRead =
                this->m_ulDataSize - (DWORD)(this->m_pbDataCur - this->m_pbData);
        }

        memcpy(pBuffer, this->m_pbDataCur, dwSizeToRead);
        this->m_pbDataCur += dwSizeToRead;

        if (pdwSizeRead)
        {
            *pdwSizeRead = dwSizeToRead;
        }
        return S_OK;
    }

    if (!this->m_hWaveFile || this->m_hWaveFile == INVALID_HANDLE_VALUE)
    {
        return CO_E_NOTINITIALIZED;
    }

    if (!pBuffer || !pdwSizeRead)
    {
        return E_INVALIDARG;
    }

    sizeToRead = dwSizeToRead;
    if (sizeToRead > this->m_ck.cksize)
    {
        sizeToRead = this->m_ck.cksize;
    }
    this->m_ck.cksize -= sizeToRead;

    bytesRead = 0;
    ReadFile(this->m_hWaveFile, pBuffer, sizeToRead, &bytesRead, NULL);
    if (pdwSizeRead)
    {
        *pdwSizeRead = bytesRead;
    }
    return S_OK;
}

//-----------------------------------------------------------------------------
// Name: CWaveFile::Close()
// Desc:
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0045e4b0
HRESULT CWaveFile::Close()
{
    if (this->m_dwFlags == WAVEFILE_READ)
    {
        if (this->m_hWaveFile && this->m_hWaveFile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(this->m_hWaveFile);
        }
        this->m_hWaveFile = INVALID_HANDLE_VALUE;
    }
    return S_OK;
}
