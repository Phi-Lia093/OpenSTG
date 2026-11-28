#pragma once

// The renderer: desktop OpenGL 3.3 on Windows.
//
// The upstream port used OpenGL ES so it could also reach Android, iOS and the web. Those
// targets are gone; this build is Windows only and uses desktop OpenGL, so the shaders are
// GLSL 3.30 core with no precision qualifiers.
//
// Windows ships opengl32.dll, whose header stops at OpenGL 1.1. Everything the renderer calls
// beyond that is resolved at runtime through SDL_GL_GetProcAddress: the declarations and the
// tokens the 1.1 header lacks are below, GL.cpp defines the pointers and fills them in once
// the context exists.
//
// glDrawArrays is 1.1 and exported directly, so it is not listed. glActiveTexture is 1.3,
// which the import library does not carry, so it is resolved under its own name.

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

#include "core/inttypes.hpp"
#include "render/ZunGraphics.hpp"
#include "script/AnmManager.hpp"

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



struct CachedState
{
    void Invalidate()
    {
        dirtyMatrix = true;
        dirtyFog = true;
        dirtyViewport = true;
        dirtyColorOp = true;
        dirtyTexArg = true;
        dirtyTexFactor = true;
        dirtyAlphaTest = true;

        currentStride = -1;
        currentVao = 0xFFFFFFFF;
    }
    bool dirtyMatrix = true;
    bool dirtyFog = true;
    bool dirtyViewport = true;
    bool dirtyColorOp = true;
    bool dirtyTexArg = true;
    bool dirtyTexFactor = true;
    bool dirtyAlphaTest = true;

    i32 currentStride = -1;
    GLuint currentVao = 0xFFFFFFFF;
};

class GLGraphics : public ZunGraphics
{
  public:
    static ZunGraphics *Init();

    ~GLGraphics() override
    {
        Exit();
    }

    void Exit() override;

    RendererType GetType() override
    {
        return RENDERER_OPENGL;
    }
    void BeginFrame() override;
    void EndFrame() override;
    void SetFogRange(f32 nearPlane, f32 farPlane) override;
    void SetFogColor(ZunColor color) override;
    void SetColorOp(TextureOpComponent component, ColorOp op) override;
    void SetTextureFactor(ZunColor factor) override;
    void SetTransformMatrix(TransformMatrix type, const ZunMatrix &matrix) override;
    void SetTextureFilter() override;

    void GetViewport(ZunViewport &viewport) override;
    void SetViewport(const ZunViewport &viewport) override;

    void Enable(Capabilities cap) override;
    void Disable(Capabilities cap) override;

    void SetBlendMode(BlendMode srcMode, BlendMode dstMode) override;
    void SetDepthMask(bool enable) override;
    void SetDepthFunc(DepthFunc func) override;

    void SetTextureArg(TextureArg arg) override;

    void SetClearDepth(f32 depth) override;
    void SetClearColor(ZunColor color) override;
    void SetAlphaTestRef(u8 ref) override;
    void Clear(u32 clearBits) override;

    GfxTextureHandle CreateTexture() override;
    void BindTexture(GfxTextureHandle handle) override;
    void DeleteTexture(GfxTextureHandle handle) override;
    void SetTextureImage(u32 width, u32 height, PixelFormat fmt, PixelDataType type,
                         const void *data) override;
    void SetTextureSubImage(i32 xoffset, i32 yoffset, i32 width, i32 height,
                            const void *data) override;

    void ReadPixels(i32 x, i32 y, i32 width, i32 height, void *pixels) override;
    void DrawPrimitive(PrimitiveType type, i32 startVertex, i32 primitiveCount) override;
    void DrawPrimitiveUP(PrimitiveType type, i32 primitiveCount, const void *vertexData,
                         i32 vertexStride) override;

    void SwapBuffers() override;

  private:
    SDL_GLContext ctx;
    u32 shaderProgram;
    u32 vaos[3][3];
    u32 vbos[3];
    u32 curVbo = 0xFFFFFFFF;
    u32 blitProgram;
    u32 blitVao;
    u8 alphaRef = 0;
    TextureArg texArg = TEX_ARG_DIFFUSE;
    ZunColor textureFactor = {0xFFFFFFFF};
    ColorOp colorOpRgb = COLOR_OP_MODULATE;
    ColorOp colorOpAlpha = COLOR_OP_MODULATE;

    static constexpr size_t VBO_CAPACITY = 1024 * 1024;
    size_t vboOffset = 0;

    GLuint defaultFbo = 0;
    GLuint fbo = 0;
    GLuint fboColor = 0;
    GLuint fboDepth = 0;

    GLuint unitQuadVao = 0;
    GLuint unitQuadVbo = 0;

    ZunMatrix transforms[4];
    ZunViewport viewport;
    bool fogEnabled = false;
    f32 fogNear = 0.0f;
    f32 fogFar = 1.0f;
    ZunColor fogColor = {0};
    ZunColor clearColor = {0};
    bool blendEnabled = false;
    bool depthTestEnabled = false;
    bool alphaTestEnabled = false;
    bool depthMaskEnabled = true;

    GLint u_Model, u_View, u_Proj, u_TextureMatrix;
    GLint u_ScreenSpace, u_Viewport;
    GLint u_UseTexture, u_Texture;
    GLint u_ColorOpRgb, u_ColorOpAlpha, u_TexArg, u_TextureFactor;
    GLint u_AlphaTest, u_AlphaRef;
    GLint u_FogEnabled, u_FogColor, u_FogNear, u_FogFar;
    GLint u_BlitTexture = -1;

    CachedState stateCache;

    u64 prevTicks = 0;

    void Flush()
    {
        if (g_AnmManager && g_AnmManager->spritesToDraw != 0)
        {
            g_AnmManager->Flush();
        }
    }

    static GLuint CompileShader(GLenum type, const char *source)
    {
        u32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (!isCompiled)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            Supervisor::DebugPrint("shader compile error: %s\n", log);
            return 0;
        }
        return shader;
    }
};
