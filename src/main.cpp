#include <direct.h>
#include <shlobj.h>
#include <stdio.h>
#include <windows.h>
#include <winnls32.h>

// clang-format keeps on ordering gameerrorcontext below anmmanager
// clang-format off
#include "Global.hpp"
#include "Renderer.hpp"
#include "AnmManager.hpp"
#include "BulletManager.hpp"
#include "GameManager.hpp"
#include "ResultScreen.hpp"
#include "ScreenEffect.hpp"
#include "SoundPlayer.hpp"
#include "Stage.hpp"
#include "Supervisor.hpp"
#include "ZunResult.hpp"
#include "dxutil.hpp"
#include "i18n.hpp"
// clang-format on

enum RenderResult
{
    RENDER_RESULT_EXIT_SUCCESS_2 = -1,
    RENDER_RESULT_KEEP_RUNNING = 0,
    RENDER_RESULT_EXIT_SUCCESS = 1,
    RENDER_RESULT_EXIT_ERROR = 2
};

#pragma pack(4)
struct GameWindow
{
    static ZunResult CheckForRunningGameInstance(HINSTANCE hInstance);
    static i32 ChecksumExecutable();
    static i32 CreateGameWindow(HINSTANCE hInstance);
    static char *FormatCapability(const char *capabilityName, u32 capabilityFlags,
                                  u32 mask, char *buf);
    static i32 InitD3dInterface();
    static i32 InitD3dRendering();
    static void Present();
    RenderResult Render();
    static void ResetRenderState();
    static ZunBool ResolveIt(const char *shortcutPath, char *dstPath, i32 maxPathLen);
    static void SetWindowActive(HWND window);
    static LRESULT __stdcall WindowProc(HWND hWnd, u32 uMsg, WPARAM wParam,
                                        LPARAM lParam);

    SDL_Window *window;
    ZunBool isAppClosing;
    ZunBool isAppActive;
    ZunBool isAppInactive;
    i8 curFrame;
    // pad 3
    LARGE_INTEGER lpFrequency;
    bool usesRelativePath;
    // pad 3
    u32 screenSaveActive;
    u32 lowPowerActive;
    u32 powerOffActive;
};
C_ASSERT(sizeof(GameWindow) == 0x2c);

// GLOBAL: TH07 0x00575c20
GameWindow g_GameWindow;

// GLOBAL: TH07 0x0135e1f4
HANDLE g_Mutex;

// GLOBAL: TH07 0x0135e1f8
i32 g_FrameCount;

// GLOBAL: TH07 0x0135e200
f64 g_LastFrameTime;

// GLOBAL: TH07 0x0135e208
LARGE_INTEGER g_LastPerfCounter;

#pragma var_order(d3dDeviceStatus, msg, res, i)
// FUNCTION: TH07 0x00434020
i32 WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdline, i32 nCmdShow)
{
    HRESULT d3dDeviceStatus;
    i32 res;
    tagMSG msg;

    res = RENDER_RESULT_KEEP_RUNNING;
    g_Supervisor.hInstance = hInstance;
    SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0,
                          &g_GameWindow.screenSaveActive, 0);
    SystemParametersInfoA(SPI_GETLOWPOWERACTIVE, 0,
                          &g_GameWindow.lowPowerActive, 0);
    SystemParametersInfoA(SPI_GETPOWEROFFACTIVE, 0,
                          &g_GameWindow.powerOffActive, 0);
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, NULL, 2);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, 0, NULL, 2);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, 0, NULL, 2);
    if (GameWindow::CheckForRunningGameInstance(hInstance) == ZUN_ERROR)
    {
        goto stop;
    }

    // STRING: TH07 0x00497c60
    if (g_Supervisor.LoadConfig("th07.cfg") != ZUN_SUCCESS)
    {
        goto stop;
    }

    GameWindow::ChecksumExecutable();
    QueryPerformanceFrequency(&g_GameWindow.lpFrequency);

start:
    if (GameWindow::InitD3dInterface())
    {
        goto stop;
    }

    if (GameWindow::CreateGameWindow(hInstance))
    {
        goto stop;
    }

    if (GameWindow::InitD3dRendering())
    {
        goto stop;
    }

    // SDL audio does not need the window handle the way DirectSound did.
    g_SoundPlayer.InitializeDSound(NULL);
    Controller::GetJoystickCaps();
    Controller::ResetKeyboard();
    g_AnmManager = ZUN_NEW(AnmManager, "SprtCtrlInf");
    if (!g_Supervisor.cfg.windowed)
    {
        WINNLSEnableIME(0, 0);
        ShowCursor(0);
    }
    res = g_Supervisor.RegisterChain();
    if (res != ZUN_SUCCESS)
    {
        if (res == ZUN_ERROR)
        {
            goto cleanup;
        }
        res = RENDER_RESULT_EXIT_ERROR;
        goto cleanup;
    }
    res = RENDER_RESULT_KEEP_RUNNING;
    g_GameWindow.curFrame = -30;
    while (!g_GameWindow.isAppClosing)
    {
        SDL_Event event;
        ZunBool hadEvent = FALSE;

        // Drain the event queue. The device-lost dance the original performed has no
        // equivalent: an OpenGL context is not revocable the way a D3D device is.
        while (SDL_PollEvent(&event))
        {
            hadEvent = TRUE;
            if (event.type == SDL_EVENT_QUIT)
            {
                g_GameWindow.isAppClosing = TRUE;
            }
            else if (event.type == SDL_EVENT_WINDOW_FOCUS_LOST)
            {
                g_GameWindow.isAppActive = FALSE;
                g_GameWindow.isAppInactive = TRUE;
            }
            else if (event.type == SDL_EVENT_WINDOW_FOCUS_GAINED)
            {
                g_GameWindow.isAppActive = TRUE;
                g_GameWindow.isAppInactive = FALSE;
            }
        }

        if (g_GameWindow.isAppClosing)
        {
            break;
        }

        res = g_GameWindow.Render();
        if (res != RENDER_RESULT_KEEP_RUNNING)
        {
            break;
        }
        g_Supervisor.deviceNotReset = 0;
        (void)hadEvent;
    }
cleanup:
    if (g_GameManager.plst.magic != 0)
    {
        ResultScreen::RegisterChain(2);
    }
    g_Chain.Release();
    while (g_SoundPlayer.ProcessQueues())
        ;

stop:
    g_SoundPlayer.Release();
    ZUN_DELETE(g_AnmManager);

    if (g_Renderer.context)
    {
        SDL_GL_DestroyContext(g_Renderer.context);
        g_Renderer.context = NULL;
    }
    if (g_GameWindow.window)
    {
        SDL_DestroyWindow(g_GameWindow.window);
        g_GameWindow.window = NULL;
    }
    ShowCursor(1);
    if (res == RENDER_RESULT_EXIT_ERROR)
    {
        g_GameErrorContext.m_BufferEnd = g_GameErrorContext.m_Buffer;
        *g_GameErrorContext.m_BufferEnd = NULL;
        g_GameErrorContext.Log(TH_LOG_RESTARTING);
        if (!g_Supervisor.cfg.windowed)
        {
            WINNLSEnableIME(0, 1);
        }
        i32 i = 0;
        while (i < 60)
        {
            if (PeekMessageA(&msg, NULL, 0, 0, 1))
            {
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }
            i++;
        }
        goto start;
    }
    FileSystem::WriteDataToFile("th07.cfg", &g_Supervisor.cfg,
                                sizeof(GameConfiguration));
    SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE,
                          g_GameWindow.screenSaveActive, NULL, 2);
    SystemParametersInfoA(SPI_SETLOWPOWERACTIVE, g_GameWindow.lowPowerActive,
                          NULL, 2);
    SystemParametersInfoA(SPI_SETPOWEROFFACTIVE, g_GameWindow.powerOffActive,
                          NULL, 2);
    WINNLSEnableIME(0, 1);
    g_GameErrorContext.Flush();
    return 0;
}

// FUNCTION: TH07 0x00434490
LRESULT __stdcall GameWindow::WindowProc(HWND hWnd, u32 uMsg, WPARAM wParam,
                                         LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ERASEBKGND:
        return 1;
    case MM_MOM_DONE:
        if (g_Supervisor.midiOutput)
        {
            g_Supervisor.midiOutput->UnprepareHeader((LPMIDIHDR)lParam);
        }
        break;
    case WM_ACTIVATEAPP:
        g_GameWindow.isAppActive = wParam;
        if (g_GameWindow.isAppActive)
        {
            g_GameWindow.isAppInactive = FALSE;
        }
        else
        {
            g_GameWindow.isAppInactive = TRUE;
        }
        break;
    case WM_SETCURSOR:
        if (!g_Supervisor.cfg.windowed)
        {
            if (g_GameWindow.isAppInactive)
            {
                SetCursor(LoadCursorA(NULL, IDC_ARROW));
                ShowCursor(1);
            }
            else
            {
                ShowCursor(0);
                SetCursor(NULL);
            }
        }
        else
        {
            SetCursor(LoadCursorA(NULL, IDC_ARROW));
            ShowCursor(1);
        }
        return 1;
    case WM_CLOSE:
        g_GameWindow.isAppClosing = TRUE;
        return 1;
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

#pragma var_order(i, snapshotPath)
// FUNCTION: TH07 0x004345c0
void GameWindow::Present()
{
    char snapshotPath[252];
    i32 i;

    g_AnmManager->TakeScreenshotIfRequested();
    if (WAS_PRESSED_RAW(TH_BUTTON_HOME))
    {
        // STRING: TH07 0x00497c1c
        _mkdir("snapshot");
        for (i = 0; i < 1000; i++)
        {
            // STRING: TH07 0x00497c08
            sprintf(snapshotPath, "snapshot/th%.3d.bmp", i);
            if (FileSystem::CheckFileExists(snapshotPath) == 0)
            {
                break;
            }
        }
        if (i < 1000)
        {
            g_Supervisor.TakeSnapshot(snapshotPath);
        }
    }
    if (g_Supervisor.renderSkipFrames != 0)
    {
        g_Supervisor.renderSkipFrames--;
    }

    // This is where the finished frame reaches the screen. The original ended here with
    // IDirect3DDevice8::Present; the swap replaces it, and without it nothing drawn
    // this frame is ever displayed.
    RendererSwap();

    // Hold the frame at 60 Hz. The original relied on Present blocking for the vertical
    // blank, which does not happen on every driver -- on this machine the loop runs at
    // 120 fps. Sleeping until the next 1/60 s boundary reproduces the intended rate
    // regardless of what the driver does.
    {
        static LARGE_INTEGER lastFrameTime;
        static LARGE_INTEGER perfFrequency;

        if (perfFrequency.QuadPart == 0)
        {
            QueryPerformanceFrequency(&perfFrequency);
            QueryPerformanceCounter(&lastFrameTime);
        }
        else
        {
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);

            // Time left until the next frame is due, in performance-counter ticks.
            LONGLONG frameTicks = perfFrequency.QuadPart / 60;
            LONGLONG elapsed = now.QuadPart - lastFrameTime.QuadPart;
            LONGLONG remaining = frameTicks - elapsed;

            if (remaining > 0)
            {
                // Sleep takes milliseconds, so convert and leave the sub-millisecond
                // remainder to be absorbed by the next frame.
                DWORD sleepMs = (DWORD)(remaining * 1000 / perfFrequency.QuadPart);
                if (sleepMs > 0)
                {
                    Sleep(sleepMs);
                }
                QueryPerformanceCounter(&now);
            }

            lastFrameTime.QuadPart = now.QuadPart;
        }
    }
}

#pragma var_order(chainRes, perfCounter, perfDiff, curTime, timeDiff)
// FUNCTION: TH07 0x004346e0
RenderResult GameWindow::Render()
{
    f64 timeDiff;
    f64 curTime;
    f64 perfDiff;
    // The x86 API writes two DWORDs; the target local is DWORD-aligned.
    struct CounterSample { DWORD LowPart; LONG HighPart; } perfCounter;
    i32 chainRes;
    ZunBool clearedThisFrame = FALSE;

    if (!this->isAppActive)
    {
        return RENDER_RESULT_KEEP_RUNNING;
    }

    // The frame body below is re-entered with `goto begin_loop` while the frame's time
    // budget allows, exactly as the original did. Note that curFrame starts negative
    // and the loop runs whenever it is at or below the frameskip setting.
begin_loop:
    {
        // Start every frame from a known buffer. After a swap the back buffer's contents
        // are undefined, so anything the game does not repaint this frame -- a region
        // outside a menu's background blit, or a transparent part of it -- would show
        // leftovers from an older frame and flicker between two images.
        if (!clearedThisFrame)
        {
            RendererClear(0.0f, 0.0f, 0.0f);
            TextOverlayBegin();
            clearedThisFrame = TRUE;
        }
    }
    if ((i32)g_Supervisor.cfg.frameskipConfig <= (i32)this->curFrame)
    {
        RendererBeginFrame();
        g_AnmManager->ResetVertexBuffer();
        g_Supervisor.fogEnabled = 255;
        g_Supervisor.DisableFog();
        g_Chain.RunDrawChain();
        g_AnmManager->Flush();

        // The text layer goes on last, over everything the game just drew.
        TextOverlayFlush();
    }

    g_AnmManager->Flush();
    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = GAME_WINDOW_WIDTH;
    g_Supervisor.viewport.Height = GAME_WINDOW_HEIGHT;
    RendererSetViewport((i32)g_Supervisor.viewport.X, (i32)g_Supervisor.viewport.Y,
                        (i32)g_Supervisor.viewport.Width,
                        (i32)g_Supervisor.viewport.Height);

    chainRes = g_Chain.RunCalcChain();
    g_SoundPlayer.ProcessQueues();

    if (!chainRes)
    {
        return RENDER_RESULT_EXIT_SUCCESS;
    }
    if (chainRes == -1)
    {
        return RENDER_RESULT_EXIT_ERROR;
    }

    this->curFrame++;

    if (g_Supervisor.cfg.windowed || g_Supervisor.VsyncDisabled())
    {
        if (this->curFrame != 0)
        {
            if (g_GameWindow.lpFrequency.LowPart != 0)
            {
                QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&perfCounter));

                // The reference point starts at zero, so the first frame after a reset
                // would measure the whole time since boot and spin the catch-up loop for
                // thousands of iterations. Start from "now" instead.
                if (g_LastPerfCounter.LowPart == 0 && g_LastPerfCounter.HighPart == 0)
                {
                    g_LastPerfCounter.LowPart = perfCounter.LowPart;
                    g_LastPerfCounter.HighPart = perfCounter.HighPart;
                }

                perfDiff = (f64)(perfCounter.LowPart - g_LastPerfCounter.LowPart) / (f64)g_GameWindow.lpFrequency.LowPart;

                if (perfDiff < 0.0)
                {
                    g_LastPerfCounter.LowPart = perfCounter.LowPart;
                    g_LastPerfCounter.HighPart = perfCounter.HighPart;
                }

                if (perfDiff >= 1.0 / 60.0 || g_GameWindow.usesRelativePath)
                {
                    while (perfDiff >= 1.0 / 60.0)
                    {
                        g_LastPerfCounter.LowPart += g_GameWindow.lpFrequency.LowPart / 60;
                        perfDiff -= 1.0 / 60.0;
                    }
                    if ((i32)g_Supervisor.cfg.frameskipConfig < (i32)this->curFrame)
                    {
                        goto LAB_00434a18;
                    }
                    goto begin_loop;
                }

                // Not enough time has passed for the next frame. The original reached
                // this point inside Direct3D's present, which blocked until the
                // vertical blank; with an explicit swap there is nothing to block on,
                // so sleep the remainder instead of spinning a core flat out.
                if (perfDiff > 0.0005)
                {
                    Sleep((DWORD)((1.0 / 60.0 - perfDiff) * 1000.0));
                }
            }
            else
            {
                timeBeginPeriod(1);
                curTime = (f64)timeGetTime();

                if (curTime < g_LastFrameTime)
                {
                    g_LastFrameTime = curTime;
                }

                timeDiff = fabs(curTime - g_LastFrameTime);
                timeEndPeriod(1);

                if (timeDiff >= 50.0 / 3.0 || g_GameWindow.usesRelativePath)
                {
                    while (timeDiff >= 50.0 / 3.0)
                    {
                        g_LastFrameTime += 50.0 / 3.0;
                        timeDiff -= 50.0 / 3.0;
                    }
                    if ((i32)g_Supervisor.cfg.frameskipConfig < (i32)this->curFrame)
                    {
                        goto LAB_00434a18;
                    }
                    goto begin_loop;
                }
            }
        }
    }

    if (!g_Supervisor.cfg.windowed && !g_Supervisor.VsyncDisabled())
    {
        if ((i32)g_Supervisor.cfg.frameskipConfig >= (i32)this->curFrame)
        {
            Present();
            goto begin_loop;
        }

    LAB_00434a18:
        Present();
        this->curFrame = 0;
        g_FrameCount++;
    }
    else
    {
        // Windowed and vsync-disabled runs reach this point with a completed frame but
        // never pass through Present, which is where the buffers are swapped. Showing
        // it here keeps one swap per frame for those modes too.
        Present();
        this->curFrame = 0;
        g_FrameCount++;
    }

    return RENDER_RESULT_KEEP_RUNNING;
}

// FUNCTION: TH07 0x00434a40
i32 GameWindow::InitD3dInterface()
{
    // SDL replaces the Direct3D interface creation. The window itself is created by
    // CreateGameWindow, which needs the video subsystem up first.
    // Video, audio and gamepad are brought up together. Audio used to be initialised
    // lazily from the sound manager, but SDL tracks each subsystem separately and the
    // lazily-initialised one did not report as available.
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD |
                  SDL_INIT_JOYSTICK))
    {
        g_GameErrorContext.Fatal(TH_ERR_D3D_CREATE_FAIL);
        return true;
    }
    return false;
}

// FUNCTION: TH07 0x00434a80
i32 GameWindow::CreateGameWindow(HINSTANCE hInstance)
{
    Uint32 flags = SDL_WINDOW_OPENGL;
    g_GameWindow.isAppActive = TRUE;
    g_GameWindow.isAppInactive = FALSE;

    (void)hInstance;

    // Match the original's window policy: a fixed-size window unless the config asks
    // for fullscreen, and a 640x480 back buffer either way.
    if (!g_Supervisor.cfg.windowed)
    {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

    g_GameWindow.window = SDL_CreateWindow(TH_WINDOW_TITLE, GAME_WINDOW_WIDTH,
                                           GAME_WINDOW_HEIGHT, flags);
    if (!g_GameWindow.window)
    {
        g_GameErrorContext.Fatal(TH_ERR_D3D_CREATE_FAIL);
        return true;
    }

    g_Renderer.window = g_GameWindow.window;
    g_Renderer.context = SDL_GL_CreateContext(g_GameWindow.window);
    if (!g_Renderer.context)
    {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(g_GameWindow.window);
        g_GameWindow.window = NULL;
        return true;
    }

    // The swap interval is applied once the context is current. Setting it here, before
    // the renderer loads its entry points, silently did nothing and left the game
    // running unthrottled.
    SDL_GL_MakeCurrent(g_GameWindow.window, g_Renderer.context);
    SDL_GL_SetSwapInterval(1);

    SDL_SetWindowPosition(g_GameWindow.window, SDL_WINDOWPOS_CENTERED,
                          SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(g_GameWindow.window);

    // The old HWND is still used as a handle by a couple of call sites; keep it in
    // sync with the SDL window's native handle so those stay valid.
    g_Supervisor.hwndGameWindow = (HWND)SDL_GetPointerProperty(
        SDL_GetWindowProperties(g_GameWindow.window),
        SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);

    return false;
}

// FUNCTION: TH07 0x00434bd0
i32 GameWindow::InitD3dRendering()
{
    f32 fov;
    f32 aspectRatio;
    f32 halfWidth;
    f32 halfHeight;
    f32 halfCameraDistance;
    D3DXVECTOR3 pEye;
    D3DXVECTOR3 pAt;
    D3DXVECTOR3 pUp;

    // SDL and the GL context already exist; this sets up the renderer on top of them.
    if (!RendererInitialize())
    {
        g_GameErrorContext.Fatal(TH_ERR_D3D_INIT_FAIL);
        return 1;
    }

    // The original queried capabilities and downgraded settings when the hardware could
    // not cope. GL 3.3 has all of these unconditionally, so the checks are recorded as
    // satisfied rather than probed.
    g_Supervisor.usingTnLHal = 1;
    g_Supervisor.supports32BitTex = 1;
    g_Supervisor.hasLockableBackbuffer = 1;
    g_Supervisor.lockableBackBuffer = TRUE;
    g_Supervisor.cfg.colorAddEmulation = 0;
    g_Supervisor.cfg.noVertexBuffers = 0;
    g_Supervisor.cfg.use16BitTextures = 0;
    g_Supervisor.cfg.disableTextureBlend = 0;
    g_Supervisor.cfg.disableGouraud = 0;
    g_Supervisor.cfg.disableZBuffer = 0;
    g_Supervisor.cfg.disableFog = 0;

    // Same camera the original built: a 30 degree vertical field of view, looking at the
    // centre of the 640x480 playfield from far enough back to contain it.
    halfWidth = 320.0f;
    halfHeight = 240.0f;
    aspectRatio = 4.0f / 3.0f;
    fov = ZUN_PI / 6.0f;
    halfCameraDistance = halfHeight / tanf(fov / 2.0f);
    pUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    pAt = D3DXVECTOR3(halfWidth, -halfHeight, 0.0f);
    pEye = D3DXVECTOR3(halfWidth, -halfHeight, -halfCameraDistance);
    D3DXMatrixLookAtLH(&g_Supervisor.viewMatrix, &pEye, &pAt, &pUp);
    D3DXMatrixPerspectiveFovLH(&g_Supervisor.projectionMatrix, fov, aspectRatio, 100.0f,
                               10000.0f);

    g_Supervisor.viewport.X = 0;
    g_Supervisor.viewport.Y = 0;
    g_Supervisor.viewport.Width = GAME_WINDOW_WIDTH;
    g_Supervisor.viewport.Height = GAME_WINDOW_HEIGHT;
    g_Supervisor.viewport.MinZ = 0.0f;
    g_Supervisor.viewport.MaxZ = 1.0f;

    RendererSetViewport(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    RendererSetMatrices(&g_Supervisor.viewMatrix, &g_Supervisor.projectionMatrix);
    return 0;
}

// FUNCTION: TH07 0x004351c0
char *GameWindow::FormatCapability(const char *capabilityName,
                                   u32 capabilityFlags, u32 mask, char *buf)
{
    buf += sprintf(buf, capabilityName);
    if ((capabilityFlags & mask) == 0)
    {
        buf += sprintf(buf, TH_CAP_UNSUPPORTED);
    }
    else
    {
        buf += sprintf(buf, TH_CAP_SUPPORTED);
    }
    return buf;
}


// FUNCTION: TH07 0x004356a0
void GameWindow::ResetRenderState()
{
    // Most of what this did was fixed-function pipeline setup: lighting, culling,
    // shading mode, fog, alpha test, texture stages. The renderer has none of that --
    // it has one shader, alpha blending and optional depth writes -- so what remains is
    // the handful of choices that still mean something.
    RendererSetBlend(RENDER_BLEND_ALPHA);
    RendererSetZWrite(g_Supervisor.cfg.disableZBuffer ? false : true);

    // The game's caches are stale after a reset; make the next draw reconfigure.
    if (g_AnmManager)
    {
        g_AnmManager->SetVertexShader(255);
        g_AnmManager->SetSprite(NULL);
        g_AnmManager->SetTexture(NULL);
        g_AnmManager->SetColorOp(255);
        g_AnmManager->SetBlendMode(255);
        g_AnmManager->SetZWriteDisable(255);
    }
}

#pragma var_order(exePath, startupInfo, resolvedPath, ext)
// FUNCTION: TH07 0x00435bd0
ZunResult GameWindow::CheckForRunningGameInstance(HINSTANCE hInstance)
{
    char *ext;
    char resolvedPath[264];
    STARTUPINFO startupInfo;
    char exePath[264];

    // STRING: TH07 0x0049732c
    g_Mutex = CreateMutexA(NULL, 1, "Touhou YouYouMu App");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        g_GameErrorContext.Fatal(TH_ERR_ALREADY_OPEN);
        return ZUN_ERROR;
    }

    startupInfo.cb = sizeof(startupInfo);
    memset(&startupInfo.lpReserved, 0, sizeof(startupInfo) - 4);
    GetModuleFileNameA(NULL, exePath, 0x105);
    GetConsoleTitleA(resolvedPath, 0x105);
    GetStartupInfoA(&startupInfo);
    if (startupInfo.lpTitle)
    {
        ext = strrchr(startupInfo.lpTitle, '.');
        if (FileSystem::CheckFileExists(startupInfo.lpTitle) &&
            ext)
        {
            // STRING: TH07 0x0049730c
            if (_stricmp(ext, ".lnk") == 0)
            {
                do
                {
                    ResolveIt(startupInfo.lpTitle, resolvedPath, 0x104);
                    ext = strrchr(resolvedPath, '.');
                } while (_stricmp(ext, ".lnk") == 0);
            }
            else
            {
                strcpy(resolvedPath, startupInfo.lpTitle);
            }

            if (strcmp(exePath, resolvedPath) != 0)
            {
                g_GameWindow.usesRelativePath = true;
            }
        }
    }
    if (!g_Mutex)
    {
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

#pragma var_order(processId, param, idAttachTo)
// FUNCTION: TH07 0x00435e30
void GameWindow::SetWindowActive(HWND window)
{
    DWORD idAttachTo;
    void *param;
    DWORD processId;

    idAttachTo = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    processId = GetWindowThreadProcessId(window, NULL);
    AttachThreadInput(processId, idAttachTo, 1);
    SystemParametersInfoA(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &param, 0);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, NULL, 0);

    // ZUN landmine: ZUN here opts to use SetActiveWindow to bring input focus
    // onto the game window rather than SetForegroundWindow. This is quite
    // unusual since it's intended to be used only for already foregrounded
    // windows, which might not always be the case if the application decides
    // to launch in the background.
    // This normally works fine since applications do launch in the foreground,
    // but for some reason on Wine it doesn't work at all, leading to broken
    // input.
    // This is fixed when you use vpatch btw
#ifdef NON_MATCHING
    SetForegroundWindow(window);
#else
    SetActiveWindow(window);
#endif
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &param, 0);
    AttachThreadInput(processId, idAttachTo, 0);
}

#pragma var_order(filename, i, dataCursor, checksum, dataBase)
// FUNCTION: TH07 0x00435ec0
i32 GameWindow::ChecksumExecutable()
{
    u32 *dataBase;
    u32 checksum;
    u32 *dataCursor;
    u32 i;
    char filename[MAX_PATH + 1];

    if (GetModuleFileNameA(NULL, filename, 0x105))
    {
        checksum = 0;
        dataBase = dataCursor = (u32 *)FileSystem::OpenFile(filename, 1);
        if (!dataCursor)
        {
            return -1;
        }

        for (i = 0; i < g_LastFileSize / 4 - 1; i++, dataCursor++)
        {
            checksum += *dataCursor;
        }
        // STRING: TH07 0x004972fc
        utils::DebugPrint("main sum %d\r\n", checksum);
        ZUN_FREE(dataBase);
        g_Supervisor.exeChecksum = checksum;
        g_Supervisor.exeSize = g_LastFileSize;
        return checksum;
    }

    return -1;
}

#pragma var_order(hr, ret, psl, ppf, wPath, wfd)
// FUNCTION: TH07 0x00435fc0
ZunBool GameWindow::ResolveIt(const char *shortcutPath, char *dstPath,
                              i32 maxPathLen)
{
    WIN32_FIND_DATAA wfd;
    LPWSTR wPath;
    IPersistFile *ppf;
    IShellLinkA *psl;
    i32 ret;
    HRESULT hr;

    if (!dstPath)
    {
        return 0;
    }

    ret = 0;
    CoInitialize(NULL);
    if (SUCCEEDED(hr = CoCreateInstance(CLSID_ShellLink, NULL,
                                        CLSCTX_INPROC_SERVER, IID_IShellLink,
                                        (void **)&psl)))
    {
        if (SUCCEEDED(hr = psl->QueryInterface(IID_IPersistFile,
                                               (void **)&ppf)))
        {
            wPath = new WCHAR[maxPathLen];
            if (SUCCEEDED(hr))
            {
                MultiByteToWideChar(CP_ACP, 0, shortcutPath, -1, wPath,
                                    maxPathLen);
                if (SUCCEEDED(hr = ppf->Load(wPath, STGM_READ)))
                {
                    if (SUCCEEDED(hr = psl->GetPath(dstPath, maxPathLen, &wfd,
                                                    0)))
                    {
                        ret = 1;
                    }
                }
            }
            delete wPath;
            ppf->Release();
        }
        psl->Release();
    }
    CoUninitialize();
    return ret;
}
