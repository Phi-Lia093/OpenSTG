// Desktop OpenGL 3.3 entry points for Windows.
//
// Windows ships opengl32.dll, whose header stops at GL 1.1, so everything the renderer
// uses beyond that has to be resolved at runtime through SDL_GL_GetProcAddress. SDL's
// SDL_opengl.h pulls in the system header and SDL_opengl_glext.h only supplies tokens,
// not function declarations, so the declarations and pointers live here.
//
// Included from Gles.hpp everywhere; Gles.cpp defines TH_GL_DECLARE_IMPLEMENTATION so the
// pointers are defined once and filled in by ThGlLoadFunctions().
//
// Note: glDrawArrays is GL 1.1 and exported by opengl32 directly, so it is not listed
// here; glActiveTexture is GL 1.3, which the SDK import library does not carry, so it
// is resolved like the rest.

#pragma once

#if defined(_WIN32)

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

// Tokens the 1.1 header is missing. Values are from the OpenGL registry.
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGBA8 0x8058
#define GL_BGRA 0x80E1
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_FUNC_ADD 0x8006
#define GL_MAX 0x8008
#define GL_MIN 0x8007
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_ALWAYS 0x0207
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_MIRRORED_REPEAT 0x8370
#define GL_REPEAT 0x2901
#define GL_GENERATE_MIPMAP 0x8191

// Every entry point beyond GL 1.1 that the renderer calls.
#define TH_GL_ENTRY_POINTS(X)                                                  \
    X(PFNGLGENBUFFERSPROC, glGenBuffers)                                        \
    X(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)                                  \
    X(PFNGLBINDBUFFERPROC, glBindBuffer)                                        \
    X(PFNGLBUFFERDATAPROC, glBufferData)                                        \
    X(PFNGLBUFFERSUBDATAPROC, glBufferSubData)                                  \
    X(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)                              \
    X(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)                        \
    X(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)                              \
    X(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)              \
    X(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray)            \
    X(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)                      \
    X(PFNGLVERTEXATTRIB4FPROC, glVertexAttrib4f)                                \
    X(PFNGLCREATESHADERPROC, glCreateShader)                                    \
    X(PFNGLDELETESHADERPROC, glDeleteShader)                                    \
    X(PFNGLSHADERSOURCEPROC, glShaderSource)                                    \
    X(PFNGLCOMPILESHADERPROC, glCompileShader)                                  \
    X(PFNGLGETSHADERIVPROC, glGetShaderiv)                                      \
    X(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)                            \
    X(PFNGLCREATEPROGRAMPROC, glCreateProgram)                                  \
    X(PFNGLDELETEPROGRAMPROC, glDeleteProgram)                                  \
    X(PFNGLATTACHSHADERPROC, glAttachShader)                                    \
    X(PFNGLLINKPROGRAMPROC, glLinkProgram)                                      \
    X(PFNGLGETPROGRAMIVPROC, glGetProgramiv)                                    \
    X(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)                          \
    X(PFNGLUSEPROGRAMPROC, glUseProgram)                                        \
    X(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)                        \
    X(PFNGLUNIFORM1IPROC, glUniform1i)                                          \
    X(PFNGLUNIFORM1FPROC, glUniform1f)                                          \
    X(PFNGLUNIFORM2FPROC, glUniform2f)                                          \
    X(PFNGLUNIFORM3FPROC, glUniform3f)                                          \
    X(PFNGLUNIFORM4FPROC, glUniform4f)                                          \
    X(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)                            \
    X(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)                                \
    X(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate)                          \
    X(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)                              \
    X(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)                        \
    X(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)                              \
    X(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)                    \
    X(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)                \
    X(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)                            \
    X(PFNGLDELETERENDERBUFFERSPROC, glDeleteRenderbuffers)                      \
    X(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)                            \
    X(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage)                      \
    X(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer)

#ifdef TH_GL_DECLARE_IMPLEMENTATION
#define TH_GL_FUNCTION(type, name) type name;
#else
#define TH_GL_FUNCTION(type, name) extern type name;
#endif

TH_GL_ENTRY_POINTS(TH_GL_FUNCTION)

#undef TH_GL_FUNCTION

// Resolves every pointer above through SDL. Returns false if the context lacks one,
// which means the driver did not give a 3.3 core context.
bool ThGlLoadFunctions();

// glActiveTexture is GL 1.3: the SDK header declares it but the import library does not
// export it, so it cannot be redeclared as a pointer under that name either. It gets its
// own pointer under a distinct name, and call sites go through ThGlActiveTexture.
typedef void(__stdcall *TH_PFNGLACTIVETEXTURE)(GLenum);

#ifdef TH_GL_DECLARE_IMPLEMENTATION
TH_PFNGLACTIVETEXTURE th_glActiveTexture = nullptr;
#else
extern TH_PFNGLACTIVETEXTURE th_glActiveTexture;
#endif

inline void ThGlActiveTexture(GLenum unit)
{
    if (th_glActiveTexture)
    {
        th_glActiveTexture(unit);
    }
}

#endif // _WIN32
