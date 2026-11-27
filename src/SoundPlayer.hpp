#pragma once

#include <SDL3/SDL.h>

#include "ZunResult.hpp"
#include "dsutil.hpp"
#include "inttypes.hpp"

enum AudioOpcode
{
    AUDIO_PRELOAD = 1,
    AUDIO_START = 2,
    AUDIO_STOP = 3,
    AUDIO_SHUTDOWN = 4,
    AUDIO_FADEOUT = 5,
    AUDIO_PAUSE = 6,
    AUDIO_UNPAUSE = 7
};

enum SoundIdx
{
    SOUND_SHOOTING = 0,
    SOUND_PICHUN = 4,
    SOUND_BOMB_SAKUYA_A = 5,
    SOUND_BOMB_REIMARI = 6,
    SOUND_BOMB_MARISA_A_FOCUS = 7,
    SOUND_SELECT = 10,
    SOUND_BACK = 11,
    SOUND_MOVE_MENU = 12,
    SOUND_BOMB_REIMU_A = 13,
    SOUND_BOMB = 14,
    SOUND_ENEMY_SPELLCARD_END = 15,
    SOUND_BOMB_SAKUMARI = 19,
    SOUND_ENEMY_DAMAGED = 20,
    SOUND_ITEM_GET = 21,
    SOUND_DIR_CHANGING = 25,
    SOUND_EXTEND = 28,
    SOUND_LOW_SPELL_TIME_WARNING = 29,
    SOUND_GRAZE = 30,
    SOUND_POWERUP = 31,
    SOUND_BORDER_ACTIVATE = 32,
    SOUND_BORDER_BREAK = 33,
    SOUND_BORDER_ACTIVATE2 = 36,
    SOUND_PAUSED = 37
};

struct SoundBufferIdxVolume
{
    i32 bufferIdx;
    i16 volume;
    i16 field2_0x6;
};

struct SoundPlayerCommand
{
    i32 opcode;
    i32 arg1;
    i32 arg2;
    char string[256];
};
C_ASSERT(sizeof(SoundPlayerCommand) == 0x10c);

#define MAX_SOUND_COMMANDS 31

// The audio front end the game talks to. This struct used to mirror the
// original's memory layout down to the last DirectSound pointer; with the
// playback layer moved to SDL3 the layout no longer matters, only the member
// names (Supervisor.cpp still sets bgmArchivePath before PlayLoadedAudio).
struct SoundPlayer
{
    SoundPlayer()
    {
        memset(this, 0, sizeof(SoundPlayer));
        for (i32 i = 0; i < 128; i++)
        {
            this->unusedSoundVolRelated[i] = -1;
        }
    }

    static DWORD __stdcall BackgroundMusicPlayerThread(LPVOID lpThreadParameter);
    void ApplySoundVolume(i32 idx);
    i32 GetFmtIndexByName(const char *name);
    static WAVEFORMATEX *GetWavFormatData(u8 *soundData, const char *formatString,
                                          i32 *formatSize, u32 fileSizeExcludingFormat);
    ZunResult InitializeDSound(HWND gameWindow);
    ZunResult InitSoundBuffers();
    ZunResult LoadBGM(i32 idx);
    ZunResult LoadFmt(const char *path);
    ZunResult LoadSound(i32 idx, const char *path);
    void PlaySoundByIdx(i32 idx, u32 unused);
    ZunResult PreloadBGM(i32 idx, const char *path);
    i32 ProcessQueues();
    void PushCommand(AudioOpcode opcode, i32 arg1, const char *arg2);
    ZunResult Release();
    ZunResult ReopenBGM(const char *name);
    ZunResult StartBGM(const char *path);
    void StopBGM();

    void FadeOut(f32 duration)
    {
        if (this->backgroundMusic)
        {
            this->backgroundMusic->FadeOut(duration);
        }
    }

    // One CSound per entry of g_SFXList (see SoundPlayer.cpp), each holding the
    // decoded WAV and its own SDL audio stream.
    CSound *sounds[128];
    // Per game-sound settings from SOUND_BUFFER_IDX_VOL: which g_SFXList entry
    // to play and how loud. Replaces the array of pre-configured DirectSound
    // duplicate buffers; the volume is applied to the CSound instead.
    SoundBufferIdxVolume *duplicateSoundBuffers[128];
    i32 unusedSoundVolRelated[128];
    // The BGM stream runs on its own thread, exactly like the original's
    // DirectSound notification thread; the event releases it once per tick.
    HANDLE backgroundMusicUpdateEvent;
    HWND gameWindow;
    CSoundManager *manager;
    DWORD backgroundMusicThreadId; // ZUN name: m_dwNotifyThreadID
    HANDLE backgroundMusicThreadHandle;
    u32 unused_61c;
    i32 soundQueue[5];
    ThBgmFormat *bgmPreloadFmtData[16];
    LPBYTE bgmPreloadData[16];
    LPBYTE bgmPreloadDataCursor[16];
    DWORD bgmPreloadAllocSizes[16];
    i32 curBgmIdx;
    ThBgmFormat *bgmFmtData;
    SoundPlayerCommand commandQueue[MAX_SOUND_COMMANDS + 1];
    char bgmFileNames[16][256];
    char bgmArchivePath[256];
    CStreamingSound *backgroundMusic;
    i32 unused_39c4;
    i32 bgmSeekOffset;
};
extern SoundPlayer g_SoundPlayer;
