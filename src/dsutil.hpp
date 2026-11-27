//-----------------------------------------------------------------------------
// File: dsutil.hpp
//
// Desc: Wave reading and sound playback for the game's audio.
//
//       This started life as Microsoft's DirectSound DSUtil framework. The
//       playback half now runs on SDL3: every CSound owns its own
//       SDL_AudioStream (opened with SDL_OpenAudioDeviceStream and driven by a
//       get-callback), so several sounds can play at the same time and SDL
//       does the mixing and any format conversion the device needs.
//       CStreamingSound keeps its samples out of memory: it pulls them from
//       the archive a chunk at a time and streams them the same way.
//
//       What did NOT change is the file layout of thbgm.dat: ThBgmFormat is
//       still parsed straight out of the file, and CWaveFile still seeks
//       inside it with the same offsets. That is why ThBgmFormat's size is
//       still asserted.
//-----------------------------------------------------------------------------
#pragma once

#include <SDL3/SDL.h>
#include <windows.h>
#include <mmsystem.h>

#include "inttypes.hpp"

//-----------------------------------------------------------------------------
// Classes used by this header
//-----------------------------------------------------------------------------
class CSoundManager;
class CSound;
class CStreamingSound;
class CWaveFile;
struct SoundPlayer;

//-----------------------------------------------------------------------------
// Typing macros
//-----------------------------------------------------------------------------
#define WAVEFILE_READ 1
#define WAVEFILE_WRITE 2

// Playing flags. These used to be the DirectSound play flags passed straight
// through to the buffer; only "looping or not" ever mattered to the game.
#define SDLSND_PLAY_ONCE 0
#define SDLSND_PLAY_LOOPING 1

// The number of bytes CStreamingSound feeds the stream per chunk. The game's
// own StartBGM/LoadBGM compute this value and hand it over (the original used
// it to slice the DirectSound ring buffer into notify regions).
#define SDLSND_STREAM_CHUNK_SIZE (64 * 1024)

// Once this much PCM is queued and waiting for the device, the BGM consumer
// stops pulling from the archive until it has drained again.
#define SDLSND_STREAM_MAX_QUEUED (256 * 1024)

#define DSUtil_StopSound(s) \
    {                       \
        if (s)              \
            s->Stop();      \
    }
#define DSUtil_PlaySound(s) \
    {                       \
        if (s)              \
            s->Play(0, 0);  \
    }
#define DSUtil_PlaySoundLooping(s)              \
    {                                           \
        if (s)                                  \
            s->Play(0, SDLSND_PLAY_LOOPING);    \
    }

struct ThBgmFormat
{
    char name[16];
    i32 startOffset;
    DWORD preloadAllocSize;
    i32 introLength;
    i32 totalLength;
    WAVEFORMATEX format;
    // pad 2
};
// ThBgmFormat is the record format of the bgm/thbgm.fmt table (and of the
// ZWAV header in thbgm.dat), read straight out of the data files. The layout
// on disk is fixed, so this assertion has to stay.
C_ASSERT(sizeof(ThBgmFormat) == 0x34);

//-----------------------------------------------------------------------------
// Name: class CWaveFile
// Desc: Encapsulates reading a LZSS blob or a file inside a ZWAV archive.
//       Nothing DirectSound-specific is left here; it is a byte source and the
//       ZUN thbgm seek bookkeeping around it.
//-----------------------------------------------------------------------------
class CWaveFile
{
  public:
    HMMIO h_mmio;
    MMCKINFO m_ck;
    MMCKINFO m_ckRiff;
    DWORD m_dwSize;
    MMIOINFO m_mmioinfo;
    DWORD m_dwFlags;
    BOOL m_bIsReadingFromMemory;
    u8 *m_pbData;
    u8 *m_pbDataCur;
    ULONG m_ulDataSize;
    HANDLE m_hWaveFile;
    ThBgmFormat *m_pzwf;

    CWaveFile();
    ~CWaveFile();

    HRESULT Open(LPCSTR strFileName, ThBgmFormat *pzwf, DWORD dwFlags);
    HRESULT OpenFromMemory(u8 *pbData, ULONG ulDataSize, ThBgmFormat *pzwf,
                           DWORD dwFlags);
    HRESULT Close();

    HRESULT Read(u8 *pBuffer, DWORD dwSizeToRead, DWORD *pdwSizeRead);
    DWORD GetSize();
    HRESULT ResetFile(bool bLoop);
    HRESULT Reopen(ThBgmFormat *pzwf);

    ThBgmFormat *GetFormat()
    {
        return this->m_pzwf;
    }
};

//-----------------------------------------------------------------------------
// Name: class CSoundManager
// Desc: Owns the SDL audio subsystem. Sounds create their own device-backed
//       streams, so this is a thin "is audio available" guard plus the
//       factories that the game calls.
//-----------------------------------------------------------------------------
class CSoundManager
{
  public:
    SDL_AudioDeviceID audioDevice;

    CSoundManager();
    ~CSoundManager();

    HRESULT Initialize(HWND hWnd, DWORD dwCoopLevel, DWORD dwPrimaryChannels,
                       DWORD dwPrimaryFreq, DWORD dwPrimaryBitRate);
    bool IsValid()
    {
        return this->audioDevice != 0;
    }
    HRESULT SetPrimaryBufferFormat(DWORD dwPrimaryChannels, DWORD dwPrimaryFreq,
                                   DWORD dwPrimaryBitRate);
    void Release();

    // The dwCreationFlags/guid3DAlgorithm arguments are the caller's DirectSound
    // buffer flags (DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY).
    // SDL3 needs none of that, so they are accepted and ignored.
    HRESULT CreateStreaming(CStreamingSound **ppStreamingSound,
                            LPCSTR strWaveFileName, DWORD dwCreationFlags,
                            GUID guid3DAlgorithm, DWORD dwNotifyCount,
                            DWORD dwNotifySize, ThBgmFormat *pzwf);
    HRESULT CreateStreamingFromMemory(CStreamingSound **ppStreamingSound,
                                      u8 *pbData, ULONG ulDataSize,
                                      ThBgmFormat *pzwf, DWORD dwCreationFlags,
                                      GUID guid3DAlgorithm, DWORD dwNotifyCount,
                                      DWORD dwNotifySize);
};

//-----------------------------------------------------------------------------
// Name: class CSound
// Desc: A WAV held in memory and played through its own SDL_AudioStream.
//       Each Play() starts a new voice, so the same sound can overlap itself
//       (the game fires se_plst00 many times per frame). At most
//       m_dwNumBuffers voices are alive; the oldest is recycled beyond that.
//-----------------------------------------------------------------------------
class CSound
{
  public:
    CSound(u8 *pbData, DWORD dwDataSize, DWORD dwNumBuffers,
           CWaveFile *pWaveFile, const WAVEFORMATEX *pFormat);
    virtual ~CSound();

    // SDL_OpenAudioDeviceStream's get-callback: runs on the thread that drives
    // the device, and fills the stream whenever the device wants samples.
    static void SDLCALL StreamGetCallback(void *userdata, SDL_AudioStream *stream,
                                          int additional_amount,
                                          int total_amount);
    void StreamGet(SDL_AudioStream *stream, int additional_amount);
    bool FadeOutShouldSkip();
    void MixVoices(u8 *buffer, u32 bytes, const SDL_AudioSpec &spec);

    // Feed the device whatever it asks for. Kept for the old call shape; SDL's
    // callback does the work, so this reports 0.
    u32 Pump();
    void StopVoices();
    u32 CountVoices();
    // True while any voice still has samples waiting to be played.
    bool IsAudioPending();
    void SetVolume(i32 dwVolume);
    // Reopen the stream with the format of the WAV being played, so the device
    // never has to guess.
    void ReopenStream(const WAVEFORMATEX *format);

    HRESULT Play(DWORD dwPriority, DWORD dwFlags);
    u32 Stop();
    HRESULT Reset();
    HRESULT Pause();
    HRESULT Unpause();
    void FadeOut(f32 duration);

    // A "voice" is one concurrent playback of the sound.
    struct Voice
    {
        u32 position;
        u32 length;
        f32 gain;
        bool active;
        // Non-zero for one-shot voices: the SDL tick the voice starts at, so a
        // voice that was silent while the device had nothing to send does not
        // replay late. Streaming (BGM) voices leave it at 0 and play purely from
        // the queue, because the BGM thread feeds that queue whenever it can.
        Uint64 startedAt;
    };

    SDL_AudioStream *m_pAudioStream;
    SDL_AudioSpec m_streamSpec;
    const u8 *m_pData;
    u32 m_dataSize;
    CWaveFile *m_pWaveFile;
    DWORD m_dwNumBuffers;
    i32 m_iCurFadeoutProgress;
    i32 m_iTotalFadeout;
    DWORD m_dwIsFadingOut;
    DWORD m_dwPriority;
    DWORD m_dwFlags;
    DWORD unused_28;
    DWORD unused_2c;
    BOOL m_bIsPlaying;
    Voice *m_voices;
    i32 m_soundVolume;
    SDL_Mutex *m_lock;
    // Milliseconds of audio mixed on stream callbacks so far; timeline for the
    // timestamped one-shot voices.
    Uint64 m_mixEpoch;
};

//-----------------------------------------------------------------------------
// Name: class CStreamingSound
// Desc: Plays a CWaveFile by pulling chunks out of it on demand.
//-----------------------------------------------------------------------------
class CStreamingSound : public CSound
{
  public:
    DWORD m_dwLastPlayPos;
    DWORD m_dwPlayProgress;
    DWORD m_dwNextWriteOffset;
    BOOL m_bFillNextNotificationWithSilence;
    DWORD m_dwNotifySize;
    HANDLE m_hNotifyEvent;
    BOOL m_bIsLocked;

    CStreamingSound(CWaveFile *pWaveFile, ThBgmFormat *pFormat,
                    DWORD dwNotifySize);
    ~CStreamingSound();

    HRESULT HandleWaveStreamNotification(BOOL bLoopedPlay);
    HRESULT Reset();
    HRESULT InitSoundBuffers();
    HRESULT UpdateFadeOut();

    void FadeOut(f32 duration)
    {
        this->m_dwIsFadingOut = 1;
        this->m_iCurFadeoutProgress = (i32)(duration * 60.0f);
        this->m_iTotalFadeout = this->m_iCurFadeoutProgress;
    }

    CWaveFile *GetWaveFile()
    {
        return this->m_pWaveFile;
    }
};
