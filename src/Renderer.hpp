// The renderer. Replaces Direct3D 8 with SDL3 + OpenGL 3.3.
//
// The original game talks to D3D through g_Supervisor.d3dDevice and a handful of
// D3DX helpers. Everything below is a deliberate drop-in for that surface, so the
// gameplay code -- which is correct and is not being rewritten -- keeps making the
// same calls and gets the same picture.
//
// Two conventions carry over from D3D8 and must not be "fixed", because the game's
// coordinates assume them:
//
//   * Origin is the top-left, +y points down, and the viewport is 640x480.
//   * The depth range is 0..1 with 1.0 being near, matching D3D.
//
// Textures are uploaded as RGBA8 and drawn with premultiplied-source alpha blending,
// which is what the original's texture-stage setup amounts to.
#pragma once

#include <SDL3/SDL.h>

#include <cmath>
#include <cstring>

#include "ZunBool.hpp"
#include "inttypes.hpp"

// --- matrix ----------------------------------------------------------------------
//
// Row-major 4x4, matching D3DXMATRIX's memory layout so existing indexing code such as
// `m[3][0] = vm->pos.x` keeps working unchanged. Vectors are row vectors: v * M.

struct RenderMatrix
{
    f32 m[4][4];

    void Identity()
    {
        memset(this, 0, sizeof(*this));
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
    }
};

RenderMatrix *MatrixMultiply(RenderMatrix *out, const RenderMatrix *a,
                             const RenderMatrix *b);
RenderMatrix *MatrixTranslation(RenderMatrix *out, f32 x, f32 y, f32 z);
RenderMatrix *MatrixScaling(RenderMatrix *out, f32 x, f32 y, f32 z);
RenderMatrix *MatrixRotationX(RenderMatrix *out, f32 angle);
RenderMatrix *MatrixRotationY(RenderMatrix *out, f32 angle);
RenderMatrix *MatrixRotationZ(RenderMatrix *out, f32 angle);
RenderMatrix *MatrixLookAtLH(RenderMatrix *out, const f32 eye[3], const f32 at[3],
                             const f32 up[3]);
RenderMatrix *MatrixPerspectiveFovLH(RenderMatrix *out, f32 fovY, f32 aspect,
                                     f32 zn, f32 zf);

// --- colour ----------------------------------------------------------------------
//
// The game stores colours as 0xAARRGGBB. The renderer converts to the float RGBA the
// shader wants, so keep the byte order here identical to D3DCOLOR.

typedef u32 RenderColor;
#define RENDER_COLOR_RGBA(r, g, b, a) \
    ((RenderColor)(((u32)(a) << 24) | ((u32)(r) << 16) | ((u32)(g) << 8) | (u32)(b)))
#define RENDER_COLOR_ALPHA(c) ((u32)((c) >> 24) & 0xff)

// --- textures --------------------------------------------------------------------

struct RenderTexture
{
    u32 glTexture;
    i32 width;
    i32 height;
    u32 renderTargetFbo;
    // CPU mirror of the image, RGBA8. Kept because the game copies pixels between
    // surfaces and takes screenshots, which GL cannot do directly without reading back.
    u8 *pixels;
};

RenderTexture *RenderTextureCreate(u32 width, u32 height, const void *rgba);
void RenderTextureDestroy(RenderTexture *texture);
void RenderTextureUpload(RenderTexture *texture, const void *rgba);

// A surface is a texture the game can write into: it holds the ending artwork and the
// GDI-rendered text images. Reading pixels back is a normal operation on it.
RenderTexture *RenderSurfaceCreate(u32 width, u32 height);
void RenderSurfaceCopy(RenderTexture *dst, i32 dstX, i32 dstY, RenderTexture *src,
                       i32 srcX, i32 srcY, i32 width, i32 height);
void RenderSurfaceBlitToScreen(RenderTexture *src, i32 dstX, i32 dstY, i32 srcX,
                               i32 srcY, i32 width, i32 height);

// Captures the current back buffer into a texture, which is how the game takes its
// title-screen and pause-menu screenshots.
void RendererCaptureBackbuffer(RenderTexture *texture);

// Reads the whole framebuffer into an RGBA8 buffer, top row first.
void RendererReadPixels(u8 *rgba, i32 width, i32 height);

// Writes a summary of the current framebuffer and renderer state to a file.
void RendererDumpFrameState(const char *path);

// THTX pixel formats, indexed the way the game indexes them. These are the formats
// that appear inside an ANM archive; none of them is RGBA8.
enum TextureFormat
{
    TEXTURE_FORMAT_UNKNOWN = 0,
    TEXTURE_FORMAT_ARGB8888 = 1,
    TEXTURE_FORMAT_ARGB1555 = 2,
    TEXTURE_FORMAT_RGB565 = 3,
    TEXTURE_FORMAT_RGB888 = 4,
    TEXTURE_FORMAT_ARGB4444 = 5,
};

// Converts a block of pixels in one of the formats above to RGBA8, which is the only
// format uploaded to GL. Returns the number of bytes written to dst.
u32 ConvertToRGBA8(const u8 *src, u8 *dst, u32 pixelCount, TextureFormat format,
                   RenderColor colorKey, bool hasColorKey);

// Decodes an encoded image (PNG, JPG) held in memory into a freshly allocated RGBA8
// buffer, using SDL_image. Out parameters receive the dimensions. Returns NULL on
// failure; the caller frees the result with free().
u8 *DecodeImageToRGBA(const void *encoded, u32 encodedSize, i32 *width, i32 *height);

// --- text ------------------------------------------------------------------------
//
// Glyph rasterisation with stb_truetype, which reads a TrueType file directly and needs
// no platform text API. That keeps text rendering working the same way everywhere,
// unlike the GDI path this replaces.
//
// A face is loaded once and caches its glyphs; drawing composes into a caller-supplied
// RGBA8 buffer, which the game then uploads as a sprite.

struct FontFace;

// Loads a TrueType/OpenType file. Returns NULL if the file is missing or unreadable.
FontFace *FontLoad(const char *path);
void FontFree(FontFace *face);

// Chooses the face used when a caller does not name one.
void FontSetDefault(FontFace *face);
FontFace *FontGetDefault();

// Measures a UTF-8 string at the given pixel height. Returns the advance width.
i32 FontMeasureText(FontFace *face, const char *utf8, f32 pixelHeight);

// Distance from the top of a line to its baseline, in pixels.
i32 FontAscent(FontFace *face, f32 pixelHeight);

// Draws a UTF-8 string into an RGBA8 buffer of the given size.
//
// Each glyph is rasterised, then blended with an optional outline drawn by stamping the
// glyph in every direction first, which is how the original produced its bold outlined
// look. Returns the number of pixels the text advanced horizontally.
i32 FontDrawText(FontFace *face, u8 *rgba, i32 bufferWidth, i32 bufferHeight,
                 i32 penX, i32 penY, const char *utf8, f32 pixelHeight,
                 RenderColor fillColor, RenderColor outlineColor, bool hasOutline);

// --- text overlay -----------------------------------------------------------------
//
// Text is drawn on top of the finished frame rather than being routed through the
// sprite batcher. The game composes text into ANM sprites, but those sprites share a
// handful of atlas textures that are rewritten every frame, so text kept going missing
// under the artwork; a separate layer that is composited last is immune to that.
//
// Call TextOverlayBegin at the start of a frame, add strings with TextOverlayDraw, then
// TextOverlayFlush once everything else has been drawn. Coordinates are in the 640x480
// game space with +y pointing down.

void TextOverlayBegin();
void TextOverlayDraw(const char *utf8, i32 x, i32 y, f32 pixelHeight, RenderColor color,
                     RenderColor outlineColor, bool hasOutline);
void TextOverlayFlush();
bool TextOverlayIsEmpty();

// --- vertices --------------------------------------------------------------------
//
// Position in screen space after projection, plus colour and UV. This is the
// D3DFVF_TEX1|D3DFVF_DIFFUSE|D3DFVF_XYZRHW vertex the sprite batcher already fills in.

struct RenderVertex
{
    f32 x, y, z;
    RenderColor color;
    f32 u, v;
};

// --- renderer --------------------------------------------------------------------

enum RenderBlend
{
    RENDER_BLEND_ALPHA = 0,  // src*alpha + dst*(1-alpha), the original's blend mode 0
    RENDER_BLEND_ADD = 1,    // src*alpha + dst, the original's blend mode 1
};

struct Renderer
{
    SDL_Window *window;
    SDL_GLContext context;

    f32 clearR, clearG, clearB;
    ZunBool clearDepth;

    // Viewport in pixels; the game runs at a fixed 640x480.
    i32 viewportX, viewportY, viewportWidth, viewportHeight;

    RenderMatrix viewMatrix;
    RenderMatrix projectionMatrix;

    // Current GL state, tracked to avoid redundant uniform and texture calls.
    u32 currentTexture;
    RenderBlend currentBlend;
    ZunBool zWriteDisabled;
    u32 program;
    u32 vbo;
    u32 vao;
    i32 uViewProj;
    i32 uUseTexture;
};
extern Renderer g_Renderer;

bool RendererInitialize(); 
void RendererShutdown();
void RendererSwap();
void RendererClear(f32 r, f32 g, f32 b);
// Clears only the colour buffer for a sub-rectangle, which is how the stage clears the
// playfield without disturbing what is already drawn around it.
void RendererClearRegion(i32 x, i32 y, i32 width, i32 height, f32 r, f32 g, f32 b);
void RendererClearDepth();
void RendererSetViewport(i32 x, i32 y, i32 width, i32 height);
void RendererSetMatrices(const RenderMatrix *view, const RenderMatrix *projection);
void RendererSetBlend(RenderBlend blend);
void RendererSetZWrite(bool enabled);
void RendererBeginFrame();

// Draws a run of triangle-list vertices in one batch. `texture` may be null, in which
// case the vertex colour is used directly (the original's SELECTARG1 path).
void RendererDrawTriangles(const RenderVertex *vertices, i32 vertexCount,
                           RenderTexture *texture, bool useTexture);

// Screen-space coloured quad, with each corner given its own colour. This is the
// original's DrawSquare / DrawColoredQuad: a solid fill used for flashes, fades and
// the pause dimmer. Coordinates are in the 640x480 space; +y points down.
void RendererDrawColoredQuad(f32 left, f32 top, f32 right, f32 bottom,
                             RenderColor topLeft, RenderColor topRight,
                             RenderColor bottomLeft, RenderColor bottomRight);
