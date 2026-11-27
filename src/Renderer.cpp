// The renderer: SDL3 + OpenGL 3.3 in place of Direct3D 8.
//
// The game keeps drawing through the same D3D-shaped calls it always has; this file is
// the other side of those calls. Nothing here knows about Touhou, bullets or scripts.
//
// Conventions inherited from D3D8, which the game's coordinates depend on:
//   * Row vectors, row-major matrices: v' = v * M, so translation lives in row 3.
//   * Left-handed projection, depth 0..1, and the origin at the top-left with +y down.
//   * Colours are 0xAARRGGBB.
#include "Renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <vector>

// RenderDoc's in-application trigger. Loading it lets the game ask for a capture at a
// known frame instead of relying on the F12 hotkey, which is not usable when the test
// machine is driven from a script.
#include "renderdoc_app.h"

static RENDERDOC_API_1_6_0 *g_RenderDocApi;
static i32 g_CaptureFrame = -1;
static i32 g_FrameIndex;
static i32 g_DumpThisFrame;
// Milliseconds after the first frame at which to dump, for capture points that depend
// on how long the player spends in the menus.
static u32 g_DumpAfterMs;
static u32 g_DumpStartTicks;

// Per-frame counters, so a dump can show whether drawing was even attempted.
static u32 g_DrawCalls;
static u32 g_DrawnVertices;
static u32 g_SkippedNoTexture;
static u32 g_SkippedNoVerts;
// Bounds of everything submitted, in the game's 640x480 space.
static f32 g_MinX = 1e9f, g_MaxX = -1e9f, g_MinY = 1e9f, g_MaxY = -1e9f;

Renderer g_Renderer;

// --- matrix ----------------------------------------------------------------------

RenderMatrix *MatrixMultiply(RenderMatrix *out, const RenderMatrix *a,
                             const RenderMatrix *b)
{
    RenderMatrix result;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            f32 sum = 0.0f;
            for (int k = 0; k < 4; k++)
            {
                sum += a->m[row][k] * b->m[k][col];
            }
            result.m[row][col] = sum;
        }
    }
    *out = result;
    return out;
}

RenderMatrix *MatrixTranslation(RenderMatrix *out, f32 x, f32 y, f32 z)
{
    out->Identity();
    out->m[3][0] = x;
    out->m[3][1] = y;
    out->m[3][2] = z;
    return out;
}

RenderMatrix *MatrixScaling(RenderMatrix *out, f32 x, f32 y, f32 z)
{
    out->Identity();
    out->m[0][0] = x;
    out->m[1][1] = y;
    out->m[2][2] = z;
    return out;
}

RenderMatrix *MatrixRotationX(RenderMatrix *out, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    out->Identity();
    out->m[1][1] = c;
    out->m[1][2] = s;
    out->m[2][1] = -s;
    out->m[2][2] = c;
    return out;
}

RenderMatrix *MatrixRotationY(RenderMatrix *out, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    out->Identity();
    out->m[0][0] = c;
    out->m[0][2] = -s;
    out->m[2][0] = s;
    out->m[2][2] = c;
    return out;
}

RenderMatrix *MatrixRotationZ(RenderMatrix *out, f32 angle)
{
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    out->Identity();
    out->m[0][0] = c;
    out->m[0][1] = s;
    out->m[1][0] = -s;
    out->m[1][1] = c;
    return out;
}

RenderMatrix *MatrixLookAtLH(RenderMatrix *out, const f32 eye[3], const f32 at[3],
                             const f32 up[3])
{
    f32 zAxis[3] = {at[0] - eye[0], at[1] - eye[1], at[2] - eye[2]};
    f32 length = sqrtf(zAxis[0] * zAxis[0] + zAxis[1] * zAxis[1] + zAxis[2] * zAxis[2]);
    if (length == 0.0f)
    {
        out->Identity();
        return out;
    }
    zAxis[0] /= length;
    zAxis[1] /= length;
    zAxis[2] /= length;

    f32 xAxis[3] = {up[1] * zAxis[2] - up[2] * zAxis[1],
                    up[2] * zAxis[0] - up[0] * zAxis[2],
                    up[0] * zAxis[1] - up[1] * zAxis[0]};
    length = sqrtf(xAxis[0] * xAxis[0] + xAxis[1] * xAxis[1] + xAxis[2] * xAxis[2]);
    if (length == 0.0f)
    {
        out->Identity();
        return out;
    }
    xAxis[0] /= length;
    xAxis[1] /= length;
    xAxis[2] /= length;

    f32 yAxis[3] = {zAxis[1] * xAxis[2] - zAxis[2] * xAxis[1],
                    zAxis[2] * xAxis[0] - zAxis[0] * xAxis[2],
                    zAxis[0] * xAxis[1] - zAxis[1] * xAxis[0]};

    out->Identity();
    out->m[0][0] = xAxis[0];
    out->m[0][1] = yAxis[0];
    out->m[0][2] = zAxis[0];
    out->m[1][0] = xAxis[1];
    out->m[1][1] = yAxis[1];
    out->m[1][2] = zAxis[1];
    out->m[2][0] = xAxis[2];
    out->m[2][1] = yAxis[2];
    out->m[2][2] = zAxis[2];
    out->m[3][0] = -(xAxis[0] * eye[0] + xAxis[1] * eye[1] + xAxis[2] * eye[2]);
    out->m[3][1] = -(yAxis[0] * eye[0] + yAxis[1] * eye[1] + yAxis[2] * eye[2]);
    out->m[3][2] = -(zAxis[0] * eye[0] + zAxis[1] * eye[1] + zAxis[2] * eye[2]);
    return out;
}

RenderMatrix *MatrixPerspectiveFovLH(RenderMatrix *out, f32 fovY, f32 aspect,
                                     f32 zn, f32 zf)
{
    f32 height = 1.0f / tanf(fovY * 0.5f);
    f32 width = height / aspect;
    out->Identity();
    out->m[0][0] = width;
    out->m[1][1] = height;
    out->m[2][2] = zf / (zf - zn);
    out->m[2][3] = 1.0f;
    out->m[3][2] = -zn * zf / (zf - zn);
    return out;
}

// --- GL plumbing -----------------------------------------------------------------

// Minimal OpenGL type aliases. SDL3 does not ship a loader and the GL headers are not
// in the build, so everything the renderer needs is declared here.
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef unsigned char GLubyte;
typedef int GLint;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

// Entry point shapes. These live at file scope, not in Entry, so the LOAD macro can
// name them from anywhere.
typedef GLuint (__stdcall *PFN_glCreateShader)(GLenum);
typedef void (__stdcall *PFN_glShaderSource)(GLuint, GLsizei, const GLchar *const *, const GLint *);
typedef void (__stdcall *PFN_glCompileShader)(GLuint);
typedef void (__stdcall *PFN_glGetShaderiv)(GLuint, GLenum, GLint *);
typedef void (__stdcall *PFN_glGetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef GLuint (__stdcall *PFN_glCreateProgram)(void);
typedef void (__stdcall *PFN_glAttachShader)(GLuint, GLuint);
typedef void (__stdcall *PFN_glLinkProgram)(GLuint);
typedef void (__stdcall *PFN_glGetProgramiv)(GLuint, GLenum, GLint *);
typedef void (__stdcall *PFN_glGetProgramInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (__stdcall *PFN_glDeleteShader)(GLuint);
typedef void (__stdcall *PFN_glUseProgram)(GLuint);
typedef GLint (__stdcall *PFN_glGetUniformLocation)(GLuint, const GLchar *);
typedef void (__stdcall *PFN_glUniformMatrix4fv)(GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (__stdcall *PFN_glUniform1i)(GLint, GLint);
typedef void (__stdcall *PFN_glGenBuffers)(GLsizei, GLuint *);
typedef void (__stdcall *PFN_glBindBuffer)(GLenum, GLuint);
typedef void (__stdcall *PFN_glBufferData)(GLenum, GLsizeiptr, const void *, GLenum);
typedef void (__stdcall *PFN_glGenVertexArrays)(GLsizei, GLuint *);
typedef void (__stdcall *PFN_glBindVertexArray)(GLuint);
typedef void (__stdcall *PFN_glEnableVertexAttribArray)(GLuint);
typedef void (__stdcall *PFN_glVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei,
                                          const void *);
typedef void (__stdcall *PFN_glActiveTexture)(GLenum);
typedef void (__stdcall *PFN_glGenTextures)(GLsizei, GLuint *);
typedef void (__stdcall *PFN_glBindTexture)(GLenum, GLuint);
typedef void (__stdcall *PFN_glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum,
                                 GLenum, const void *);
typedef void (__stdcall *PFN_glTexParameteri)(GLenum, GLenum, GLint);
typedef void (__stdcall *PFN_glDeleteTextures)(GLsizei, const GLuint *);
typedef void (__stdcall *PFN_glDrawArrays)(GLenum, GLint, GLsizei);
typedef void (__stdcall *PFN_glViewport)(GLint, GLint, GLsizei, GLsizei);
typedef void (__stdcall *PFN_glClear)(GLbitfield);
typedef void (__stdcall *PFN_glClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (__stdcall *PFN_glEnable)(GLenum);
typedef void (__stdcall *PFN_glDisable)(GLenum);
typedef void (__stdcall *PFN_glBlendFunc)(GLenum, GLenum);
typedef void (__stdcall *PFN_glDepthMask)(GLboolean);
typedef void (__stdcall *PFN_glGenFramebuffers)(GLsizei, GLuint *);
typedef void (__stdcall *PFN_glBindFramebuffer)(GLenum, GLuint);
typedef void (__stdcall *PFN_glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
typedef void (__stdcall *PFN_glDeleteFramebuffers)(GLsizei, const GLuint *);
typedef const GLubyte *(__stdcall *PFN_glGetString)(GLenum);
typedef void (__stdcall *PFN_glCopyTexSubImage2D)(GLenum, GLint, GLint, GLint, GLint, GLint,
                                        GLsizei, GLsizei);
typedef void (__stdcall *PFN_glScissor)(GLint, GLint, GLsizei, GLsizei);
typedef void (__stdcall *PFN_glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *);

static void *GLProc(const char *name)
{
    return (void *)SDL_GL_GetProcAddress(name);
}

// The OpenGL 3.3 core entry points used here, loaded once at startup.
namespace Entry
{

PFN_glCreateShader glCreateShader;
PFN_glShaderSource glShaderSource;
PFN_glCompileShader glCompileShader;
PFN_glGetShaderiv glGetShaderiv;
PFN_glGetShaderInfoLog glGetShaderInfoLog;
PFN_glCreateProgram glCreateProgram;
PFN_glAttachShader glAttachShader;
PFN_glLinkProgram glLinkProgram;
PFN_glGetProgramiv glGetProgramiv;
PFN_glGetProgramInfoLog glGetProgramInfoLog;
PFN_glDeleteShader glDeleteShader;
PFN_glUseProgram glUseProgram;
PFN_glGetUniformLocation glGetUniformLocation;
PFN_glUniformMatrix4fv glUniformMatrix4fv;
PFN_glUniform1i glUniform1i;
PFN_glGenBuffers glGenBuffers;
PFN_glBindBuffer glBindBuffer;
PFN_glBufferData glBufferData;
PFN_glGenVertexArrays glGenVertexArrays;
PFN_glBindVertexArray glBindVertexArray;
PFN_glEnableVertexAttribArray glEnableVertexAttribArray;
PFN_glVertexAttribPointer glVertexAttribPointer;
PFN_glActiveTexture glActiveTexture;
PFN_glGenTextures glGenTextures;
PFN_glBindTexture glBindTexture;
PFN_glTexImage2D glTexImage2D;
PFN_glTexParameteri glTexParameteri;
PFN_glDeleteTextures glDeleteTextures;
PFN_glDrawArrays glDrawArrays;
PFN_glViewport glViewport;
PFN_glClear glClear;
PFN_glClearColor glClearColor;
PFN_glEnable glEnable;
PFN_glDisable glDisable;
PFN_glBlendFunc glBlendFunc;
PFN_glDepthMask glDepthMask;
PFN_glGenFramebuffers glGenFramebuffers;
PFN_glBindFramebuffer glBindFramebuffer;
PFN_glFramebufferTexture2D glFramebufferTexture2D;
PFN_glDeleteFramebuffers glDeleteFramebuffers;
PFN_glGetString glGetString;
PFN_glCopyTexSubImage2D glCopyTexSubImage2D;
PFN_glScissor glScissor;
PFN_glReadPixels glReadPixels;

#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TEXTURE_2D 0x0DE1
#define GL_TRIANGLES 0x0004
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058
#define GL_UNSIGNED_BYTE 0x1401
#define GL_FLOAT 0x1406
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_BLEND 0x0BE2
#define GL_DEPTH_TEST 0x0B71
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE 1
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_DEPTH_BUFFER_BIT 0x0100
#define GL_ARRAY_BUFFER 0x8892
#define GL_STREAM_DRAW 0x88E0
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_FRAMEBUFFER 0x8D40
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_VERSION 0x1F02
#define GL_SCISSOR_TEST 0x0C11

#define LOAD(name)                                                      \
    do                                                                  \
    {                                                                   \
        Entry::name = (PFN_##name)GLProc(#name);                        \
        if (!Entry::name)                                               \
        {                                                               \
            SDL_Log("renderer: missing GL entry point %s", #name);      \
            return false;                                               \
        }                                                               \
    } while (0)

}  // namespace Entry

static bool LoadGL()
{
    LOAD(glCreateShader);
    LOAD(glShaderSource);
    LOAD(glCompileShader);
    LOAD(glGetShaderiv);
    LOAD(glGetShaderInfoLog);
    LOAD(glCreateProgram);
    LOAD(glAttachShader);
    LOAD(glLinkProgram);
    LOAD(glGetProgramiv);
    LOAD(glGetProgramInfoLog);
    LOAD(glDeleteShader);
    LOAD(glUseProgram);
    LOAD(glGetUniformLocation);
    LOAD(glUniformMatrix4fv);
    LOAD(glUniform1i);
    LOAD(glGenBuffers);
    LOAD(glBindBuffer);
    LOAD(glBufferData);
    LOAD(glGenVertexArrays);
    LOAD(glBindVertexArray);
    LOAD(glEnableVertexAttribArray);
    LOAD(glVertexAttribPointer);
    LOAD(glActiveTexture);
    LOAD(glGenTextures);
    LOAD(glBindTexture);
    LOAD(glTexImage2D);
    LOAD(glTexParameteri);
    LOAD(glDeleteTextures);
    LOAD(glDrawArrays);
    LOAD(glViewport);
    LOAD(glClear);
    LOAD(glClearColor);
    LOAD(glEnable);
    LOAD(glDisable);
    LOAD(glBlendFunc);
    LOAD(glDepthMask);
    LOAD(glGenFramebuffers);
    LOAD(glBindFramebuffer);
    LOAD(glFramebufferTexture2D);
    LOAD(glDeleteFramebuffers);
    LOAD(glGetString);
    LOAD(glCopyTexSubImage2D);
    LOAD(glScissor);
    LOAD(glReadPixels);
    return true;
}

static const char *kVertexShader = R"(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 uViewProj;

out vec4 vColor;
out vec2 vUV;

void main()
{
    // Column-vector convention, matching GL's own matrix layout: clip = M * pos.
    gl_Position = uViewProj * vec4(aPos, 1.0);
    // The packed 0xAARRGGBB colour arrives as B, G, R, A bytes, so swap red and blue.
    vColor = aColor.bgra;
    vUV = aUV;
}
)";

// Two shading paths, matching the original's texture-stage configurations:
//
//   * textured: colour and alpha both come from texture * vertex colour, which is the
//     MODULATE setup used for every sprite.
//   * untextured: the vertex colour alone, which is SELECTARG1 with a diffuse argument,
//     used for solid fills such as the pause dimmer and screen flashes.
//
// Textures are stored as-is: the game's PNG/THTX data is non-premultiplied RGBA.
static const char *kFragmentShader = R"(#version 330 core
in vec4 vColor;
in vec2 vUV;

uniform sampler2D uTexture;
uniform int uUseTexture;

out vec4 fragColor;

void main()
{
    if (uUseTexture != 0)
    {
        fragColor = texture(uTexture, vUV) * vColor;
    }
    else
    {
        fragColor = vColor;
    }
}
)";

static GLuint CompileShader(GLenum type, const char *source)
{
    // Trace every attempt: the value returned and the GL error right after it. Written
    // and flushed per call so a crash cannot lose the record.
    typedef GLenum (__stdcall *PFN_glGetError)(void);
    GLuint shader = Entry::glCreateShader(type);
    if (!shader)
    {
        SDL_Log("renderer: glCreateShader failed (type 0x%X)", (unsigned)type);
        return 0;
    }
    Entry::glShaderSource(shader, 1, &source, NULL);
    Entry::glCompileShader(shader);
    GLint status = 0;
    Entry::glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        char log[4096];
        log[0] = '\0';
        Entry::glGetShaderInfoLog(shader, sizeof(log) - 1, NULL, log);
        log[sizeof(log) - 1] = '\0';
        SDL_Log("renderer: shader compile failed (type 0x%X): %s", (unsigned)type, log);
        return 0;
    }
    return shader;
}

static bool BuildPipeline()
{
    GLuint vertex = CompileShader(GL_VERTEX_SHADER, kVertexShader);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, kFragmentShader);
    if (!vertex || !fragment)
    {
        return false;
    }

    GLuint program = Entry::glCreateProgram();
    Entry::glAttachShader(program, vertex);
    Entry::glAttachShader(program, fragment);
    Entry::glLinkProgram(program);

    GLint status = 0;
    Entry::glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        char log[4096];
        log[0] = '\0';
        Entry::glGetProgramInfoLog(program, sizeof(log) - 1, NULL, log);
        log[sizeof(log) - 1] = '\0';
        SDL_Log("renderer: program link failed: %s", log);
        return false;
    }
    Entry::glDeleteShader(vertex);
    Entry::glDeleteShader(fragment);

    g_Renderer.program = program;
    g_Renderer.uViewProj = Entry::glGetUniformLocation(program, "uViewProj");
    g_Renderer.uUseTexture = Entry::glGetUniformLocation(program, "uUseTexture");

    Entry::glUseProgram(program);
    // The texture sampler always reads unit 0.
    Entry::glUniform1i(g_Renderer.uUseTexture, 0);

    Entry::glGenVertexArrays(1, &g_Renderer.vao);
    Entry::glBindVertexArray(g_Renderer.vao);
    Entry::glGenBuffers(1, &g_Renderer.vbo);
    Entry::glBindBuffer(GL_ARRAY_BUFFER, g_Renderer.vbo);

    GLsizei stride = sizeof(RenderVertex);
    Entry::glEnableVertexAttribArray(0);
    Entry::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                                 (const void *)0);
    // The colour is a packed 0xAARRGGBB word, so in little-endian memory the four bytes
    // are B, G, R, A. Read them as normalised unsigned bytes; the vertex shader swaps
    // the red and blue channels back.
    Entry::glEnableVertexAttribArray(1);
    Entry::glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride,
                                 (const void *)offsetof(RenderVertex, color));
    // The UVs come after the packed colour, so their offset is measured from the struct
    // rather than assumed: it moves whenever the colour field changes size.
    Entry::glEnableVertexAttribArray(2);
    Entry::glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                                 (const void *)offsetof(RenderVertex, u));
    return true;
}

// --- public API ------------------------------------------------------------------

bool RendererInitialize()
{
    // Bind to RenderDoc if it is running this process. Failure is expected in normal
    // play and simply means no capture is available.
    if (HMODULE renderdoc = GetModuleHandleA("renderdoc.dll"))
    {
        pRENDERDOC_GetAPI getApi =
            (pRENDERDOC_GetAPI)GetProcAddress(renderdoc, "RENDERDOC_GetAPI");
        if (getApi)
        {
            if (!getApi(eRENDERDOC_API_Version_1_6_0, (void **)&g_RenderDocApi))
            {
                g_RenderDocApi = NULL;
            }
        }
    }
    if (const char *captureAt = getenv("TH07_CAPTURE_FRAME"))
    {
        g_CaptureFrame = atoi(captureAt);
    }
    if (const char *dumpAfter = getenv("TH07_DUMP_AFTER_MS"))
    {
        g_DumpAfterMs = (u32)atoi(dumpAfter);
    }

    // A GL context is only usable on the thread it was made current on, and SDL does
    // not guarantee that the context created during window setup is still current by
    // the time the first GL call is made. Bind it explicitly: without this every call
    // fails with GL_INVALID_OPERATION and glCreateShader returns 0.
    if (g_Renderer.window)
    {
        if (!SDL_GL_MakeCurrent(g_Renderer.window, g_Renderer.context))
        {
            SDL_Log("renderer: SDL_GL_MakeCurrent failed: %s", SDL_GetError());
            return false;
        }
    }

    if (!LoadGL())
    {
        return false;
    }
    if (!BuildPipeline())
    {
        return false;
    }

    Entry::glEnable(GL_BLEND);
    Entry::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Entry::glDisable(GL_DEPTH_TEST);
    Entry::glDepthMask(GL_FALSE);

    g_Renderer.currentTexture = 0;
    g_Renderer.currentBlend = RENDER_BLEND_ALPHA;
    g_Renderer.zWriteDisabled = true;
    g_Renderer.viewportX = 0;
    g_Renderer.viewportY = 0;
    g_Renderer.viewportWidth = 640;
    g_Renderer.viewportHeight = 480;
    g_Renderer.viewMatrix.Identity();
    g_Renderer.projectionMatrix.Identity();
    return true;
}

void RendererSwap()
{
    // Read back before the swap: afterwards the back buffer holds undefined contents.
    if (g_DumpThisFrame)
    {
        g_DumpThisFrame = 0;
        g_CaptureFrame = -1;
        RendererDumpFrameState("frame_state.txt");

        // Start a fresh tally for the next frame.
        g_DrawCalls = 0;
        g_DrawnVertices = 0;
        g_SkippedNoTexture = 0;
        g_SkippedNoVerts = 0;
        g_MinX = g_MinY = 1e9f;
        g_MaxX = g_MaxY = -1e9f;
    }

    if (g_Renderer.window)
    {
        SDL_GL_SwapWindow(g_Renderer.window);
    }
}

void RendererShutdown()
{
    g_Renderer.window = NULL;
    g_Renderer.context = NULL;
}

void RendererSetViewport(i32 x, i32 y, i32 width, i32 height)
{
    g_Renderer.viewportX = x;
    g_Renderer.viewportY = y;
    g_Renderer.viewportWidth = width;
    g_Renderer.viewportHeight = height;
    Entry::glViewport(x, y, width, height);
}

void RendererClear(f32 r, f32 g, f32 b)
{
    Entry::glClearColor(r, g, b, 1.0f);
    Entry::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererSetMatrices(const RenderMatrix *view, const RenderMatrix *projection)
{
    g_Renderer.viewMatrix = *view;
    g_Renderer.projectionMatrix = *projection;
}

void RendererSetBlend(RenderBlend blend)
{
    if (g_Renderer.currentBlend == blend)
    {
        return;
    }
    g_Renderer.currentBlend = blend;
    if (blend == RENDER_BLEND_ADD)
    {
        Entry::glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        Entry::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

void RendererSetZWrite(bool enabled)
{
    // zWriteDisabled tracks the GL state as a ZunBool-ish integer; compare against the
    // boolean form to keep the types honest.
    if ((g_Renderer.zWriteDisabled != 0) == !enabled)
    {
        return;
    }
    g_Renderer.zWriteDisabled = enabled ? 0 : 1;
    Entry::glDepthMask(enabled ? GL_TRUE : GL_FALSE);
    if (enabled)
    {
        Entry::glEnable(GL_DEPTH_TEST);
    }
    else
    {
        Entry::glDisable(GL_DEPTH_TEST);
    }
}

void RendererBeginFrame()
{
    Entry::glUseProgram(g_Renderer.program);
    Entry::glBindVertexArray(g_Renderer.vao);

    // Ask RenderDoc for a capture once the requested frame is in flight. The comparison
    // is >= rather than == because BeginFrame runs once per render pass, so the counter
    // can step over the exact value that was asked for.
    if (g_CaptureFrame >= 0 && g_FrameIndex >= g_CaptureFrame)
    {
        if (g_RenderDocApi && g_RenderDocApi->TriggerCapture)
        {
            g_RenderDocApi->TriggerCapture();
        }
        g_DumpThisFrame = 1;
        g_CaptureFrame = -1;
    }

    // A second, time-based trigger: gameplay starts well after the loading screen and
    // its frame number is not predictable, so a wall-clock delay is easier to aim.
    if (g_DumpAfterMs > 0)
    {
        if (g_DumpStartTicks == 0)
        {
            g_DumpStartTicks = SDL_GetTicks();
        }
        if (SDL_GetTicks() - g_DumpStartTicks >= g_DumpAfterMs)
        {
            g_DumpThisFrame = 1;
            g_DumpAfterMs = 0;
        }
    }
    g_FrameIndex++;

    // Record where the frame counter has reached, so a dump that never appeared can be
    // told apart from a dump that fired at the wrong moment.
    // Do not reset the counters here: BeginFrame runs once per render pass, and a frame
    // has several (background, objects, GUI). They are reset after the dump instead, so
    // the totals cover the whole frame.

    // Maps the game's 640x480 pixel space to clip space. The shader multiplies as
    // clip = M * pos, so this is built directly in GL's column-major layout:
    // scale x into -1..1, flip y because the game counts downwards from the top, and
    // translate so the centre lands on the origin.
    f32 upload[16] = {
        2.0f / (f32)g_Renderer.viewportWidth, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f / (f32)g_Renderer.viewportHeight, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
    };
    Entry::glUniformMatrix4fv(g_Renderer.uViewProj, 1, GL_FALSE, upload);
}

void RendererDrawTriangles(const RenderVertex *vertices, i32 vertexCount,
                           RenderTexture *texture, bool useTexture)
{
    if (vertexCount <= 0)
    {
        return;
    }

    GLuint glTexture = 0;
    if (useTexture && texture)
    {
        glTexture = texture->glTexture;
    }
    else
    {
        useTexture = false;
    }

    // The sampler must be told which unit to read. GL leaves a sampler on unit 0 by
    // default, but relying on that is fragile, and the uniform is easy to set here.
    GLint uTexture = Entry::glGetUniformLocation(g_Renderer.program, "uTexture");
    if (uTexture != -1)
    {
        Entry::glUniform1i(uTexture, 0);
    }

    // Bind unconditionally. The cached-id shortcut is unsafe here: anything that changes
    // GL state between draws -- binding a texture for an upload, a blit helper, a
    // framebuffer switch -- can leave the sampler pointing elsewhere without the cache
    // knowing, and the skip then samples the wrong image.
    g_Renderer.currentTexture = glTexture;
    Entry::glActiveTexture(GL_TEXTURE0);
    Entry::glBindTexture(GL_TEXTURE_2D, glTexture);

    GLint uUse = Entry::glGetUniformLocation(g_Renderer.program, "uUseTexture");
    if (uUse != -1)
    {
        Entry::glUniform1i(uUse, useTexture ? 1 : 0);
    }

    // Record what this call actually submitted so a dump can show whether anything
    // reached the GPU and where it landed.
    g_DrawCalls++;
    g_DrawnVertices += (u32)vertexCount;
    if (!useTexture)
    {
        g_SkippedNoTexture++;
    }
    for (i32 i = 0; i < vertexCount; i++)
    {
        if (vertices[i].x < g_MinX) g_MinX = vertices[i].x;
        if (vertices[i].x > g_MaxX) g_MaxX = vertices[i].x;
        if (vertices[i].y < g_MinY) g_MinY = vertices[i].y;
        if (vertices[i].y > g_MaxY) g_MaxY = vertices[i].y;
    }

    Entry::glBindBuffer(GL_ARRAY_BUFFER, g_Renderer.vbo);
    Entry::glBufferData(GL_ARRAY_BUFFER, vertexCount * (GLsizeiptr)sizeof(RenderVertex),
                        vertices, GL_STREAM_DRAW);
    Entry::glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

// --- textures --------------------------------------------------------------------

RenderTexture *RenderTextureCreate(u32 width, u32 height, const void *rgba)
{
    RenderTexture *texture = new RenderTexture();
    texture->width = (i32)width;
    texture->height = (i32)height;
    texture->renderTargetFbo = 0;
    texture->pixels = NULL;

    Entry::glGenTextures(1, &texture->glTexture);
    Entry::glBindTexture(GL_TEXTURE_2D, texture->glTexture);
    Entry::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                           GL_LINEAR);
    Entry::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                           GL_LINEAR);
    Entry::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                           GL_CLAMP_TO_EDGE);
    Entry::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                           GL_CLAMP_TO_EDGE);
    Entry::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)width,
                        (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                        rgba);
    return texture;
}

void RenderTextureDestroy(RenderTexture *texture)
{
    if (!texture)
    {
        return;
    }
    if (texture->renderTargetFbo)
    {
        Entry::glDeleteFramebuffers(1, &texture->renderTargetFbo);
        texture->renderTargetFbo = 0;
    }
    if (texture->glTexture)
    {
        Entry::glDeleteTextures(1, &texture->glTexture);
        texture->glTexture = 0;
    }
    free(texture->pixels);
    delete texture;
}

void RenderTextureUpload(RenderTexture *texture, const void *rgba)
{
    if (!texture)
    {
        return;
    }
    Entry::glBindTexture(GL_TEXTURE_2D, texture->glTexture);
    Entry::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)texture->width,
                        (GLsizei)texture->height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, rgba);
    // The bind above changed which texture is current, so the renderer's idea of it is
    // stale from here on.
    g_Renderer.currentTexture = texture->glTexture;
}

// --- surfaces --------------------------------------------------------------------

RenderTexture *RenderSurfaceCreate(u32 width, u32 height)
{
    u32 bytes = width * height * 4;
    u8 *pixels = (u8 *)malloc(bytes);
    if (!pixels)
    {
        return NULL;
    }
    memset(pixels, 0, bytes);
    RenderTexture *texture = RenderTextureCreate(width, height, pixels);
    texture->pixels = pixels;
    return texture;
}

void RenderSurfaceCopy(RenderTexture *dst, i32 dstX, i32 dstY, RenderTexture *src,
                       i32 srcX, i32 srcY, i32 width, i32 height)
{
    if (!dst || !dst->pixels || !src || !src->pixels)
    {
        return;
    }
    // Clamp to both images so a stray rectangle cannot scribble past the end of a
    // buffer, which is the one thing a D3D CopyRects would silently refuse.
    for (i32 row = 0; row < height; row++)
    {
        i32 sy = srcY + row;
        i32 dy = dstY + row;
        if (sy < 0 || sy >= src->height || dy < 0 || dy >= dst->height)
        {
            continue;
        }
        i32 copyWidth = width;
        i32 sx = srcX;
        i32 dx = dstX;
        if (sx < 0)
        {
            copyWidth += sx;
            dx -= sx;
            sx = 0;
        }
        if (dx < 0)
        {
            copyWidth += dx;
            sx -= dx;
            dx = 0;
        }
        if (sx + copyWidth > src->width)
        {
            copyWidth = src->width - sx;
        }
        if (dx + copyWidth > dst->width)
        {
            copyWidth = dst->width - dx;
        }
        if (copyWidth <= 0)
        {
            continue;
        }
        memcpy(dst->pixels + ((size_t)dy * dst->width + dx) * 4,
               src->pixels + ((size_t)sy * src->width + sx) * 4, (size_t)copyWidth * 4);
    }
    RenderTextureUpload(dst, dst->pixels);
}

void RenderSurfaceBlitToScreen(RenderTexture *src, i32 dstX, i32 dstY, i32 srcX,
                               i32 srcY, i32 width, i32 height)
{
    if (!src)
    {
        return;
    }
    f32 u0 = (f32)srcX / (f32)src->width;
    f32 v0 = (f32)srcY / (f32)src->height;
    f32 u1 = (f32)(srcX + width) / (f32)src->width;
    f32 v1 = (f32)(srcY + height) / (f32)src->height;
    f32 x0 = (f32)dstX;
    f32 y0 = (f32)dstY;
    f32 x1 = (f32)(dstX + width);
    f32 y1 = (f32)(dstY + height);

    RenderVertex quad[6] = {
        {x0, y0, 0.0f, 0xffffffff, u0, v0}, {x1, y0, 0.0f, 0xffffffff, u1, v0},
        {x0, y1, 0.0f, 0xffffffff, u0, v1}, {x1, y0, 0.0f, 0xffffffff, u1, v0},
        {x1, y1, 0.0f, 0xffffffff, u1, v1}, {x0, y1, 0.0f, 0xffffffff, u0, v1},
    };
    RendererDrawTriangles(quad, 6, src, true);
}

void RendererReadPixels(u8 *rgba, i32 width, i32 height)
{
    // Read the framebuffer. GL returns rows bottom-up, so flip them into the caller's
    // top-down buffer while copying.
    u32 size = (u32)(width * height) * 4;
    u8 *temp = (u8 *)malloc(size);
    if (!temp)
    {
        return;
    }
    Entry::glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, temp);
    for (i32 row = 0; row < height; row++)
    {
        memcpy(rgba + (size_t)row * width * 4,
               temp + (size_t)(height - 1 - row) * width * 4, (size_t)width * 4);
    }
    free(temp);
}

// Reports what actually reached the framebuffer, plus the counters the renderer keeps.
// Written to a file at a fixed frame so a run can be inspected without a debugger.
void RendererDumpFrameState(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f)
    {
        return;
    }

    u8 *pixels = (u8 *)malloc((size_t)640 * 480 * 4);
    if (pixels)
    {
        RendererReadPixels(pixels, 640, 480);

        // Summarise the image: how many pixels differ from the first one, and the
        // range of values seen. A frame that was cleared shows one uniform colour.
        u32 first = *(u32 *)pixels;
        u32 different = 0;
        u32 minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;
        u32 opaque = 0;
        for (i32 i = 0; i < 640 * 480; i++)
        {
            u8 r = pixels[i * 4 + 0];
            u8 g = pixels[i * 4 + 1];
            u8 b = pixels[i * 4 + 2];
            u8 a = pixels[i * 4 + 3];
            if (*(u32 *)(pixels + i * 4) != first)
            {
                different++;
            }
            if (a != 0)
            {
                opaque++;
            }
            if (r < minR) minR = r;
            if (r > maxR) maxR = r;
            if (g < minG) minG = g;
            if (g > maxG) maxG = g;
            if (b < minB) minB = b;
            if (b > maxB) maxB = b;
        }

        fprintf(f, "first pixel      : %08x\n", first);
        fprintf(f, "pixels != first  : %u / %d\n", different, 640 * 480);
        fprintf(f, "opaque pixels    : %u\n", opaque);
        fprintf(f, "red   range      : %u..%u\n", minR, maxR);
        fprintf(f, "green range      : %u..%u\n", minG, maxG);
        fprintf(f, "blue  range      : %u..%u\n", minB, maxB);
        free(pixels);
    }

    fprintf(f, "program          : %u\n", g_Renderer.program);
    fprintf(f, "vao              : %u\n", g_Renderer.vao);
    fprintf(f, "vbo              : %u\n", g_Renderer.vbo);
    fprintf(f, "uViewProj        : %d\n", g_Renderer.uViewProj);
    fprintf(f, "uUseTexture      : %d\n", g_Renderer.uUseTexture);
    fprintf(f, "viewport         : %d,%d %dx%d\n", g_Renderer.viewportX,
            g_Renderer.viewportY, g_Renderer.viewportWidth, g_Renderer.viewportHeight);
    fprintf(f, "draw calls       : %u\n", g_DrawCalls);
    fprintf(f, "drawn vertices   : %u\n", g_DrawnVertices);
    fprintf(f, "untextured draws : %u\n", g_SkippedNoTexture);
    fprintf(f, "vertex x range   : %.1f .. %.1f\n", g_MinX, g_MaxX);
    fprintf(f, "vertex y range   : %.1f .. %.1f\n", g_MinY, g_MaxY);

    // The view and projection matrices decide whether the 3D background lands on screen
    // at all, so record them alongside the pixel summary.
    fprintf(f, "view matrix:\n");
    for (i32 r = 0; r < 4; r++)
    {
        fprintf(f, "  %8.3f %8.3f %8.3f %8.3f\n", g_Renderer.viewMatrix.m[r][0],
                g_Renderer.viewMatrix.m[r][1], g_Renderer.viewMatrix.m[r][2],
                g_Renderer.viewMatrix.m[r][3]);
    }
    fprintf(f, "projection matrix:\n");
    for (i32 r = 0; r < 4; r++)
    {
        fprintf(f, "  %8.3f %8.3f %8.3f %8.3f\n", g_Renderer.projectionMatrix.m[r][0],
                g_Renderer.projectionMatrix.m[r][1],
                g_Renderer.projectionMatrix.m[r][2],
                g_Renderer.projectionMatrix.m[r][3]);
    }
    fclose(f);
}

void RendererCaptureBackbuffer(RenderTexture *texture)
{
    if (!texture)
    {
        return;
    }
    Entry::glBindTexture(GL_TEXTURE_2D, texture->glTexture);
    // Copy the framebuffer straight into the texture, so the screenshot path costs one
    // GPU read instead of a full CPU round trip.
    Entry::glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (GLsizei)texture->width,
                               (GLsizei)texture->height);
}

u32 ConvertToRGBA8(const u8 *src, u8 *dst, u32 pixelCount, TextureFormat format,
                   RenderColor colorKey, bool hasColorKey)
{
    const u32 keyR = (colorKey >> 16) & 0xff;
    const u32 keyG = (colorKey >> 8) & 0xff;
    const u32 keyB = colorKey & 0xff;

    for (u32 i = 0; i < pixelCount; i++)
    {
        u32 r = 0, g = 0, b = 0, a = 255;

        switch (format)
        {
        case TEXTURE_FORMAT_ARGB8888:
        {
            // Stored little-endian as B, G, R, A.
            b = src[0];
            g = src[1];
            r = src[2];
            a = src[3];
            src += 4;
            break;
        }
        case TEXTURE_FORMAT_ARGB1555:
        {
            u16 pixel = (u16)(src[0] | (src[1] << 8));
            src += 2;
            a = (pixel & 0x8000) ? 255 : 0;
            r = (pixel >> 10) & 0x1f;
            g = (pixel >> 5) & 0x1f;
            b = pixel & 0x1f;
            r = (r << 3) | (r >> 2);
            g = (g << 3) | (g >> 2);
            b = (b << 3) | (b >> 2);
            break;
        }
        case TEXTURE_FORMAT_RGB565:
        {
            u16 pixel = (u16)(src[0] | (src[1] << 8));
            src += 2;
            r = (pixel >> 11) & 0x1f;
            g = (pixel >> 5) & 0x3f;
            b = pixel & 0x1f;
            r = (r << 3) | (r >> 2);
            g = (g << 2) | (g >> 4);
            b = (b << 3) | (b >> 2);
            break;
        }
        case TEXTURE_FORMAT_RGB888:
        {
            b = src[0];
            g = src[1];
            r = src[2];
            src += 3;
            break;
        }
        case TEXTURE_FORMAT_ARGB4444:
        {
            u16 pixel = (u16)(src[0] | (src[1] << 8));
            src += 2;
            a = (pixel >> 12) & 0xf;
            r = (pixel >> 8) & 0xf;
            g = (pixel >> 4) & 0xf;
            b = pixel & 0xf;
            a = (a << 4) | a;
            r = (r << 4) | r;
            g = (g << 4) | g;
            b = (b << 4) | b;
            break;
        }
        default:
            break;
        }

        // Colour-key transparency, the original's equivalent of D3DX's color-key
        // argument: match on RGB and drop alpha.
        if (hasColorKey && a == 255 && r == keyR && g == keyG && b == keyB)
        {
            a = 0;
        }

        dst[0] = (u8)r;
        dst[1] = (u8)g;
        dst[2] = (u8)b;
        dst[3] = (u8)a;
        dst += 4;
    }
    return pixelCount * 4;
}

u8 *DecodeImageToRGBA(const void *encoded, u32 encodedSize, i32 *width, i32 *height)
{
    // SDL_image reads from an SDL_IOStream, so wrap the memory without copying.
    SDL_IOStream *stream = SDL_IOFromConstMem(encoded, (size_t)encodedSize);
    if (!stream)
    {
        return NULL;
    }

    SDL_Surface *surface = IMG_Load_IO(stream, true);
    if (!surface)
    {
        SDL_Log("renderer: image decode failed: %s", SDL_GetError());
        return NULL;
    }

    SDL_Surface *converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);
    if (!converted)
    {
        return NULL;
    }

    *width = converted->w;
    *height = converted->h;
    size_t size = (size_t)converted->w * (size_t)converted->h * 4;
    u8 *rgba = (u8 *)malloc(size);
    if (!rgba)
    {
        SDL_DestroySurface(converted);
        return NULL;
    }

    // SDL surfaces are row-padded; copy row by row into a tight buffer.
    const u8 *src = (const u8 *)converted->pixels;
    u8 *dst = rgba;
    for (i32 row = 0; row < converted->h; row++)
    {
        memcpy(dst, src + (size_t)row * (size_t)converted->pitch, (size_t)converted->w * 4);
        dst += (size_t)converted->w * 4;
    }
    SDL_DestroySurface(converted);
    return rgba;
}

void RendererDrawColoredQuad(f32 left, f32 top, f32 right, f32 bottom,
                             RenderColor topLeft, RenderColor topRight,
                             RenderColor bottomLeft, RenderColor bottomRight)
{
    // Two triangles, no texture: the vertex colour is the fill.
    RenderVertex vertices[6] = {
        {left, top, 0.0f, topLeft, 0.0f, 0.0f},
        {right, top, 0.0f, topRight, 0.0f, 0.0f},
        {left, bottom, 0.0f, bottomLeft, 0.0f, 0.0f},
        {right, top, 0.0f, topRight, 0.0f, 0.0f},
        {right, bottom, 0.0f, bottomRight, 0.0f, 0.0f},
        {left, bottom, 0.0f, bottomLeft, 0.0f, 0.0f},
    };
    RendererDrawTriangles(vertices, 6, NULL, false);
}

void RendererClearRegion(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b)
{
    // Scissor to the region so everything outside it survives, then clear colour only.
    Entry::glEnable(GL_SCISSOR_TEST);
    Entry::glScissor(x, g_Renderer.viewportHeight - (y + height), width, height);
    Entry::glClearColor(r, g, b, 1.0f);
    Entry::glClear(GL_COLOR_BUFFER_BIT);
    Entry::glDisable(GL_SCISSOR_TEST);
}

void RendererClearDepth()
{
    Entry::glDepthMask(GL_TRUE);
    Entry::glClear(GL_DEPTH_BUFFER_BIT);
    Entry::glDepthMask(g_Renderer.zWriteDisabled ? GL_FALSE : GL_TRUE);
}
