#include "Supervisor.hpp"

#include <SDL3/SDL.h>
#include <stdio.h>

#include "AnmManager.hpp"
#include "AsciiManager.hpp"
#include "Ending.hpp"
#include "GameManager.hpp"
#include "Global.hpp"
#include "MainMenu.hpp"
#include "MidiOutput.hpp"
#include "MusicRoom.hpp"
#include "ResultScreen.hpp"
#include "SoundPlayer.hpp"
#include "TextHelper.hpp"
#include "ZunResult.hpp"
#include "dxutil.hpp"
#include "i18n.hpp"
#include "pbg4/Pbg4Archive.hpp"

// GLOBAL: TH07 0x0049ee40
ControllerMapping g_ControllerMapping = {0, 1, 2, 4, -1, -1, -1, -1, 3};

// GLOBAL: TH07 0x004b9e4c
u16 g_CurFrameRawInput;

// GLOBAL: TH07 0x004b9e50
u16 g_CurFrameGameInput;

// GLOBAL: TH07 0x004b9e54
u16 g_LastFrameRawInput;

// GLOBAL: TH07 0x004b9e58
u16 g_LastFrameGameInput;

// GLOBAL: TH07 0x004b9e5c
u16 g_IsEighthFrameOfHeldInput;

// GLOBAL: TH07 0x004b9e60
u16 g_NumOfFramesInputsWereHeld;

// The gamepad counterpart of the shot-slow focus timer in Global.cpp. That one is private
// to the joystick code in Controller::GetControllerInput, so the SDL path keeps its own;
// the arithmetic below is copied from it unchanged so the auto-focus timing is identical.
static u16 g_GamepadAutoFocusTimer;

// GLOBAL: TH07 0x00575950
Supervisor g_Supervisor;

// GLOBAL: TH07 0x0135dfec
u32 g_FpsUpdateCounter;

// GLOBAL: TH07 0x0135dff0
char g_ReplayFpsBuffer[256];

// GLOBAL: TH07 0x0135e0f0
char g_FpsCounterBuffer[256];

// GLOBAL: TH07 0x0135e1f0
u32 g_NumFramesSinceLastTime;

// GLOBAL: TH07 0x0135e298
LARGE_INTEGER g_PerformanceCounter;

// FUNCTION: TH07 0x00437908
void Supervisor::CheckTiming()
{
    f64 timeDiff;
    f64 perfDiff;

    if (!this->checkTiming)
    {
        return;
    }

    QueryPerformanceCounter(&this->curPerfCounter);
    GetLocalTime(&this->curTime);

    timeDiff = (f64)this->curTime.wDay * 24.0 * 60.0 * 60.0 +
               (f64)(this->curTime.wHour * 60 * 60) +
               (f64)(this->curTime.wMinute * 60) +
               (f64)this->curTime.wSecond;

    perfDiff = (f64)this->prevTime.wDay * 24.0 * 60.0 * 60.0 +
               (f64)(this->prevTime.wHour * 60 * 60) +
               (f64)(this->prevTime.wMinute * 60) +
               (f64)this->prevTime.wSecond;

    if (timeDiff < perfDiff)
    {
        timeDiff = (f64)(this->prevTime.wDay + 1) * 24.0 * 60.0 * 60.0 +
                   (f64)(this->curTime.wHour * 60 * 60) +
                   (f64)(this->curTime.wMinute * 60) +
                   (f64)this->curTime.wSecond;
    }

    timeDiff -= perfDiff;
    timeDiff = timeDiff * 1000.0 + (f64)this->curTime.wMilliseconds -
               (f64)this->prevTime.wMilliseconds;
    timeDiff /= 1000.0;

    perfDiff = (f64)(this->curPerfCounter.LowPart - this->prevPerfCounter.LowPart) /
               (f64)this->perfFrequency.LowPart;

    if (perfDiff >= 1.0)
    {
        if (timeDiff / perfDiff > 2.5)
        {
            this->timingErrorCount++;
            if (this->maxTimingError < this->timingErrorCount)
            {
                this->maxTimingError = this->timingErrorCount;
            }
            if (this->timingSpikeAccumulator < this->timingErrorCount)
            {
                this->timingSpikeAccumulator = this->timingErrorCount;
            }
            if (this->timingSpikeAccumulator >= 10)
            {
                this->timingBadCount++;
                this->timingSpikeAccumulator = 0;
            }
            utils::DebugPrint2(TH_LOG_ALQ_CHECK, timeDiff, perfDiff,
                               timeDiff / perfDiff);
        }
        else if (this->timingErrorCount != 0)
        {
            this->timingErrorCount--;
        }
        this->checkTiming = FALSE;
    }

    if (this->maxTimingError >= 40 || this->timingBadCount >= 16)
    {
        this->timingBad = 1;
    }
    else
    {
        this->timingBad = 0;
    }
}

// FUNCTION: TH07 0x00437c39
void AnmManager::ReleaseVertexBuffer()
{
    // vertexBuffer is no longer a D3D vertex buffer with a Release() to call: the
    // renderer owns its own GPU buffers now and the field is plain storage.
    this->vertexBuffer = NULL;
}

// FUNCTION: TH07 0x00437c70
u32 Supervisor::OnUpdate(Supervisor *arg)
{
    g_AnmManager->SetVertexShader(255);
    g_AnmManager->SetSprite(NULL);
    g_AnmManager->SetTexture(NULL);
    g_AnmManager->SetColorOp(255);
    g_AnmManager->SetBlendMode(255);
    g_AnmManager->SetZWriteDisable(255);
    g_AnmManager->ClearFrameState();
    g_AnmManager->SetCameraMode(255);
    g_AnmManager->SetColor(0x80808080);
    g_AnmManager->offset.y = 0.0f;
    g_AnmManager->offset.x = 0.0f;
    g_Supervisor.fogEnabled = 255;
    if (g_SoundPlayer.backgroundMusic)
    {
        g_SoundPlayer.backgroundMusic->UpdateFadeOut();
    }
    if (!g_GameManager.slowModeSlowActive)
    {
        g_LastFrameRawInput = g_CurFrameRawInput;
        g_CurFrameRawInput = Supervisor::GetInputFrame();
        g_IsEighthFrameOfHeldInput = 0;
        if (g_LastFrameRawInput == g_CurFrameRawInput)
        {
            if (g_NumOfFramesInputsWereHeld >= 30)
            {
                if (g_NumOfFramesInputsWereHeld % 8 == 0)
                {
                    g_IsEighthFrameOfHeldInput = 1;
                }
                if (g_NumOfFramesInputsWereHeld >= 38)
                {
                    g_NumOfFramesInputsWereHeld = 30;
                }
            }
            g_NumOfFramesInputsWereHeld++;
        }
        else
        {
            g_NumOfFramesInputsWereHeld = 0;
        }
    }
    else
    {
        g_CurFrameRawInput |= Supervisor::GetInputFrame();
    }
    if (arg->wantedState != arg->curState)
    {
        arg->prevState = arg->wantedState;
        utils::DebugPrint2("scene %d -> %d\r\n", arg->wantedState,
                           arg->curState);
        switch (arg->wantedState)
        {
        case SUPERVISOR_STATE_INIT:
        CASE_SUPERVISOR_STATE_INIT:
            arg->curState = SUPERVISOR_STATE_MAINMENU;
                    if (MainMenu::RegisterChain(0) != ZUN_SUCCESS)
            {
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            }
            break;
        case SUPERVISOR_STATE_MAINMENU:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_GAMEMANAGER:
                if (GameManager::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            case SUPERVISOR_STATE_EXIT_ERROR:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR;
            case SUPERVISOR_STATE_RESULTSCREEN:
                if (ResultScreen::RegisterChain(0) != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            case SUPERVISOR_STATE_MUSICROOM:
                if (MusicRoom::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            case SUPERVISOR_STATE_ENDING:
                GameManager::CutChain();
                if (Ending::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            }
            break;
        case SUPERVISOR_STATE_RESULTSCREEN:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_MAINMENU:
                arg->curState = 0;
                goto CASE_SUPERVISOR_STATE_INIT;
            }
            break;
        case SUPERVISOR_STATE_GAMEMANAGER:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_MAINMENU:
                GameManager::CutChain();
                arg->curState = SUPERVISOR_STATE_INIT;
                ReplayManager::SaveReplay(NULL, NULL);
                goto CASE_SUPERVISOR_STATE_INIT;
                break;
            case SUPERVISOR_STATE_RESULTSCREEN_FROM_GAME:
                GameManager::CutChain();
                if (ResultScreen::RegisterChain(1) != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            case SUPERVISOR_STATE_RESTART_FROM_BEGINNING:
                GameManager::CutChain();
                if (!g_GameManager.practice && g_GameManager.difficulty < DIFF_EXTRA)
                {
                    g_GameManager.currentStage = DUMMYSTAGE;
                }
                else
                {
                    g_GameManager.currentStage--;
                }
                ReplayManager::SaveReplay(NULL, NULL);
                if (GameManager::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                arg->curState = SUPERVISOR_STATE_GAMEMANAGER;
                break;
            case SUPERVISOR_STATE_RESTART_STAGE:
                g_Supervisor.curState = SUPERVISOR_STATE_NEXT_STAGE;
                GameManager::CutChain();
                g_GameManager.currentStage--;
                if (GameManager::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                arg->curState = SUPERVISOR_STATE_GAMEMANAGER;
                break;
            case SUPERVISOR_STATE_NEXT_STAGE_USELESS:
                // ZUN bloat: The idea was likely to start the next stage
                // with all stats reset to initial values, but the curState
                // assignment literally right after makes it the exact same as
                // SUPERVISOR_STATE_NEXT_STAGE for all intents and purposes.
                g_Supervisor.curState = SUPERVISOR_STATE_NEXT_STAGE;
                GameManager::CutChain();
                if (GameManager::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                arg->curState = SUPERVISOR_STATE_GAMEMANAGER;
                break;
            case SUPERVISOR_STATE_NEXT_STAGE:
                GameManager::CutChain();
                if (GameManager::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                arg->curState = SUPERVISOR_STATE_GAMEMANAGER;
                break;
            case SUPERVISOR_STATE_REPLAY_END:
                GameManager::CutChain();
                arg->curState = SUPERVISOR_STATE_INIT;
                ReplayManager::SaveReplay(NULL, NULL);
                arg->curState = SUPERVISOR_STATE_MAINMENU;
                            if (MainMenu::RegisterChain(1) != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            case SUPERVISOR_STATE_ENDING:
                g_GameManager.plst.playDataByDifficulty[g_GameManager.difficulty]
                    .noContinueClearCount =
                    g_GameManager.plst.playDataByDifficulty[g_GameManager.difficulty]
                        .noContinueClearCount +
                    1;
                GameManager::CutChain();
                if (Ending::RegisterChain() != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            }
            break;
        case SUPERVISOR_STATE_RESULTSCREEN_FROM_GAME:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                ReplayManager::SaveReplay(NULL, NULL);
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_MAINMENU:
                arg->curState = SUPERVISOR_STATE_INIT;
                ReplayManager::SaveReplay(NULL, NULL);
                goto CASE_SUPERVISOR_STATE_INIT;
            }
            break;
        case SUPERVISOR_STATE_MUSICROOM:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_MAINMENU:
                arg->curState = SUPERVISOR_STATE_INIT;
                goto CASE_SUPERVISOR_STATE_INIT;
            }
            break;
        case SUPERVISOR_STATE_ENDING:
            switch (arg->curState)
            {
            case SUPERVISOR_STATE_EXIT:
                return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
            case SUPERVISOR_STATE_MAINMENU:
                arg->curState = SUPERVISOR_STATE_INIT;
                goto CASE_SUPERVISOR_STATE_INIT;
            case SUPERVISOR_STATE_RESULTSCREEN_FROM_GAME:
                if (ResultScreen::RegisterChain(1) != ZUN_SUCCESS)
                {
                    return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
                }
                break;
            }
            break;
        }
        g_CurFrameRawInput = g_LastFrameRawInput =
            g_IsEighthFrameOfHeldInput = 0;
    }
    arg->wantedState = arg->curState;
    arg->calcCount = arg->calcCount + 1;
    if (arg->calcCount % 4000 == 3999 &&
        g_Supervisor.CheckIntegrity("0100b", g_Supervisor.exeSize,
                                    g_Supervisor.exeChecksum) != ZUN_SUCCESS)
    {
        return CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS;
    }

    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: TH07 0x0043831b
u32 Supervisor::OnDraw(Supervisor *arg)
{
    DrawFpsCounter(1);
    return CHAIN_CALLBACK_RESULT_CONTINUE;
}

// FUNCTION: TH07 0x004383d8
//
// Replaces the DirectInput 8 setup. There is nothing left to create: the keyboard is
// read straight out of SDL (see Supervisor::GetInputFrame), and a gamepad -- if one is
// attached -- is a single SDL_Gamepad handle. SDL's gamepad button indices line up with
// the DirectInput joystick button indices that ControllerMapping stores (A, B, X, Y ...
// are gamepad buttons 0, 1, 2, 3), so no remapping is needed.
ZunResult Supervisor::SetupDInput()
{
    // The gamepad and joystick subsystems are not implied by SDL_INIT_VIDEO, so init
    // them explicitly. SDL_InitSubSystem is idempotent: if main.cpp already brought
    // SDL up, this only adds the flags that are still missing.
    if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS))
    {
        g_GameErrorContext.Log(TH_ERR_DINPUT_INIT_FAIL);
        return ZUN_ERROR;
    }

    // Pick up anything already plugged in. Hotplug is handled by the SDL_EVENT_GAMEPAD_ADDED
    // branch in ProcessInputEvents, so this only has to cover the initial state.
    this->RefreshGamepad();

    g_GameErrorContext.Log(TH_LOG_DINPUT_INIT_SUCCESS);
    if (g_Supervisor.controller)
    {
        g_GameErrorContext.Log(TH_LOG_FOUND_PAD);
    }
    return ZUN_SUCCESS;
}

// Opens the gamepad SDL currently reports as connected, if it is not open already.
// Called from SetupDInput and whenever an SDL_EVENT_GAMEPAD_ADDED arrives.
void Supervisor::RefreshGamepad()
{
    int count = 0;
    SDL_JoystickID *ids;

    if (g_Supervisor.controller)
    {
        return;
    }

    if (g_Supervisor.cfg.disableDinput)
    {
        return;
    }

    ids = SDL_GetGamepads(&count);
    if (ids && count > 0)
    {
        g_Supervisor.controller = SDL_OpenGamepad(ids[0]);
    }
    SDL_free(ids);
}

// The one place the raw button mask is produced for a frame.
//
// g_CurFrameRawInput holds the game's TouhouButton bitmask, not DIK scan codes: the
// original translated the DirectInput keyboard byte array through KEY_PRESSED() into
// TH_BUTTON_* bits, and that bit layout is what the gameplay code reads. The translation
// is unchanged; only its source moved to SDL_GetKeyboardState().
//
// SDL pumps events here rather than relying on the main loop to have done it: the event
// loop belongs to main.cpp / the renderer and is being reworked separately, whereas
// OnUpdate (and the frame-stepping loop in the title screen) call this directly.
u16 Supervisor::GetInputFrame()
{
    const bool *keys;
    u16 buttons;

    SDL_PumpEvents();
    ProcessInputEvents();

    buttons = 0;
    keys = SDL_GetKeyboardState(NULL);
    if (keys)
    {
        // Each line below binds the same *key* to the same Touhou button as the
        // original's KEY_PRESSED(DIK_...) chain did. The constants are not numerically
        // interchangeable: the DirectX SDK's DIK_* are PC set-1 scancodes (DIK_ESCAPE is
        // 0x01, DIK_D is 0x20) while SDL's SDL_SCANCODE_* follow USB HID usage order
        // (SDL_SCANCODE_ESCAPE is 41, SDL_SCANCODE_D is 7). Only the physical key they
        // name matters here, so the mapping is by key, not by value.
        if (keys[SDL_SCANCODE_UP])
            buttons |= TH_BUTTON_UP;
        if (keys[SDL_SCANCODE_DOWN])
            buttons |= TH_BUTTON_DOWN;
        if (keys[SDL_SCANCODE_LEFT])
            buttons |= TH_BUTTON_LEFT;
        if (keys[SDL_SCANCODE_RIGHT])
            buttons |= TH_BUTTON_RIGHT;
        if (keys[SDL_SCANCODE_KP_8])
            buttons |= TH_BUTTON_UP;
        if (keys[SDL_SCANCODE_KP_2])
            buttons |= TH_BUTTON_DOWN;
        if (keys[SDL_SCANCODE_KP_4])
            buttons |= TH_BUTTON_LEFT;
        if (keys[SDL_SCANCODE_KP_6])
            buttons |= TH_BUTTON_RIGHT;
        if (keys[SDL_SCANCODE_KP_7])
            buttons |= TH_BUTTON_UP_LEFT;
        if (keys[SDL_SCANCODE_KP_9])
            buttons |= TH_BUTTON_UP_RIGHT;
        if (keys[SDL_SCANCODE_KP_1])
            buttons |= TH_BUTTON_DOWN_LEFT;
        if (keys[SDL_SCANCODE_KP_3])
            buttons |= TH_BUTTON_DOWN_RIGHT;
        if (keys[SDL_SCANCODE_HOME])
            buttons |= TH_BUTTON_HOME;
        if (keys[SDL_SCANCODE_D])
            buttons |= TH_BUTTON_D;
        if (keys[SDL_SCANCODE_Z])
            buttons |= TH_BUTTON_SHOOT;
        if (keys[SDL_SCANCODE_X])
            buttons |= TH_BUTTON_BOMB;
        if (keys[SDL_SCANCODE_LSHIFT])
            buttons |= TH_BUTTON_FOCUS;
        if (keys[SDL_SCANCODE_RSHIFT])
            buttons |= TH_BUTTON_FOCUS;
        if (keys[SDL_SCANCODE_ESCAPE])
            buttons |= TH_BUTTON_MENU;
        if (keys[SDL_SCANCODE_LCTRL])
            buttons |= TH_BUTTON_SKIP;
        if (keys[SDL_SCANCODE_RCTRL])
            buttons |= TH_BUTTON_SKIP;
        if (keys[SDL_SCANCODE_Q])
            buttons |= TH_BUTTON_Q;
        if (keys[SDL_SCANCODE_S])
            buttons |= TH_BUTTON_S;
        if (keys[SDL_SCANCODE_RETURN])
            buttons |= TH_BUTTON_ENTER;
        if (keys[SDL_SCANCODE_R])
            buttons |= TH_BUTTON_RESET;
    }

    return Supervisor::GetGamepadInput(buttons);
}

// The gamepad half of Controller::GetControllerInput(): the configured buttons and the
// left stick, read from the SDL_Gamepad handle. Without a gamepad this returns the
// keyboard-only mask unchanged, which is the original's behaviour with no DirectInput
// controller present.
//
// ControllerMapping stores DirectInput joystick button indices; SDL's SDL_GamepadButton
// values are the same numbering (A=0, B=1, X=2, Y=3, BACK=4, GUIDE=5, START=6, sticks 7,
// shoulders 9..12, dpad 11..14), so the mapping is used as-is.
u16 Supervisor::GetGamepadInput(u16 buttons)
{
    const ControllerMapping *mapping;
    SDL_Gamepad *pad;
    u32 shootPressed;
    i32 x;
    i32 y;
    i32 axisLimit;

    pad = g_Supervisor.controller;
    if (!pad)
    {
        return buttons;
    }

    mapping = &g_Supervisor.cfg.controllerMapping;
    x = (i32)SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTX);
    y = (i32)SDL_GetGamepadAxis(pad, SDL_GAMEPAD_AXIS_LEFTY);
    axisLimit = ((i32)g_Supervisor.cfg.padAxisX + g_Supervisor.cfg.padAxisY) / 2;
    if (axisLimit <= 0)
    {
        axisLimit = 1;
    }

    shootPressed = Supervisor::SetButtonFromGamepad(&buttons, mapping->shootButton,
                                                    TH_BUTTON_SHOOT, pad);
    if (g_Supervisor.cfg.shotSlow)
    {
        if (shootPressed != 0)
        {
            if (g_GamepadAutoFocusTimer < 20)
            {
                g_GamepadAutoFocusTimer++;
            }
            if (g_GamepadAutoFocusTimer >= 10)
            {
                buttons |= TH_BUTTON_FOCUS;
            }
        }
        else
        {
            if (g_GamepadAutoFocusTimer > 10)
            {
                g_GamepadAutoFocusTimer -= 10;
                buttons |= TH_BUTTON_FOCUS;
            }
            else
            {
                g_GamepadAutoFocusTimer = 0;
            }
        }
    }

    Supervisor::SetButtonFromGamepad(&buttons, mapping->bombButton, TH_BUTTON_BOMB, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->focusButton, TH_BUTTON_FOCUS, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->menuButton, TH_BUTTON_MENU, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->upButton, TH_BUTTON_UP, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->downButton, TH_BUTTON_DOWN, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->leftButton, TH_BUTTON_LEFT, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->rightButton, TH_BUTTON_RIGHT, pad);
    Supervisor::SetButtonFromGamepad(&buttons, mapping->skipButton, TH_BUTTON_SKIP, pad);

    // The original also always treated joystick button 7 as the cheat-code D button.
    if (SDL_GetGamepadButton(pad, (SDL_GamepadButton)7))
    {
        buttons |= TH_BUTTON_D;
    }

    // DirectInput's lY grew downwards, matching TH_BUTTON_DOWN; SDL's LEFT Y grows
    // upwards, so the comparisons are swapped relative to the old code.
    if (x > axisLimit)
    {
        buttons |= TH_BUTTON_RIGHT;
    }
    if (x < -axisLimit)
    {
        buttons |= TH_BUTTON_LEFT;
    }
    if (y > axisLimit)
    {
        buttons |= TH_BUTTON_UP;
    }
    if (y < -axisLimit)
    {
        buttons |= TH_BUTTON_DOWN;
    }

    return buttons;
}

// The gamepad counterpart of Controller::SetButtonFromDirectInputJoystate: ORs the
// Touhou button in when the mapped gamepad button is held, and reports whether it was.
u32 Supervisor::SetButtonFromGamepad(u16 *outButtons, i16 controllerButtonToTest,
                                     u32 touhouButton, SDL_Gamepad *pad)
{
    if (controllerButtonToTest < 0 || !pad)
    {
        return 0;
    }

    if (SDL_GetGamepadButton(pad, (SDL_GamepadButton)controllerButtonToTest))
    {
        *outButtons |= (u16)touhouButton;
        return touhouButton;
    }
    return 0;
}

// Consumes the events this subsystem owns. The window-close and quit handling belongs
// to main.cpp, so only gamepad add/remove is looked at here; everything else is left in
// the queue for whoever else drains it.
void Supervisor::ProcessInputEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_GAMEPAD_ADDED)
        {
            Supervisor::RefreshGamepad();
        }
        else if (event.type == SDL_EVENT_GAMEPAD_REMOVED && g_Supervisor.controller &&
                 event.gdevice.which == SDL_GetGamepadID(g_Supervisor.controller))
        {
            SDL_CloseGamepad(g_Supervisor.controller);
            g_Supervisor.controller = NULL;
        }
    }

    // A pad that was unplugged while its removal event went to someone else, or that was
    // already attached before SDL saw it, would otherwise leave a dangling handle.
    if (g_Supervisor.controller && !SDL_GamepadConnected(g_Supervisor.controller))
    {
        SDL_CloseGamepad(g_Supervisor.controller);
        g_Supervisor.controller = NULL;
        Supervisor::RefreshGamepad();
    }
}

// FUNCTION: TH07 0x00438668
ZunResult Supervisor::LoadGameData()
{
    char verFile[128];

    // STRING: TH07 0x00497150
    if (g_Pbg4Archive.Load("th07.dat"))
    {
        // STRING: TH07 0x00497140
        sprintf(verFile, "th07_%.4x%c.ver", 256, 98);
        g_Supervisor.versionData = (char *)FileSystem::OpenFile(verFile, 0);
        g_Supervisor.versionDataSize = g_LastFileSize;
        if (!g_Supervisor.versionData)
        {
            g_GameErrorContext.Fatal(TH_ERR_DATA_VER_MISMATCH);
            return ZUN_ERROR;
        }
    }
    else
    {
        g_GameErrorContext.Fatal(TH_ERR_DAT_NOT_FOUND);
        return ZUN_ERROR;
    }
    return ZUN_SUCCESS;
}

#pragma var_order(i, frameCount, timeStart, fpsArray, fpsCount, timeEnd, \
                  timeDiff, fps, unused, j, fpsSum)
// FUNCTION: TH07 0x004386f3
i32 Supervisor::CheckVSync()
{
    f32 fpsSum;
    i32 j;
    f32 unused;
    f32 fps;
    i32 timeDiff;
    DWORD timeEnd;
    i32 fpsCount;
    f32 fpsArray[29];
    DWORD timeStart;
    i32 frameCount;
    i32 i;

    i = 0;
    frameCount = 0;
    fpsCount = 0;
    timeStart = 0;

    timeBeginPeriod(1);
    timeStart = timeGetTime();
    timeEndPeriod(1);

    while (i < 1800 && fpsCount < 8)
    {
        RendererBeginFrame();
        g_AnmManager->CopySurfaceToBackBuffer(0, 0, 0, 0, 0);
        RendererSwap();
        i++;
        timeBeginPeriod(1);
        timeEnd = timeGetTime();
        timeEndPeriod(1);
        frameCount++;
        timeDiff = timeEnd - timeStart;

        if (timeDiff >= 700)
        {
            timeStart = timeEnd;
            frameCount = 0;
        }
        else if (timeDiff >= 500)
        {
            unused = (f32)timeDiff / 1000.0f;
            fps = frameCount * 1000.0f / (f32)timeDiff;
            if (fps >= 57.0f)
            {
                fpsArray[fpsCount] = fps;
                fpsCount++;
            }
            timeStart = timeEnd;
            frameCount = 0;
        }
    }

    if (!g_Supervisor.cfg.disableVsync)
    {
        fpsSum = 0.0f;
        if (fpsCount >= 2)
        {
            for (j = 0; j < fpsCount; j++)
            {
                fpsSum += fpsArray[j];
            }
            fpsSum /= (f32)j;
        }
        else
        {
            fpsSum = 1000.0f;
        }

        if (fpsSum > 160.0f)
        {
            g_GameErrorContext.Log(TH_LOG_VSYNC_FAIL);
            g_GameErrorContext.Log(TH_LOG_FORCE_60FPS);
            g_Supervisor.vsyncDisabled = TRUE;
        }
        else if (fpsSum >= 65.0f)
        {
            g_GameErrorContext.Log(TH_LOG_VSYNC_FAIL_2);
            g_GameErrorContext.Log(TH_LOG_FORCE_60FPS);
            g_Supervisor.vsyncDisabled = TRUE;
            return -2;
        }
    }
    return 0;
}

// FUNCTION: TH07 0x00438986
ZunResult Supervisor::AddedCallback(Supervisor *arg)
{
    ScoreDat *scoreDat;
    i32 i;

    QueryPerformanceFrequency(&arg->perfFrequency);
    // Clear to black and present twice, as the original did, so the window never shows
    // stale contents while the data files load.
    RendererClear(0.0f, 0.0f, 0.0f);
    RendererSwap();
    RendererClear(0.0f, 0.0f, 0.0f);
    RendererSwap();
    if (LoadGameData() != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    // STRING: TH07 0x00497038
    g_AnmManager->LoadSurface(0, "data/title/th07logo.jpg");
    g_Supervisor.isInEnding = TRUE;
    if (!g_Supervisor.vsyncDisabled)
    {
        if (CheckVSync())
        {
            g_AnmManager->ReleaseSurface(0);
            return (ZunResult)-2;
        }
    }
    else
    {
        i = 0;
        while (i < 4)
        {
            RendererBeginFrame();
            g_AnmManager->CopySurfaceToBackBuffer(0, 0, 0, 0, 0);
            RendererSwap();
            i++;
        }
    }
    g_AnmManager->ReleaseSurface(0);
    arg->isInEnding = FALSE;
    arg->renderSkipFrames = 0;
    arg->lastTotalPlayTimeUpdate = timeGetTime();
    g_Rng.SetSeed(arg->lastTotalPlayTimeUpdate);
    arg->SetupDInput();
    if (!arg->midiOutput)
    {
        arg->midiOutput = ZUN_NEW(MidiOutput, "MidiSysInf");
    }
    if (arg->midiOutput)
    {
        // STRING: TH07 0x00497028
        arg->midiOutput->ReadFileData(30, "bgm/init.mid");
    }
    g_SoundPlayer.InitSoundBuffers();
    // STRING: TH07 0x00497018
    if (g_AnmManager->LoadAnms(ANM_FILE_TEXT, "data/text.anm", ANM_OFFSET_TEXT) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    if (AsciiManager::RegisterChain() != ZUN_SUCCESS)
    {
        g_GameErrorContext.Log(TH_BGM_ASCII_INIT_FAIL);
        return ZUN_ERROR;
    }

    g_AnmManager->SetupVertexBuffer();
    TextHelper::CreateTextBuffer();

    // The font the game draws text with. A face with wide script coverage is preferred so
    // that translations do not have to change this list; the first one that loads wins.
    {
        const char *fontCandidates[] = {
            "font/default.ttf",
            "C:/Windows/Fonts/msgothic.ttc",
            "C:/Windows/Fonts/YuGothR.ttc",
            "C:/Windows/Fonts/NotoSans-Regular.ttf",
            "C:/Windows/Fonts/arial.ttf",
        };
        for (i32 f = 0; f < ARRAY_SIZE_SIGNED(fontCandidates); f++)
        {
            FontFace *face = FontLoad(fontCandidates[f]);
            if (face)
            {
                FontSetDefault(face);
                break;
            }
        }
        if (!FontGetDefault())
        {
            g_GameErrorContext.Log("error : no usable font, text will not be drawn");
        }
    }

    // STRING: TH07 0x00496fe0
    if (g_SoundPlayer.LoadFmt("bgm/thbgm.fmt"))
    {
        g_GameErrorContext.Log(TH_ERR_BGM_INIT_FAIL);
        return ZUN_ERROR;
    }

    if (g_SoundPlayer.bgmSeekOffset == 0)
    {
        if (!g_Supervisor.cfg.preloadBgm)
        {
            // STRING: TH07 0x00496fac
            g_SoundPlayer.StartBGM("thbgm.dat");
        }
        else
        {
            memcpy(g_SoundPlayer.bgmArchivePath, "thbgm.dat", 10);
        }
    }
    else if (!g_Supervisor.cfg.preloadBgm)
    {
        g_SoundPlayer.StartBGM("th07.dat");
    }
    else
    {
        memcpy(g_SoundPlayer.bgmArchivePath, "th07.dat", 9);
    }
    scoreDat = ResultScreen::OpenScore("score.dat");
    memset(&g_GameManager.plst, 0, sizeof(g_GameManager.plst));
    g_GameManager.plst.th7kLen2 = g_GameManager.plst.th7kLen = sizeof(Plst);
    g_GameManager.plst.magic = PLST_MAGIC;
    g_GameManager.plst.version = 1;
    ResultScreen::ParsePlst(scoreDat, &g_GameManager.plst);
    ResultScreen::ReleaseScoreDat(scoreDat);
    g_Supervisor.midiTimer = ZUN_NEW(DummyMidiTimer, "DummyTimerSysInf");
    if (g_Supervisor.midiTimer)
    {
        g_Supervisor.midiTimer->StartTimer();
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x00438de2
ZunResult Supervisor::DeletedCallback(Supervisor *arg)
{
    SAFE_FREE(g_Supervisor.versionData);
    g_AnmManager->ReleaseVertexBuffer();
    g_AnmManager->ReleaseAnm(ANM_FILE_TEXT);
    AsciiManager::CutChain();
    // STRING: TH07 0x004980d0
    g_SoundPlayer.PushCommand(AUDIO_SHUTDOWN, 0, "dummy");
    if (arg->midiOutput)
    {
        arg->midiOutput->StopPlayback();
        ZUN_DELETE(arg->midiOutput);
    }
    ReplayManager::SaveReplay(NULL, NULL);
    TextHelper::ReleaseTextBuffer();
    if (arg->controller)
    {
        SDL_CloseGamepad(arg->controller);
        arg->controller = NULL;
    }

    if (g_GameManager.globals)
    {
        ZUN_DELETE(g_GameManager.globals);
    }

    if (g_GameManager.defaultCfg)
    {
        ZUN_DELETE(g_GameManager.defaultCfg);
    }

    g_Pbg4Archive.Release();
    if (g_Supervisor.midiTimer)
    {
        g_Supervisor.midiTimer->StopTimer();
        ZUN_DELETE(g_Supervisor.midiTimer);
    }
    return ZUN_SUCCESS;
}

#pragma var_order(chain, res, mgr)
// FUNCTION: TH07 0x00439000
ZunResult Supervisor::RegisterChain()
{
    ZunResult res;

    Supervisor *mgr = &g_Supervisor;
    mgr->wantedState = SUPERVISOR_STATE_INIT;
    mgr->curState = SUPERVISOR_STATE_EXIT;
    mgr->calcCount = 0;
    ChainElem *chain = g_Chain.CreateElem((ChainCallback)OnUpdate);
    chain->arg = mgr;
    chain->addedCallback = (ChainLifecycleCallback)AddedCallback;
    chain->deletedCallback = (ChainLifecycleCallback)DeletedCallback;
    res = g_Chain.AddToCalcChain(chain, CHAIN_PRIO_CALC_SUPERVISOR);
    if (res)
    {
        return res;
    }

    chain = g_Chain.CreateElem((ChainCallback)OnDraw);
    chain->arg = mgr;
    g_Chain.AddToDrawChain(chain, CHAIN_PRIO_DRAW_SUPERVISOR);
    return ZUN_SUCCESS;
}

#pragma var_order(fps, elapsedTimeInSecs, curTime, targetFps, curPerfCounter, \
                  fpsCounterPos, replayFpsCounterPos)
// FUNCTION: TH07 0x004390a5
void Supervisor::DrawFpsCounter(i32 showFps)
{
    Float3 replayFpsCounterPos;
    Float3 fpsCounterPos;
    LARGE_INTEGER curPerfCounter;
    f32 targetFps;
    DWORD curTime;
    f32 elapsedTimeInSecs;
    f32 fps;

    if (!g_GameManager.slowModeSlowActive)
    {
        g_NumFramesSinceLastTime =
            g_NumFramesSinceLastTime + 1 + (u32)g_Supervisor.cfg.frameskipConfig;

        if (g_Supervisor.perfFrequency.LowPart == 0)
        {
            static DWORD g_LastTime = timeGetTime();

            curTime = timeGetTime();
            if (curTime < g_LastTime)
            {
                g_LastTime = curTime;
                g_NumFramesSinceLastTime = 0;
            }
            if (curTime - g_LastTime >= 500)
            {
                elapsedTimeInSecs = (f32)(curTime - g_LastTime) / 1000.0f;
                g_LastTime = curTime;

            MERGE:
                fps = (f32)g_NumFramesSinceLastTime / elapsedTimeInSecs;
                g_NumFramesSinceLastTime = 0;
                // STRING: TH07 0x00496fa0
                sprintf(g_FpsCounterBuffer, "%.02ffps", (f64)fps);
                if (g_GameManager.notInMenu && showFps)
                {
                    targetFps = 60.0f;
                    g_Supervisor.fpsAccumulator = g_Supervisor.fpsAccumulator + targetFps;
                    if (targetFps * 0.9f < fps)
                    {
                        g_Supervisor.framerateMultiplier =
                            g_Supervisor.framerateMultiplier + targetFps;
                    }
                    else if (targetFps * 0.7f < fps)
                    {
                        g_Supervisor.framerateMultiplier =
                            g_Supervisor.framerateMultiplier + targetFps * 0.8f;
                    }
                    else if (targetFps * 0.5f < fps)
                    {
                        g_Supervisor.framerateMultiplier =
                            g_Supervisor.framerateMultiplier + targetFps * 0.6f;
                    }
                    else
                    {
                        g_Supervisor.framerateMultiplier =
                            g_Supervisor.framerateMultiplier + targetFps * 0.5f;
                    }

                    if (!g_GameManager.replay)
                    {
                        g_Supervisor.curFps = fps + 0.5f;
                    }
                    else
                    {
                        // STRING: TH07 0x00496f9c
                        sprintf(g_ReplayFpsBuffer, "%2d", (i32)g_Supervisor.curFps);
                    }
                }
            }
            goto LAB_00439350;
        }

        if (g_PerformanceCounter.LowPart == 0)
        {
            QueryPerformanceCounter(&g_PerformanceCounter);
        }
        QueryPerformanceCounter(&curPerfCounter);
        if (curPerfCounter.LowPart < g_PerformanceCounter.LowPart)
        {
            g_PerformanceCounter.LowPart = curPerfCounter.LowPart;
            g_PerformanceCounter.HighPart = curPerfCounter.HighPart;
            g_NumFramesSinceLastTime = 0;
        }
        if (curPerfCounter.LowPart >= g_PerformanceCounter.LowPart +
                                          (g_Supervisor.perfFrequency.LowPart >> 1))
        {
            elapsedTimeInSecs =
                (f32)(curPerfCounter.LowPart - g_PerformanceCounter.LowPart) /
                (f32)g_Supervisor.perfFrequency.LowPart;
            g_PerformanceCounter.LowPart = curPerfCounter.LowPart;
            g_PerformanceCounter.HighPart = curPerfCounter.HighPart;
            g_FpsUpdateCounter++;
            if (g_FpsUpdateCounter % 8 == 0)
            {
                g_Supervisor.CheckTiming();
            }
            goto MERGE;
        }
    }

LAB_00439350:
    if (!g_Supervisor.isInEnding && showFps)
    {
        fpsCounterPos.x = 512.0f;
        fpsCounterPos.y = 464.0f;
        fpsCounterPos.z = 0.0f;
        g_AsciiManager.AddString(&fpsCounterPos, g_FpsCounterBuffer);
        if (g_GameManager.replay &&
            g_GameManager.notInMenu)
        {
            replayFpsCounterPos.x = 384.0f;
            replayFpsCounterPos.y = 448.0f;
            replayFpsCounterPos.z = 0.0f;
            if (g_Supervisor.isFpsBad)
            {
                g_AsciiManager.color = 0xffff4040;
            }
            else
            {
                g_AsciiManager.color = 0xffffffd0;
            }
            g_AsciiManager.AddString(&replayFpsCounterPos, g_ReplayFpsBuffer);
            g_AsciiManager.color = 0xffffffff;
        }
    }
}

// FUNCTION: TH07 0x00439401
void ZunTimer::Increment(i32 value)
{
    if (g_Supervisor.forceIntegerTimer)
    {
        this->current++;
        this->subFrame = 0.0f;
        this->previous = -999;
    }
    if (g_Supervisor.effectiveFramerateMultiplier > 0.99f)
    {
        this->current = this->current + value;
    }
    else
    {
        if (value < 0)
        {
            Decrement(-value);
        }
        else
        {
            this->previous = this->current;
            this->subFrame = (f32)value * g_Supervisor.effectiveFramerateMultiplier +
                             this->subFrame;
            while (this->subFrame >= 1.0f)
            {
                this->current = this->current + 1;
                this->subFrame = this->subFrame - 1.0f;
            }
        }
    }
}

// FUNCTION: TH07 0x004394c7
void ZunTimer::Decrement(i32 value)
{
    if (g_Supervisor.forceIntegerTimer)
    {
        this->current--;
        this->subFrame = 0.0f;
        this->previous = -999;
    }
    if (g_Supervisor.effectiveFramerateMultiplier > 0.99f)
    {
        this->current = this->current - value;
    }
    else
    {
        if (value < 0)
        {
            Increment(-value);
        }
        else
        {
            this->previous = this->current;
            this->subFrame = this->subFrame -
                             (f32)value * g_Supervisor.effectiveFramerateMultiplier;
            while (this->subFrame < 0.0f)
            {
                this->current--;
                this->subFrame = this->subFrame + 1.0f;
            }
        }
    }
}

// FUNCTION: TH07 0x0043958d
void Supervisor::TickTimer(i32 *frames, f32 *subframes)
{
    if (this->effectiveFramerateMultiplier <= 0.99f)
    {
        *subframes = *subframes + this->effectiveFramerateMultiplier;
        if (*subframes >= 1.0f)
        {
            *frames = *frames + 1;
            *subframes = *subframes - 1.0f;
        }
    }
    else
    {
        *frames = *frames + 1;
    }
}

// ZUN name: snapShotScreen
#pragma var_order(bmfh, versionDataExeSize, local_1c, backBuffer, stride,          \
                  srcPixel, dstPixel, y, x, bytesPerRow, lockedRect, bytesWritten, \
// FUNCTION: TH07 0x004395fb
ZunBool Supervisor::TakeSnapshot(const char *filename)
{
    HANDLE bitmapFile;
    DWORD bytesWritten;
    i32 stride;
    i32 x;
    i32 y;
    u8 *dstPixel;
    u8 *srcPixel;
    u8 *pixels;
    BITMAPINFO *bitmapInfo;
    void *bitmapData;
    BITMAPFILEHEADER bmfh;

    bitmapInfo = NULL;
    bitmapData = NULL;

    memset(&bmfh, 0, sizeof(BITMAPFILEHEADER));

    // STRING: TH07 0x00496f98
    bmfh.bfType = *(WORD *)&"BM";
    bmfh.bfSize = bmfh.bfOffBits = 54;

    // Read the framebuffer back. The original locked the D3D back buffer; GL hands it
    // back as RGBA8, which is then written out as a 24-bit BMP.
    pixels = (u8 *)malloc((size_t)GAME_WINDOW_WIDTH * GAME_WINDOW_HEIGHT * 4);
    if (!pixels)
    {
        return TRUE;
    }
    RendererReadPixels(pixels, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);

    bitmapInfo = (BITMAPINFO *)ZUN_ALLOC(sizeof(BITMAPINFO));
    if (!bitmapInfo)
    {
        g_GameErrorContext.Log(TH_LOG_BITMAP_ALLOC_FAIL);
        free(pixels);
        return TRUE;
    }

    memset(bitmapInfo, 0, sizeof(BITMAPINFO));
    stride = 1920;
    bitmapData = malloc(stride * GAME_WINDOW_HEIGHT);
    if (!bitmapData)
    {
        g_GameErrorContext.Log(TH_LOG_BITMAP_ALLOC_FAIL);
        ZUN_FREE(bitmapInfo);
        free(pixels);
        return TRUE;
    }

    bmfh.bfSize += stride * GAME_WINDOW_HEIGHT;
    bitmapInfo->bmiHeader.biBitCount = 24;
    bitmapInfo->bmiHeader.biSize = 40;
    bitmapInfo->bmiHeader.biWidth = GAME_WINDOW_WIDTH;
    bitmapInfo->bmiHeader.biHeight = GAME_WINDOW_HEIGHT;
    bitmapInfo->bmiHeader.biPlanes = 1;
    bitmapInfo->bmiHeader.biCompression = 0;

    // BMP rows run bottom-up and are BGR, so flip and reorder while copying.
    for (y = GAME_WINDOW_HEIGHT - 1; y > -1; y--)
    {
        dstPixel = (u8 *)bitmapData + stride * (GAME_WINDOW_HEIGHT - 1 - y);
        srcPixel = pixels + (size_t)y * GAME_WINDOW_WIDTH * 4;
        for (x = 0; x < GAME_WINDOW_WIDTH; x++)
        {
            dstPixel[0] = srcPixel[2];
            dstPixel[1] = srcPixel[1];
            dstPixel[2] = srcPixel[0];
            srcPixel += 4;
            dstPixel += 3;
        }
    }
    free(pixels);

    bitmapFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);
    if (bitmapFile == INVALID_HANDLE_VALUE)
    {
        ZUN_FREE(bitmapInfo);
        ZUN_FREE(bitmapData);
        return TRUE;
    }

    WriteFile(bitmapFile, &bmfh, sizeof(bmfh), &bytesWritten, NULL);
    WriteFile(bitmapFile, bitmapInfo, sizeof(bitmapInfo->bmiHeader), &bytesWritten,
              NULL);
    WriteFile(bitmapFile, bitmapData, stride * GAME_WINDOW_HEIGHT, &bytesWritten,
              NULL);
    CloseHandle(bitmapFile);

    ZUN_FREE(bitmapInfo);
    ZUN_FREE(bitmapData);
    return TRUE;
}

#pragma var_order(configFile, bgm2, bytesRead2, bgm2Data, bgm, bytesRead, bgmData)
// FUNCTION: TH07 0x004398b6
ZunResult Supervisor::LoadConfig(const char *configFilename)
{
    i32 bgmData[4];
    DWORD bytesRead;
    HANDLE bgm;
    i32 bgm2Data[4];
    DWORD bytesRead2;
    HANDLE bgm2;
    u32 *configFile;

    memset(&g_Supervisor.cfg, 0, sizeof(GameConfiguration));
    configFile = (u32 *)FileSystem::OpenFile((char *)configFilename, 1);
    if (!configFile)
    {
        g_GameErrorContext.Log(TH_LOG_CONFIG_INIT);
    init:
        g_Supervisor.cfg.lifeCount = 2;
        g_Supervisor.cfg.bombCount = 3;
        g_Supervisor.cfg.colorMode16bit = 255;
        g_Supervisor.cfg.version = 0x70002;
        g_Supervisor.cfg.padAxisX = 600;
        g_Supervisor.cfg.padAxisY = 600;
        bgm2 = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
        if (bgm2 != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgm2, bgm2Data, 16, &bytesRead2, NULL);
            CloseHandle(bgm2);
            if (bgm2Data[0] != 0x5641575a || bgm2Data[1] != 1 ||
                bgm2Data[2] != 0x700)
            {
                g_GameErrorContext.Fatal(TH_ERR_BGM_VER_MISMATCH);
                return ZUN_ERROR;
            }
            g_Supervisor.cfg.musicMode = MUSIC_WAV;
        }
        else
        {
            g_Supervisor.cfg.musicMode = MUSIC_MIDI;
            utils::DebugPrint2(TH_LOG_WAV_UNAVAILABLE);
        }
        g_Supervisor.cfg.playSounds = 1;
        g_Supervisor.cfg.defaultDifficulty = (u8)DIFF_NORMAL;
        g_Supervisor.cfg.windowed = 0;
        g_Supervisor.cfg.frameskipConfig = 0;
        g_Supervisor.cfg.controllerMapping = g_ControllerMapping;
        g_Supervisor.cfg.effectQuality = QUALITY_BEAUTIFUL;
        g_Supervisor.cfg.slowMode = 0;
        g_Supervisor.cfg.shotSlow = 1;
    }
    else
    {
        g_Supervisor.cfg = *(GameConfiguration *)configFile;
        ZUN_FREE(configFile);

        bgm = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
        if (bgm != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgm, bgmData, 16, &bytesRead, NULL);
            CloseHandle(bgm);
            if (bgmData[0] != 0x5641575a || bgmData[1] != 1 ||
                bgmData[2] != 0x700)
            {
                g_GameErrorContext.Fatal(TH_ERR_BGM_VER_MISMATCH);
                return ZUN_ERROR;
            }
        }
        if (!(g_Supervisor.cfg.lifeCount < 5 &&
              g_Supervisor.cfg.bombCount < 4 &&
              g_Supervisor.cfg.colorMode16bit < 2 &&
              g_Supervisor.cfg.musicMode < 3 &&
              g_Supervisor.cfg.defaultDifficulty < 6 &&
              g_Supervisor.cfg.playSounds < 2 &&
              g_Supervisor.cfg.windowed < 2 &&
              g_Supervisor.cfg.frameskipConfig < 3 &&
              g_Supervisor.cfg.effectQuality < 3 &&
              g_Supervisor.cfg.slowMode < 2 &&
              g_Supervisor.cfg.shotSlow < 2 &&
              g_Supervisor.cfg.version == 0x70002 &&
              g_LastFileSize == sizeof(GameConfiguration)))
        {
            g_GameErrorContext.Log(TH_LOG_CONFIG_REINIT);
            goto init;
        }
        g_ControllerMapping = g_Supervisor.cfg.controllerMapping;
    }
    g_Supervisor.cfg.colorAddEmulation = 1;
    if (this->cfg.noVertexBuffers)
    {
        g_GameErrorContext.Log(TH_CONFIG_NO_VERTEX_BUFFERS);
    }
    if (this->cfg.disableFog)
    {
        g_GameErrorContext.Log(TH_CONFIG_NO_FOG);
    }
    if (this->cfg.use16BitTextures)
    {
        g_GameErrorContext.Log(TH_CONFIG_FORCE_16BIT);
    }
    if (this->IsClearingBackbuffer())
    {
        g_GameErrorContext.Log(TH_CONFIG_FORCE_BACKBUFFER_CLEAR);
    }
    if (this->cfg.disableItemDrawAroundPlayfield)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_ITEM_DRAW_AROUND_PLAYFIELD);
    }
    if (this->cfg.disableGouraud)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_GOURAUD);
    }
    if (this->cfg.disableZBuffer)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_DEPTH_TEST);
    }
    this->vsyncDisabled = FALSE;
    this->cfg.unused = 0;
    if (this->cfg.disableTextureBlend)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_TEXTURE_BLEND);
    }
    if (this->cfg.windowed)
    {
        g_GameErrorContext.Log(TH_CONFIG_WINDOWED);
    }
    if (this->cfg.forceReferenceRender)
    {
        g_GameErrorContext.Log(TH_CONFIG_FORCE_REFERENCE_RENDER);
    }
    if (this->cfg.disableDinput)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_DINPUT);
    }
    if (this->cfg.redrawEveryFrame)
    {
        g_GameErrorContext.Log(TH_CONFIG_REDRAW_EVERY_FRAME);
    }
    if (this->cfg.preloadBgm)
    {
        g_GameErrorContext.Log(TH_CONFIG_PRELOAD_BGM);
    }
    if (this->cfg.disableVsync)
    {
        g_GameErrorContext.Log(TH_CONFIG_DISABLE_VSYNC);
        g_Supervisor.vsyncDisabled = TRUE;
    }
    if (FileSystem::WriteDataToFile(configFilename, &g_Supervisor.cfg,
                                    sizeof(GameConfiguration)))
    {
        g_GameErrorContext.Fatal(TH_ERR_WRITE_CONFIG_FAIL, configFilename);
        g_GameErrorContext.Fatal(TH_ERR_WRITE_CONFIG_FAIL_HELP);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

#pragma var_order(pathext, pathbuf)
// FUNCTION: TH07 0x00439dd0
i32 Supervisor::LoadAudio(i32 idx, const char *path)
{
    char pathbuf[256];
    char *pathext;

    if (g_Supervisor.cfg.musicMode == MUSIC_MIDI)
    {
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->ReadFileData(idx, path);
        }
        return 0;
    }

    if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
    {
        strcpy(pathbuf, path);

        // ZUN landmine: the result of strrchr is not checked for NULL.
        pathext = strrchr(pathbuf, '.');
        pathext[1] = 'w';
        pathext[2] = 'a';
        pathext[3] = 'v';
        g_SoundPlayer.PushCommand(AUDIO_PRELOAD, idx, pathbuf);
    }

    return 1;
}

// FUNCTION: TH07 0x00439ec1
ZunResult Supervisor::PlayLoadedAudio(i32 idx)
{
    if (g_Supervisor.cfg.musicMode == MUSIC_MIDI)
    {
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->PlayLoaded(idx);
        }
        return ZUN_SUCCESS;
    }
    if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
    {
        if (g_Supervisor.cfg.preloadBgm)
        {
            g_SoundPlayer.PushCommand(AUDIO_SHUTDOWN, 0, "dummy");
        }
        g_SoundPlayer.PushCommand(AUDIO_START, idx, "dummy");
    }
    return ZUN_SUCCESS;
}

#pragma var_order(pathExt, pathBuf)
// FUNCTION: TH07 0x00439f4d
ZunResult Supervisor::PlayAudio(const char *path)
{
    char pathBuf[256];
    char *pathExt;

    if (g_Supervisor.cfg.musicMode == MUSIC_MIDI)
    {
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->Play(path);
        }
    }
    else
    {
        if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
        {
            // ZUN landmine: the result of strrchr is not checked for NULL.
            strcpy(pathBuf, path);
            pathExt = strrchr(pathBuf, '.');
            pathExt[1] = 'w';
            pathExt[2] = 'a';
            pathExt[3] = 'v';
            g_SoundPlayer.PushCommand(AUDIO_START, -1, pathBuf);
        }
        else
        {
            return ZUN_ERROR;
        }
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0043a05f
ZunResult Supervisor::StopAudio()
{
    if (g_Supervisor.cfg.musicMode == MUSIC_MIDI)
    {
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->StopPlayback();
        }
    }
    else
    {
        if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
        {
            if (g_Supervisor.cfg.preloadBgm)
            {
                g_SoundPlayer.PushCommand(AUDIO_SHUTDOWN, 0, "dummy");
            }
            else
            {
                g_SoundPlayer.PushCommand(AUDIO_STOP, 0, "dummy");
            }
        }
        else
        {
            return ZUN_ERROR;
        }
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0043a0d6
i32 Supervisor::FadeOutMusic(f32 musicFadeFrames)
{
    f32 effectiveFadeFrames;

    if (g_Supervisor.cfg.musicMode == MUSIC_MIDI)
    {
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->SetFadeOut(1000.0f * musicFadeFrames);
        }
    }
    else
    {
        if (g_Supervisor.cfg.musicMode == MUSIC_WAV)
        {
            if (this->effectiveFramerateMultiplier == 0.0f)
            {
                effectiveFadeFrames = musicFadeFrames;
            }
            else if (this->effectiveFramerateMultiplier > 1.0f)
            {
                effectiveFadeFrames = musicFadeFrames;
            }
            else
            {
                effectiveFadeFrames = musicFadeFrames / this->effectiveFramerateMultiplier;
            }
            // STRING: TH07 0x00496c1e
            g_SoundPlayer.PushCommand(AUDIO_FADEOUT, effectiveFadeFrames, "");
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

// FUNCTION: TH07 0x0043a18d
i32 Supervisor::IsSlowMode()
{
    return g_GameManager.defaultCfg != NULL &&
           g_GameManager.defaultCfg->slowMode;
}

// FUNCTION: TH07 0x0043a1bd
HRESULT Supervisor::EnableFog()
{
    g_AnmManager->Flush();
    // The renderer does not implement per-vertex fog, so this only tracks the flag the
    // stage code uses to decide whether to bother.
    this->fogEnabled = TRUE;
    return 0;
}

// FUNCTION: TH07 0x0043a207
HRESULT Supervisor::DisableFog()
{
    g_AnmManager->Flush();
    this->fogEnabled = FALSE;
    return 0;
}

#pragma var_order(time, timeSinceStartup)
// FUNCTION: TH07 0x0043a27f
void Supervisor::UpdateStartupTime()
{
    u32 timeSinceStartup;
    DWORD time;

    time = timeGetTime();
    if (time < this->lastTotalPlayTimeUpdate)
    {
        this->lastTotalPlayTimeUpdate = 0;
    }
    timeSinceStartup = time - this->lastTotalPlayTimeUpdate;
    g_GameManager.plst.totalHours += timeSinceStartup / 3600000;
    timeSinceStartup %= 3600000;
    g_GameManager.plst.totalMinutes += timeSinceStartup / 60000;
    timeSinceStartup %= 60000;
    g_GameManager.plst.totalSeconds += timeSinceStartup / 1000;
    timeSinceStartup %= 1000;
    g_GameManager.plst.totalMilliseconds += timeSinceStartup;
    if (g_GameManager.plst.totalMilliseconds >= 1000)
    {
        g_GameManager.plst.totalSeconds +=
            g_GameManager.plst.totalMilliseconds / 1000;
        g_GameManager.plst.totalMilliseconds %= 1000;
    }
    if (g_GameManager.plst.totalSeconds >= 60)
    {
        g_GameManager.plst.totalMinutes += g_GameManager.plst.totalSeconds / 60;
        g_GameManager.plst.totalSeconds %= 60;
    }
    if (g_GameManager.plst.totalMinutes >= 60)
    {
        g_GameManager.plst.totalHours += g_GameManager.plst.totalMinutes / 60;
        g_GameManager.plst.totalMinutes %= 60;
    }
    this->lastTotalPlayTimeUpdate = time;
}

#pragma var_order(time, timeSinceLastTime)
// FUNCTION: TH07 0x0043a3f4
void Supervisor::UpdateTime()
{
    u32 timeSinceLastTime;
    DWORD time;

    time = timeGetTime();
    if (time < this->currentTime)
    {
        this->currentTime = 0;
    }
    timeSinceLastTime = time - this->currentTime;
    g_GameManager.plst.gameHours += timeSinceLastTime / 3600000;
    timeSinceLastTime %= 3600000;
    g_GameManager.plst.gameMinutes += timeSinceLastTime / 60000;
    timeSinceLastTime %= 60000;
    g_GameManager.plst.gameSeconds += timeSinceLastTime / 1000;
    timeSinceLastTime %= 1000;
    g_GameManager.plst.gameMilliseconds += timeSinceLastTime;
    if (g_GameManager.plst.gameMilliseconds >= 1000)
    {
        g_GameManager.plst.gameSeconds +=
            g_GameManager.plst.gameMilliseconds / 1000;
        g_GameManager.plst.gameMilliseconds %= 1000;
    }
    if (g_GameManager.plst.gameSeconds >= 60)
    {
        g_GameManager.plst.gameMinutes += g_GameManager.plst.gameSeconds / 60;
        g_GameManager.plst.gameSeconds %= 60;
    }
    if (g_GameManager.plst.gameMinutes >= 60)
    {
        g_GameManager.plst.gameHours += g_GameManager.plst.gameMinutes / 60;
        g_GameManager.plst.gameMinutes %= 60;
    }
    this->currentTime = time;
}

#pragma var_order(versionData, versionDataExeChecksum, versionDataSize, oldPos, versionDataExeSize)
// FUNCTION: TH07 0x0043a569
ZunResult Supervisor::CheckIntegrity(const char *version, i32 exeSize,
                                     i32 exeChecksum)
{
#ifdef NON_MATCHING
    return ZUN_SUCCESS;
#else
    i32 versionDataExeSize;
    char *oldPos;
    i32 versionDataSize;
    i32 versionDataExeChecksum;
    char *versionData;

    if (!this->versionData)
    {
        return ZUN_SUCCESS;
    }

    versionData = this->versionData;
    versionDataSize = this->versionDataSize;
    // STRING: TH07 0x00496c18
    if (strncmp(version, "debug", 5) == 0)
    {
        return ZUN_SUCCESS;
    }

    if (strcmp("0100b", "debug") == 0)
    {
        return ZUN_SUCCESS;
    }

    while ((u32)versionDataSize > 0)
    {
        if (strncmp(version, versionData, 5) == 0)
        {
            versionData = versionData + 6;
            // STRING: TH07 0x00496c10
            sscanf(versionData, "%d %d", &versionDataExeSize, &versionDataExeChecksum);
            if (versionDataExeSize == exeSize && versionDataExeChecksum == exeChecksum)
            {
                return ZUN_SUCCESS;
            }
            return ZUN_ERROR;
        }
        oldPos = versionData;
        versionData = strchr(versionData, 10) + 1;
        versionDataSize -= (u8 *)versionData - (u8 *)oldPos;
    }
    return ZUN_ERROR;
#endif
}
