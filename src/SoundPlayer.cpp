#include "SoundPlayer.hpp"

#include "dxutil.hpp"
#include "i18n.hpp"
#include "utils.hpp"

// Game state this file reads: the configured window (KillTimer), the audio
// settings in g_Supervisor.cfg and the error log. Deliberately the only
// reference to Supervisor.hpp in the translation unit: that header still pulls
// in the Direct3D 8 types, and RenderCompat.hpp defines its own versions of
// several of them, so whichever header comes second fails to compile.
#include "Global.hpp"
#include "Supervisor.hpp"

// GLOBAL: TH07 0x0049ea88
SoundBufferIdxVolume SOUND_BUFFER_IDX_VOL[38] = {
    {0, -2000, 0}, {0, -2500, 0}, {1, -1200, 5}, {1, -1500, 5}, {2, -1000, 100}, {3, -400, 100}, {4, -400, 100}, {5, -1500, 50}, {6, -1700, 50}, {7, -1900, 50}, {8, -1000, 100}, {9, -1000, 100}, {10, -1700, 10}, {11, -1200, 10}, {12, -900, 100}, {5, -1500, 50}, {13, -900, 50}, {14, -900, 50}, {15, -900, 100}, {16, -200, 100}, {17, -1400, 0}, {18, -1300, 0}, {5, -100, 20}, {6, -1800, 20}, {7, -1800, 20}, {19, -800, 50}, {20, -1000, 50}, {21, -1300, 50}, {22, -300, 140}, {23, -900, 100}, {24, -900, 20}, {25, -500, 90}, {26, -300, 100}, {27, -300, 100}, {24, -300, 20}, {19, 0, 50}, {28, -300, 100}, {29, -300, 100}};

// GLOBAL: TH07 0x0049ebb8
const char *g_SFXList[30] = {
    // STRING: TH07 0x00496344
    "data/wav/se_plst00.wav",
    // STRING: TH07 0x0049632c
    "data/wav/se_enep00.wav",
    // STRING: TH07 0x00496310
    "data/wav/se_pldead00.wav",
    // STRING: TH07 0x004962f8
    "data/wav/se_power0.wav",
    // STRING: TH07 0x004962e0
    "data/wav/se_power1.wav",
    // STRING: TH07 0x004962c8
    "data/wav/se_tan00.wav",
    // STRING: TH07 0x004962b0
    "data/wav/se_tan01.wav",
    // STRING: TH07 0x00496298
    "data/wav/se_tan02.wav",
    // STRING: TH07 0x00496280
    "data/wav/se_ok00.wav",
    // STRING: TH07 0x00496264
    "data/wav/se_cancel00.wav",
    // STRING: TH07 0x00496248
    "data/wav/se_select00.wav",
    // STRING: TH07 0x00496230
    "data/wav/se_gun00.wav",
    // STRING: TH07 0x00496218
    "data/wav/se_cat00.wav",
    // STRING: TH07 0x00496200
    "data/wav/se_lazer00.wav",
    // STRING: TH07 0x004961e8
    "data/wav/se_lazer01.wav",
    // STRING: TH07 0x004961d0
    "data/wav/se_enep01.wav",
    // STRING: TH07 0x004961b8
    "data/wav/se_nep00.wav",
    // STRING: TH07 0x0049619c
    "data/wav/se_damage00.wav",
    // STRING: TH07 0x00496184
    "data/wav/se_item00.wav",
    // STRING: TH07 0x0049616c
    "data/wav/se_kira00.wav",
    // STRING: TH07 0x00496154
    "data/wav/se_kira01.wav",
    // STRING: TH07 0x0049613c
    "data/wav/se_kira02.wav",
    // STRING: TH07 0x00496124
    "data/wav/se_extend.wav",
    // STRING: TH07 0x0049610c
    "data/wav/se_timeout.wav",
    // STRING: TH07 0x004960f4
    "data/wav/se_graze.wav",
    // STRING: TH07 0x004960dc
    "data/wav/se_powerup.wav",
    // STRING: TH07 0x004960c4
    "data/wav/se_border.wav",
    // STRING: TH07 0x004960ac
    "data/wav/se_bonus.wav",
    // STRING: TH07 0x00496094
    "data/wav/se_bonus2.wav",
    // STRING: TH07 0x0049607c
    "data/wav/se_pause.wav",
};

// How many times one sound effect may overlap itself. What matters in play is
// a few shots landing in the same frame; this bounds the work the audio
// callback does as well.
#define SOUND_VOICES_PER_EFFECT 8

// GLOBAL: TH07 0x004ba0d8
SoundPlayer g_SoundPlayer;

//-----------------------------------------------------------------------------
// Name: SoundPlayer::InitializeDSound()
// Desc: Brings SDL audio up and creates the CSoundManager. The name is the
//       original's and is part of the game's interface; DirectSound itself is
//       gone, the manager now just owns the SDL audio subsystem.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0044b560
ZunResult SoundPlayer::InitializeDSound(HWND gameWindow)
{
    if (!SDL_WasInit(SDL_INIT_AUDIO) && !SDL_InitSubSystem(SDL_INIT_AUDIO))
    {
        g_GameErrorContext.Log(TH_ERR_DSOUND_INIT_FAIL);
        return ZUN_ERROR;
    }

    memset(this, 0, sizeof(SoundPlayer));
    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->unusedSoundVolRelated); i++)
    {
        this->unusedSoundVolRelated[i] = -1;
    }
    this->gameWindow = gameWindow;
    this->manager = ZUN_NEW(CSoundManager, "");
    if (!this->manager ||
        FAILED(this->manager->Initialize(gameWindow, 2, 2, 44100, 16)))
    {
        g_GameErrorContext.Log(TH_ERR_DSOUND_INIT_FAIL);
        if (this->manager)
        {
            ZUN_DELETE(this->manager);
        }
        // No audio device. Everything below already copes with a missing
        // manager, so the game runs (silently) instead of refusing to start.
        return ZUN_ERROR;
    }

    this->backgroundMusicThreadHandle = NULL;
    g_GameErrorContext.Log(TH_LOG_DSOUND_INIT_SUCCESS);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044b830
ZunResult SoundPlayer::Release()
{
    i32 i;

    if (!this->manager)
    {
        return ZUN_SUCCESS;
    }

    for (i = 0; i < ARRAY_SIZE_SIGNED(this->sounds); i++)
    {
        SAFE_DELETE(this->sounds[i]);
        SAFE_DELETE(this->duplicateSoundBuffers[i]);
    }
    if (this->backgroundMusic)
    {
        ZUN_DELETE(this->backgroundMusic);
    }
    StopBGM();
    KillTimer(this->gameWindow, 1);
    if (this->manager)
    {
        ZUN_DELETE(this->manager);
    }
    for (i = 0; i < ARRAY_SIZE_SIGNED(this->bgmPreloadData); i++)
    {
        SAFE_FREE(this->bgmPreloadData[i]);
        this->bgmPreloadDataCursor[i] = NULL;
        this->bgmPreloadFmtData[i] = NULL;
    }
    if (this->bgmFmtData)
    {
        ZUN_FREE(this->bgmFmtData);
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044ba80
WAVEFORMATEX *SoundPlayer::GetWavFormatData(u8 *soundData,
                                            const char *formatString,
                                            i32 *formatSize,
                                            u32 fileSizeExcludingFormat)
{
    while (fileSizeExcludingFormat > 0)
    {
        *formatSize = *(i32 *)(soundData + 4);
        if (strncmp((char *)soundData, formatString, 4) == 0)
        {
            return (WAVEFORMATEX *)(soundData + 8);
        }
        fileSizeExcludingFormat = fileSizeExcludingFormat - (*formatSize + 8);
        soundData += +*formatSize + 8;
    }
    return NULL;
}

// FUNCTION: TH07 0x0044baf0
i32 SoundPlayer::GetFmtIndexByName(const char *name)
{
    char nameBuf[128];
    i32 fmtIdx;
    const char *slashPos;

    if (!this->bgmFmtData)
    {
        return 0;
    }

    fmtIdx = 0;
    slashPos = strrchr(name, '/');
    if (!slashPos)
    {
        slashPos = strrchr(name, '\\');
    }
    if (!slashPos)
    {
        strncpy(nameBuf, name, sizeof(nameBuf) - 1);
        nameBuf[sizeof(nameBuf) - 1] = '\0';
    }
    else
    {
        strncpy(nameBuf, slashPos + 1, sizeof(nameBuf) - 1);
        nameBuf[sizeof(nameBuf) - 1] = '\0';
    }
    while (this->bgmFmtData[fmtIdx].name[0] != '\0')
    {
        if (strcmp(this->bgmFmtData[fmtIdx].name, nameBuf) == 0)
        {
            break;
        }
        fmtIdx++;
    }
    if (this->bgmFmtData[fmtIdx].name[0] == '\0')
    {
        fmtIdx = 0;
    }
    return fmtIdx;
}

//-----------------------------------------------------------------------------
// Name: SoundPlayer::LoadSound()
// Desc: Decodes one WAV out of th07.dat into a CSound. The RIFF walk is the
//       original's: "fmt " gives the WAVEFORMATEX and "data" the PCM, which is
//       then handed to SDL as a stream in exactly that format.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0044bd00
ZunResult SoundPlayer::LoadSound(i32 idx, const char *path)
{
    u8 *soundFileDat;
    WAVEFORMATEX wavData;
    WAVEFORMATEX *wavDataPtr;
    WAVEFORMATEX *audioPtr;
    u8 *dataChunk;
    i32 formatSize;
    u8 *cursor;
    i32 fileSize;
    i32 dataSize;

    if (!this->manager)
    {
        return ZUN_SUCCESS;
    }
    if (idx < 0 || idx >= ARRAY_SIZE_SIGNED(this->sounds))
    {
        return ZUN_ERROR;
    }

    SAFE_DELETE(this->sounds[idx]);
    soundFileDat = FileSystem::OpenFile(path, 0);
    cursor = soundFileDat;
    if (!cursor)
    {
        return ZUN_ERROR;
    }

    // STRING: TH07 0x0049601c
    if (strncmp((char *)cursor, "RIFF", 4) != 0)
    {
        g_GameErrorContext.Log(TH_ERR_NOT_A_WAV, path);
        ZUN_FREE(soundFileDat);
        return ZUN_ERROR;
    }

    cursor += 4;
    fileSize = *(i32 *)cursor;
    cursor += 4;

    // STRING: TH07 0x00495ff8
    if (strncmp((char *)cursor, "WAVE", 4) != 0)
    {
        g_GameErrorContext.Log(TH_ERR_PROBABLY_NOT_A_WAV, path);
        ZUN_FREE(soundFileDat);
        return ZUN_ERROR;
    }

    cursor += 4;
    // STRING: TH07 0x00495fd4
    wavDataPtr = GetWavFormatData(cursor, "fmt ", &formatSize, fileSize - 12);
    if (!wavDataPtr)
    {
        g_GameErrorContext.Log(TH_ERR_PROBABLY_NOT_A_WAV, path);
        ZUN_FREE(soundFileDat);
        return ZUN_ERROR;
    }

    memcpy(&wavData, wavDataPtr, sizeof(WAVEFORMATEX));
    wavData.cbSize = 0;

    // audioPtr is the header of the "data" chunk: FormatSize from
    // GetWavFormatData is the chunk's payload size, and the PCM starts 8 bytes
    // into the chunk (fourcc + size).
    audioPtr = GetWavFormatData(cursor, "data", &formatSize, fileSize - 12);
    if (!audioPtr)
    {
        g_GameErrorContext.Log(TH_ERR_PROBABLY_NOT_A_WAV, path);
        ZUN_FREE(soundFileDat);
        return ZUN_ERROR;
    }
    dataChunk = (u8 *)audioPtr - 8;
    dataSize = formatSize;
    // Bound the chunk by the RIFF end ("RIFF" + dwRiffSize) and by what was
    // actually allocated, so a truncated file cannot be read past its end.
    if (dataSize <= 0 || (DWORD)(dataChunk - soundFileDat) + 8 + (DWORD)dataSize >
                              (DWORD)fileSize + 8 ||
        (DWORD)(dataChunk - soundFileDat) + 8 + (DWORD)dataSize > g_LastFileSize)
    {
        g_GameErrorContext.Log(TH_ERR_PROBABLY_NOT_A_WAV, path);
        ZUN_FREE(soundFileDat);
        return ZUN_ERROR;
    }

    this->sounds[idx] = new CSound(dataChunk + 8, (DWORD)dataSize,
                                   SOUND_VOICES_PER_EFFECT, new CWaveFile(),
                                   &wavData);

    ZUN_FREE(soundFileDat);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044bff0
ZunResult SoundPlayer::LoadFmt(const char *path)
{
    if (this->bgmFmtData)
    {
        ZUN_FREE(this->bgmFmtData);
    }
    this->bgmFmtData = (ThBgmFormat *)FileSystem::OpenFile(path, 0);
    return this->bgmFmtData != NULL ? ZUN_SUCCESS : ZUN_ERROR;
}

//-----------------------------------------------------------------------------
// Name: SoundPlayer::StartBGM()
// Desc: Opens the ZWAV archive for streaming and starts the BGM thread.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0044c020
ZunResult SoundPlayer::StartBGM(const char *path)
{
    ThBgmFormat *pzwf;
    DWORD blockAlign;
    DWORD samplesPerSec;
    DWORD notifySize;

    if (!this->manager)
    {
        return ZUN_ERROR;
    }

    strcpy(this->bgmArchivePath, path);

    // STRING: TH07 0x00495fb4
    utils::DebugPrint("Streming BGM Start\r\n");
    StopBGM();
    if (!this->bgmFmtData)
    {
        return ZUN_ERROR;
    }
    pzwf = this->bgmFmtData;
    blockAlign = pzwf->format.nBlockAlign;
    samplesPerSec = pzwf->format.nSamplesPerSec;
    if (blockAlign == 0 || samplesPerSec == 0)
    {
        return ZUN_ERROR;
    }
    notifySize = samplesPerSec * 4 * blockAlign >> 4;
    notifySize -= notifySize % blockAlign;
    this->backgroundMusicUpdateEvent = CreateEventA(NULL, 0, 0, NULL);
    this->backgroundMusicThreadHandle = CreateThread(
        NULL, 0, BackgroundMusicPlayerThread, g_Supervisor.hwndGameWindow, 0,
        &this->backgroundMusicThreadId);
    if (FAILED(this->manager->CreateStreaming(
            &this->backgroundMusic, path,
            SDLSND_PLAY_ONCE, GUID_NULL, 16, notifySize, pzwf)))
    {
        utils::DebugPrint(TH_ERR_STREAMING_SOUND_BUFFER_CREATE_FAIL);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044c1b0
ZunResult SoundPlayer::ReopenBGM(const char *name)
{
    i32 fmtIdx;

    if (!this->backgroundMusic || !this->bgmFmtData)
    {
        return ZUN_ERROR;
    }

    fmtIdx = GetFmtIndexByName(name);

    this->backgroundMusic->GetWaveFile()->Reopen(&this->bgmFmtData[fmtIdx]);
    // The new track may run at a different sample rate; reopen the stream with
    // that track's format and drop whatever the previous one left queued.
    this->backgroundMusic->ReopenStream(&this->bgmFmtData[fmtIdx].format);
    this->backgroundMusic->Reset();
    // STRING: TH07 0x00495f54
    utils::DebugPrint("Streming BGM Reopen %d\r\n", fmtIdx);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044c220
ZunResult SoundPlayer::PreloadBGM(i32 idx, const char *path)
{
    LPBYTE lpBuffer;
    HANDLE handle;
    DWORD bytesRead;
    i32 fmtIdx;

    if (idx < 0 || idx >= ARRAY_SIZE_SIGNED(this->bgmPreloadData))
    {
        return ZUN_ERROR;
    }
    if (this->bgmPreloadData[idx])
    {
        if (strcmp(path, this->bgmFileNames[idx]) == 0)
        {
            return ZUN_SUCCESS;
        }
    }
    strcpy(g_SoundPlayer.bgmFileNames[idx], path);
    if (!g_Supervisor.cfg.preloadBgm)
    {
        return ZUN_SUCCESS;
    }

    if (!this->manager || !this->bgmFmtData)
    {
        return ZUN_SUCCESS;
    }

    // STRING: TH07 0x00495f38
    utils::DebugPrint("Streming BGM PreLoad %d\r\n", idx);
    handle = CreateFileA(this->bgmArchivePath, GENERIC_READ, FILE_SHARE_READ, NULL, 3,
                         FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        // STRING: TH07 0x00495f14
        utils::DebugPrint("error : bgmfile is not find %s\r\n", this->bgmArchivePath);
        return ZUN_ERROR;
    }

    fmtIdx = GetFmtIndexByName(path);
    SetFilePointer(handle, this->bgmFmtData[fmtIdx].startOffset, NULL, 0);
    lpBuffer = (LPBYTE)ZUN_ALLOC(this->bgmFmtData[fmtIdx].preloadAllocSize);
    if (!lpBuffer)
    {
        CloseHandle(handle);
        utils::DebugPrint("error : bgmfile is not find %s\r\n", this->bgmArchivePath);
        return ZUN_ERROR;
    }

    bytesRead = 0;
    ReadFile(handle, lpBuffer, this->bgmFmtData[fmtIdx].preloadAllocSize,
             &bytesRead, NULL);
    CloseHandle(handle);
    SAFE_FREE(this->bgmPreloadData[idx]);
    this->bgmPreloadFmtData[idx] = this->bgmFmtData + fmtIdx;
    this->bgmPreloadData[idx] = lpBuffer;
    this->bgmPreloadDataCursor[idx] = lpBuffer;
    this->bgmPreloadAllocSizes[idx] =
        this->bgmPreloadFmtData[idx]->preloadAllocSize;
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044c4d0
ZunResult SoundPlayer::LoadBGM(i32 idx)
{
    ThBgmFormat *preloadFmt;
    DWORD blockAlign;
    DWORD samplesPerSec;
    DWORD notifySize;

    if (!this->manager)
    {
        return ZUN_ERROR;
    }

    if (g_Supervisor.cfg.musicMode == MUSIC_OFF)
    {
        return ZUN_ERROR;
    }

    if (idx < 0 || idx >= ARRAY_SIZE_SIGNED(this->bgmPreloadData))
    {
        return ZUN_ERROR;
    }

    if (!g_Supervisor.cfg.preloadBgm)
    {
        return ReopenBGM(this->bgmFileNames[idx]);
    }

    if (!this->bgmPreloadData[idx])
    {
        return ZUN_ERROR;
    }

    // STRING: TH07 0x00495ef8
    utils::DebugPrint("Streming BGM Load no %d\r\n", idx);
    preloadFmt = this->bgmPreloadFmtData[idx];
    blockAlign = preloadFmt->format.nBlockAlign;
    samplesPerSec = preloadFmt->format.nSamplesPerSec;
    if (blockAlign == 0 || samplesPerSec == 0)
    {
        return ZUN_ERROR;
    }
    notifySize = samplesPerSec * 4 * blockAlign >> 4;
    notifySize -= notifySize % blockAlign;

    // The previous track's stream has to go before a new one is opened.
    if (this->backgroundMusic)
    {
        ZUN_DELETE(this->backgroundMusic);
    }

    this->backgroundMusicUpdateEvent = CreateEventA(NULL, 0, 0, NULL);
    this->backgroundMusicThreadHandle = CreateThread(
        NULL, 0, BackgroundMusicPlayerThread, g_Supervisor.hwndGameWindow, 0,
        &this->backgroundMusicThreadId);
    if (FAILED(this->manager->CreateStreamingFromMemory(
            &this->backgroundMusic, this->bgmPreloadDataCursor[idx],
            this->bgmPreloadAllocSizes[idx], this->bgmPreloadFmtData[idx],
            SDLSND_PLAY_ONCE, GUID_NULL, 16, notifySize)))
    {
        utils::DebugPrint(TH_ERR_STREAMING_SOUND_BUFFER_CREATE_FAIL);
        return ZUN_ERROR;
    }

    // STRING: TH07 0x00495eec
    utils::DebugPrint("load comp\r\n");
    this->curBgmIdx = idx;
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044c6b0
void SoundPlayer::StopBGM()
{
    if (this->backgroundMusic)
    {
        // STRING: TH07 0x00495ed8
        utils::DebugPrint("Streming BGM stop\r\n");
        this->backgroundMusic->Stop();
        if (this->backgroundMusicThreadHandle)
        {
            PostThreadMessageA(this->backgroundMusicThreadId, WM_QUIT, 0, 0);
            // STRING: TH07 0x00495ebc
            utils::DebugPrint("stop m_dwNotifyThreadID\r\n");
            while (WaitForSingleObject(this->backgroundMusicThreadHandle, 256) !=
                   0)
            {
                PostThreadMessageA(this->backgroundMusicThreadId, WM_QUIT, 0, 0);
            }
            // STRING: TH07 0x00495eb0
            utils::DebugPrint("stop comp\r\n");
            CloseHandle(this->backgroundMusicThreadHandle);
            CloseHandle(this->backgroundMusicUpdateEvent);
            this->backgroundMusicThreadHandle = NULL;
            this->backgroundMusicUpdateEvent = NULL;
        }

        if (this->backgroundMusic)
        {
            ZUN_DELETE(this->backgroundMusic);
        }
    }
}

//-----------------------------------------------------------------------------
// Name: SoundPlayer::InitSoundBuffers()
// Desc: Loads the 30 WAVs out of th07.dat and builds the per-sound-index
//       table that used to be 38 premade DirectSound duplicate buffers.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0044c7d0
ZunResult SoundPlayer::InitSoundBuffers()
{
    i32 i;

    if (!this->manager)
    {
        return ZUN_ERROR;
    }

    for (i = 0; i < ARRAY_SIZE_SIGNED(this->soundQueue); i++)
    {
        this->soundQueue[i] = -1;
    }
    for (i = 0; i < ARRAY_SIZE_SIGNED(g_SFXList); i++)
    {
        if (LoadSound(i, g_SFXList[i]) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Log(TH_ERR_SOUND_READ_FAIL, g_SFXList[i]);
            return ZUN_ERROR;
        }
    }
    for (i = 0; i < ARRAY_SIZE_SIGNED(SOUND_BUFFER_IDX_VOL); i++)
    {
        SAFE_DELETE(this->duplicateSoundBuffers[i]);
        this->duplicateSoundBuffers[i] =
            new SoundBufferIdxVolume(SOUND_BUFFER_IDX_VOL[i]);
        this->ApplySoundVolume(i);
    }
    return ZUN_SUCCESS;
}

//-----------------------------------------------------------------------------
// Name: SoundPlayer::ApplySoundVolume()
// Desc: Pushes SOUND_BUFFER_IDX_VOL[idx]'s level onto the CSound it refers to.
//-----------------------------------------------------------------------------
void SoundPlayer::ApplySoundVolume(i32 idx)
{
    SoundBufferIdxVolume *settings;
    i32 bufferIdx;

    if (idx < 0 || idx >= ARRAY_SIZE_SIGNED(this->duplicateSoundBuffers))
    {
        return;
    }

    settings = this->duplicateSoundBuffers[idx];
    if (!settings)
    {
        return;
    }

    bufferIdx = settings->bufferIdx;
    if (bufferIdx < 0 || bufferIdx >= ARRAY_SIZE_SIGNED(this->sounds))
    {
        return;
    }
    if (this->sounds[bufferIdx])
    {
        this->sounds[bufferIdx]->SetVolume((i32)settings->volume);
    }
}

// FUNCTION: TH07 0x0044c930
void SoundPlayer::PlaySoundByIdx(i32 idx, u32 unused)
{
    i32 unk;
    i32 i;

    if (idx < 0 || idx >= ARRAY_SIZE_SIGNED(SOUND_BUFFER_IDX_VOL))
    {
        return;
    }

    unk = SOUND_BUFFER_IDX_VOL[idx].field2_0x6;
    for (i = 0; i < ARRAY_SIZE_SIGNED(this->soundQueue); i++)
    {
        if (this->soundQueue[i] < 0)
        {
            break;
        }

        if (this->soundQueue[i] == idx)
        {
            return;
        }
    }
    if (i >= ARRAY_SIZE_SIGNED(this->soundQueue))
    {
        return;
    }

    this->soundQueue[i] = idx;
    this->unusedSoundVolRelated[idx] = unk;
}

// FUNCTION: TH07 0x0044c9c0
i32 SoundPlayer::ProcessQueues()
{
    SoundBufferIdxVolume *settings;
    SoundPlayerCommand *commandCursor;
    ThBgmFormat *format;
    i32 bufferIdx;
    char (*name)[256];
    i32 curSound;
    i32 fmtIdx;
    i32 i;
    u32 loopAgain;

    if (!this->manager)
    {
        return 0;
    }

    commandCursor = this->commandQueue;
loop:
    loopAgain = false;
    switch (commandCursor->opcode)
    {
    case AUDIO_PRELOAD:
        if (g_Supervisor.cfg.preloadBgm)
        {
            // STRING: TH07 0x00495e60
            utils::DebugPrint("Sound : PreLoad Stage\r\n");
            if (!commandCursor->arg2)
            {
                StopBGM();
                PreloadBGM(commandCursor->arg1, commandCursor->string);
                loopAgain = true;
                break;
            }
        }
        else
        {
            utils::DebugPrint("Sound : PreLoad Stage\r\n");
            PreloadBGM(commandCursor->arg1, commandCursor->string);
            loopAgain = true;
            break;
        }
        commandCursor->arg2++;
        goto loop_breakout;
    case AUDIO_START:
        if (g_Supervisor.cfg.preloadBgm && commandCursor->arg1 >= 0)
        {
            if (!commandCursor->arg2)
            {
                // STRING: TH07 0x00495e48
                utils::DebugPrint("Sound : Load Stage\r\n");
                if (LoadBGM(commandCursor->arg1) != ZUN_SUCCESS)
                {
                    break;
                }
            }
            else if (commandCursor->arg2 == 2)
            {
                // STRING: TH07 0x00495e30
                utils::DebugPrint("Sound : Reset Stage\r\n");
                if (this->backgroundMusic)
                {
                    if (FAILED(this->backgroundMusic->Reset()))
                    {
                        break;
                    }
                }
            }
            else if (commandCursor->arg2 == 5)
            {
                // The DirectSound version prefilled the ring buffer here. SDL
                // streams pull their data on demand (the BGM thread's job) and
                // the track length is what decides whether the BGM loops.
                utils::DebugPrint("Sound : Fill Buffer Stage\r\n");
                format = this->backgroundMusic
                             ? this->backgroundMusic->GetWaveFile()->GetFormat()
                             : NULL;
                commandCursor->arg1 = format && format->totalLength != 0;
            }
            else if (commandCursor->arg2 == 7)
            {
                utils::DebugPrint("Sound : Play Stage\r\n");
                if (this->backgroundMusic)
                {
                    this->backgroundMusic->Play(0, SDLSND_PLAY_LOOPING);
                }
            }
            else if (commandCursor->arg2 >= 20)
            {
                break;
            }
        }
        else if (!this->backgroundMusic)
        {
            break;
        }
        else if (!commandCursor->arg2)
        {
            // STRING: TH07 0x00495de4
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->backgroundMusic->Stop();
        }
        else if (commandCursor->arg2 == 1)
        {
            if (this->backgroundMusic->m_bIsLocked)
            {
                goto loop_breakout;
            }
            // STRING: TH07 0x00495dc8
            utils::DebugPrint("Sound : Recreate Stage\r\n");
            this->backgroundMusic->InitSoundBuffers();
        }
        else if (commandCursor->arg2 == 2)
        {
            // STRING: TH07 0x00495db0
            utils::DebugPrint("Sound : ReOpen Stage\r\n");
            name = commandCursor->arg1 >= 0
                       ? &this->bgmFileNames[commandCursor->arg1]
                       : &commandCursor->string;
            ReopenBGM(*name);
        }
        else if (commandCursor->arg2 == 3)
        {
            // STRING: TH07 0x00495e14
            utils::DebugPrint("Sound : Fill Buffer Stage\r\n");
            this->backgroundMusic->Reset();
            if (!this->backgroundMusic->GetWaveFile()->GetFormat())
            {
                break;
            }
            commandCursor->arg1 = this->backgroundMusic->GetWaveFile()
                                      ->GetFormat()
                                      ->totalLength != 0;
        }
        else if (commandCursor->arg2 == 4)
        {
            // STRING: TH07 0x00495dfc
            utils::DebugPrint("Sound : Play Stage\r\n");
            this->backgroundMusic->Play(0, SDLSND_PLAY_LOOPING);
        }
        else if (commandCursor->arg2 >= 7)
        {
            break;
        }
        commandCursor->arg2++;
        goto loop_breakout;
    case AUDIO_SHUTDOWN:
        if (!this->backgroundMusic)
        {
            break;
        }

        if (!commandCursor->arg2)
        {
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->backgroundMusic->Stop();
        }
        else if (commandCursor->arg2 == 1)
        {
            // STRING: TH07 0x00495d94
            utils::DebugPrint("Sound : Thread Stop Stage\r\n");
            if (!this->backgroundMusicThreadHandle)
            {
                break;
            }
            PostThreadMessageA(this->backgroundMusicThreadId, WM_QUIT, 0, 0);
        }
        else if (commandCursor->arg2 == 2)
        {
            if (this->backgroundMusicThreadHandle &&
                WaitForSingleObject(this->backgroundMusicThreadHandle, 256))
            {
                // STRING: TH07 0x00495d70
                utils::DebugPrint("Sound : Thread Stop Wait Stage\r\n");
                PostThreadMessageA(this->backgroundMusicThreadId, WM_QUIT, 0, 0);
                commandCursor->arg2--;
            }
            else
            {
                this->backgroundMusicThreadHandle = NULL;
            }
        }
        else if (commandCursor->arg2 == 3)
        {
            // STRING: TH07 0x00495d50
            utils::DebugPrint("Sound : Handle Close Stage\r\n");
            if (this->backgroundMusicThreadHandle)
            {
                CloseHandle(this->backgroundMusicThreadHandle);
                this->backgroundMusicThreadHandle = NULL;
            }
            if (this->backgroundMusicUpdateEvent)
            {
                CloseHandle(this->backgroundMusicUpdateEvent);
                this->backgroundMusicUpdateEvent = NULL;
            }

            if (this->backgroundMusic)
            {
                ZUN_DELETE(this->backgroundMusic);
            }
        }
        else if (commandCursor->arg2 == 10)
        {
            break;
        }
        commandCursor->arg2++;
        goto loop_breakout;
    case AUDIO_STOP:
        if (!this->backgroundMusic)
        {
            break;
        }

        if (!commandCursor->arg2)
        {
            utils::DebugPrint("Sound : Stop Stage\r\n");
            this->backgroundMusic->Stop();
        }
        else if (commandCursor->arg2 == 1)
        {
            break;
        }
        commandCursor->arg2++;
        goto loop_breakout;
    case AUDIO_FADEOUT: {
        // STRING: TH07 0x00495d34
        utils::DebugPrint("Sound : Fade Out Stage %d\r\n", commandCursor->arg1);
        g_SoundPlayer.FadeOut((f32)commandCursor->arg1);
        break;
    }
    case AUDIO_PAUSE:
        if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
        {
            if (!this->backgroundMusic)
            {
                break;
            }
            if (this->backgroundMusic->m_bIsLocked)
            {
                // STRING: TH07 0x00495d2c
                utils::DebugPrint("locked\n");
                goto loop_breakout;
            }
            this->backgroundMusic->Pause();
        }
        break;
    case AUDIO_UNPAUSE:
        if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
        {
            if (!this->backgroundMusic)
            {
                break;
            }
            if (this->backgroundMusic->m_bIsLocked)
            {
                goto loop_breakout;
            }
            this->backgroundMusic->Unpause();
        }
        break;
    default:
        goto loop_breakout;
    }
    for (i = 0; i < MAX_SOUND_COMMANDS; i++, commandCursor++)
    {
        if (commandCursor->opcode == 0)
        {
            break;
        }
        commandCursor[0] = commandCursor[1];
    }

    if (loopAgain)
    {
        goto loop;
    }

loop_breakout:
    if (this->backgroundMusic && this->backgroundMusic->m_dwIsFadingOut)
    {
        this->backgroundMusic->UpdateFadeOut();
    }

    if (!g_Supervisor.cfg.playSounds)
    {
        return this->commandQueue[0].opcode;
    }

    // Start the queued effects. SDL mixes them: each one owns a stream, so a
    // sound that is still playing is not restarted or cut off (the old
    // duplicate-buffer scheme stopped and rewound a shared buffer instead).
    for (i = 0; i < ARRAY_SIZE_SIGNED(this->soundQueue); i++)
    {
        if (this->soundQueue[i] < 0)
        {
            break;
        }

        curSound = this->soundQueue[i];
        this->soundQueue[i] = -1;
        settings = this->duplicateSoundBuffers[curSound];
        if (!settings)
        {
            continue;
        }

        bufferIdx = settings->bufferIdx;
        if (bufferIdx < 0 || bufferIdx >= ARRAY_SIZE_SIGNED(this->sounds) ||
            !this->sounds[bufferIdx])
        {
            continue;
        }

        this->ApplySoundVolume(curSound);
        this->sounds[bufferIdx]->Play(0, SDLSND_PLAY_ONCE);
    }
    return this->commandQueue[0].opcode;
}

//-----------------------------------------------------------------------------
// Name: SoundPlayer::BackgroundMusicPlayerThread()
// Desc: Feeds the BGM stream from the archive. This is the original's
//       DirectSound notification thread, retargeted at SDL: instead of waking
//       on position notifications it wakes every tick, pushes the next chunk,
//       and still receives the WM_QUIT that stops it.
//-----------------------------------------------------------------------------
// FUNCTION: TH07 0x0044d200
DWORD __stdcall SoundPlayer::BackgroundMusicPlayerThread(LPVOID lpThreadParameter)
{
    ZunBool stopped;
    ZunBool looped;
    DWORD waitObj;
    LPVOID lpThreadParameterCopy;
    MSG msg;
    HRESULT hr;

    lpThreadParameterCopy = lpThreadParameter;
    stopped = FALSE;
    looped = TRUE;
    while (!stopped)
    {
        waitObj = MsgWaitForMultipleObjects(
            1, &g_SoundPlayer.backgroundMusicUpdateEvent, 0, 0x10,
            QS_HOTKEY | QS_PAINT | QS_TIMER | QS_POSTMESSAGE |
                QS_MOUSEBUTTON | QS_MOUSEMOVE | QS_KEY);
        if (!g_SoundPlayer.backgroundMusic)
        {
            stopped = TRUE;
        }

        switch (waitObj)
        {
        case 0:
            break;
        case 1:
            while (PeekMessageA(&msg, NULL, 0, 0, 1))
            {
                if (msg.message == WM_QUIT)
                {
                    stopped = TRUE;
                }
            }
            break;
        default:
            // Timeout, or the posted message is not a thread message yet.
            break;
        }

        // The caller polls this flag around the buffer work, exactly like the
        // original guarded its notification handler.
        if (g_SoundPlayer.backgroundMusic)
        {
            g_SoundPlayer.backgroundMusic->m_bIsLocked = TRUE;
            if (g_SoundPlayer.backgroundMusic->m_bIsPlaying)
            {
                hr = g_SoundPlayer.backgroundMusic->HandleWaveStreamNotification(looped);
                (void)hr;
            }
        }
        if (g_SoundPlayer.backgroundMusic)
        {
            g_SoundPlayer.backgroundMusic->m_bIsLocked = FALSE;
        }
    }
    utils::DebugPrint(TH_LOG_BGM_THREAD_EXIT);
    return 0;
}

// FUNCTION: TH07 0x0044d2f0
void SoundPlayer::PushCommand(AudioOpcode opcode, i32 arg1, const char *arg2)
{
    i32 i;

    for (i = 0; i < MAX_SOUND_COMMANDS; i++)
    {
        if (this->commandQueue[i].opcode != 0)
        {
            continue;
        }

        this->commandQueue[i].opcode = opcode;
        this->commandQueue[i].arg1 = arg1;
        strncpy(this->commandQueue[i].string, arg2,
                sizeof(this->commandQueue[i].string) - 1);
        this->commandQueue[i].string[sizeof(this->commandQueue[i].string) - 1] = '\0';
        this->commandQueue[i].arg2 = 0;

        break;
    }
    // STRING: TH07 0x00495ce0
    utils::DebugPrint("Sound Que Add %d\r\n", opcode);
}
