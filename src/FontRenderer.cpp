// Text rendering with stb_truetype.
//
// The original composed text with GDI, which is Windows-only and depends on the system
// code page. This replaces it with a TrueType rasteriser that takes UTF-8 directly and
// behaves identically on every platform, while keeping the outlined look the game's
// text has: an outline colour stamped around each glyph and a fill colour on top.

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <SDL3/SDL.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Renderer.hpp"

struct FontFace
{
    // The file contents stay resident: stb_truetype reads glyph outlines straight out of
    // this buffer, so it must outlive the face.
    unsigned char *fileData;
    stbtt_fontinfo info;
    f32 scaleForPixelHeight;
    f32 cachedPixelHeight;
};

static FontFace *g_DefaultFont = NULL;

FontFace *FontLoad(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        SDL_Log("font: cannot open %s", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (size <= 0)
    {
        fclose(file);
        return NULL;
    }

    unsigned char *data = (unsigned char *)malloc((size_t)size);
    if (!data)
    {
        fclose(file);
        return NULL;
    }
    if (fread(data, 1, (size_t)size, file) != (size_t)size)
    {
        free(data);
        fclose(file);
        return NULL;
    }
    fclose(file);

    FontFace *face = new FontFace();
    face->fileData = data;
    face->cachedPixelHeight = 0.0f;
    face->scaleForPixelHeight = 0.0f;

    // Offset 0 is where a plain TrueType file keeps its first face; .ttc collections pack
    // several, and index 0 is the one callers normally want.
    int offset = stbtt_GetFontOffsetForIndex(data, 0);
    if (offset < 0 || !stbtt_InitFont(&face->info, data, offset))
    {
        SDL_Log("font: cannot parse %s", path);
        free(data);
        delete face;
        return NULL;
    }

    SDL_Log("font: loaded %s", path);
    return face;
}

void FontFree(FontFace *face)
{
    if (!face)
    {
        return;
    }
    if (g_DefaultFont == face)
    {
        g_DefaultFont = NULL;
    }
    free(face->fileData);
    delete face;
}

void FontSetDefault(FontFace *face)
{
    g_DefaultFont = face;
}

FontFace *FontGetDefault()
{
    return g_DefaultFont;
}

static f32 FontScaleFor(FontFace *face, f32 pixelHeight)
{
    if (pixelHeight < 1.0f)
    {
        pixelHeight = 1.0f;
    }
    if (face->cachedPixelHeight != pixelHeight)
    {
        face->scaleForPixelHeight = stbtt_ScaleForPixelHeight(&face->info, pixelHeight);
        face->cachedPixelHeight = pixelHeight;
    }
    return face->scaleForPixelHeight;
}

// Reads one UTF-8 sequence and advances the cursor. Returns the code point.
static u32 DecodeUtf8(const char **cursor)
{
    const unsigned char *p = (const unsigned char *)*cursor;
    u32 codePoint;
    i32 extraBytes;

    if (p[0] < 0x80)
    {
        *cursor += 1;
        return p[0];
    }
    if ((p[0] & 0xE0) == 0xC0)
    {
        codePoint = p[0] & 0x1F;
        extraBytes = 1;
    }
    else if ((p[0] & 0xF0) == 0xE0)
    {
        codePoint = p[0] & 0x0F;
        extraBytes = 2;
    }
    else if ((p[0] & 0xF8) == 0xF0)
    {
        codePoint = p[0] & 0x07;
        extraBytes = 3;
    }
    else
    {
        // Invalid lead byte: step over it so the loop always makes progress.
        *cursor += 1;
        return 0xFFFD;
    }

    for (i32 i = 1; i <= extraBytes; i++)
    {
        if ((p[i] & 0xC0) != 0x80)
        {
            *cursor += 1;
            return 0xFFFD;
        }
        codePoint = (codePoint << 6) | (p[i] & 0x3F);
    }
    *cursor += extraBytes + 1;
    return codePoint;
}

i32 FontAscent(FontFace *face, f32 pixelHeight)
{
    if (!face)
    {
        return 0;
    }
    int ascent = 0, descent = 0, lineGap = 0;
    stbtt_GetFontVMetrics(&face->info, &ascent, &descent, &lineGap);
    return (i32)((f32)ascent * FontScaleFor(face, pixelHeight));
}

// --- text overlay -----------------------------------------------------------------
//
// One full-screen RGBA layer, cleared at the start of each frame and composited over
// everything else at the end. Keeping it separate from the game's own textures means the
// artwork can be redrawn any number of times without erasing text, which is what kept
// happening while text lived in the sprite atlas.

struct OverlayString
{
    const char *utf8;
    i32 x, y;
    f32 pixelHeight;
    RenderColor color;
    RenderColor outlineColor;
    bool hasOutline;
};

static u8 *g_OverlayPixels = NULL;
static i32 g_OverlayWidth = 640;
static i32 g_OverlayHeight = 480;
static RenderTexture *g_OverlayTexture = NULL;
static std::vector<OverlayString> g_OverlayStrings;

void TextOverlayBegin()
{
    g_OverlayStrings.clear();

    if (!g_OverlayPixels)
    {
        g_OverlayPixels =
            (u8 *)malloc((size_t)g_OverlayWidth * g_OverlayHeight * 4);
        if (!g_OverlayPixels)
        {
            return;
        }
    }
    memset(g_OverlayPixels, 0,
           (size_t)g_OverlayWidth * g_OverlayHeight * 4);

    if (!g_OverlayTexture)
    {
        g_OverlayTexture = RenderTextureCreate(g_OverlayWidth, g_OverlayHeight, NULL);
    }
}

void TextOverlayDraw(const char *utf8, i32 x, i32 y, f32 pixelHeight, RenderColor color,
                     RenderColor outlineColor, bool hasOutline)
{
    if (!utf8 || !g_OverlayPixels)
    {
        return;
    }
    // Strings are copied because callers commonly pass a stack buffer that is gone by the
    // time the layer is composited.
    OverlayString item;
    size_t length = strlen(utf8);
    char *copy = (char *)malloc(length + 1);
    if (!copy)
    {
        return;
    }
    memcpy(copy, utf8, length + 1);
    item.utf8 = copy;
    item.x = x;
    item.y = y;
    item.pixelHeight = pixelHeight;
    item.color = color;
    item.outlineColor = outlineColor;
    item.hasOutline = hasOutline;
    g_OverlayStrings.push_back(item);
}

bool TextOverlayIsEmpty()
{
    return g_OverlayStrings.empty();
}

void TextOverlayFlush()
{
    if (g_OverlayStrings.empty() || !g_OverlayPixels || !g_OverlayTexture)
    {
        return;
    }

    FontFace *face = FontGetDefault();
    if (!face)
    {
        return;
    }

    // Compose every queued string into the layer. y is the top of the line, so the
    // baseline sits an ascent below it.
    for (size_t i = 0; i < g_OverlayStrings.size(); i++)
    {
        const OverlayString &item = g_OverlayStrings[i];
        i32 baseline = item.y + FontAscent(face, item.pixelHeight);
        FontDrawText(face, g_OverlayPixels, g_OverlayWidth, g_OverlayHeight,
                     item.x, baseline, item.utf8, item.pixelHeight, item.color,
                     item.outlineColor, item.hasOutline);
        free((void *)item.utf8);
    }
    g_OverlayStrings.clear();

    RenderTextureUpload(g_OverlayTexture, g_OverlayPixels);
    RenderSurfaceBlitToScreen(g_OverlayTexture, 0, 0, 0, 0, g_OverlayWidth,
                              g_OverlayHeight);
}

i32 FontMeasureText(FontFace *face, const char *utf8, f32 pixelHeight)
{
    if (!face || !utf8)
    {
        return 0;
    }
    f32 scale = FontScaleFor(face, pixelHeight);
    i32 advance = 0;
    i32 kerning = 0;
    const char *cursor = utf8;

    while (*cursor)
    {
        u32 codePoint = DecodeUtf8(&cursor);
        if (codePoint == 0)
        {
            break;
        }
        int advanceWidth = 0;
        int leftBearing = 0;
        int nextCodePoint = (u32)(unsigned char)*cursor;
        stbtt_GetCodepointHMetrics(&face->info, (int)codePoint, &advanceWidth,
                                   &leftBearing);
        (void)leftBearing;
        advance += (i32)((f32)advanceWidth * scale);
        if (nextCodePoint)
        {
            kerning = stbtt_GetCodepointKernAdvance(&face->info, (int)codePoint,
                                                    (int)nextCodePoint);
            advance += (i32)((f32)kerning * scale);
        }
    }
    return advance;
}

// Blends one coverage value into an RGBA8 pixel using source-over.
static void BlendPixel(u8 *dst, u8 r, u8 g, u8 b, u8 coverage)
{
    if (coverage == 0)
    {
        return;
    }
    u32 srcAlpha = coverage;
    u32 dstAlpha = dst[3];
    u32 outAlpha = srcAlpha + dstAlpha * (255 - srcAlpha) / 255;
    if (outAlpha == 0)
    {
        return;
    }
    // Colour is composited in premultiplied terms so antialiased edges blend cleanly
    // against whatever is already in the buffer.
    for (i32 channel = 0; channel < 3; channel++)
    {
        u32 src = channel == 0 ? r : (channel == 1 ? g : b);
        u32 dstColor = dst[channel];
        dst[channel] = (u8)((src * srcAlpha + dstColor * dstAlpha * (255 - srcAlpha) / 255) /
                            outAlpha);
    }
    dst[3] = (u8)outAlpha;
}

// Stamps every glyph of the string in one colour. Used for the outline, which is what
// gives the original's text its heavy bordered edge.
static void StampString(FontFace *face, u8 *rgba, i32 bufferWidth, i32 bufferHeight,
                        i32 penX, i32 penY, const char *utf8, f32 pixelHeight,
                        u8 r, u8 g, u8 b)
{
    f32 scale = FontScaleFor(face, pixelHeight);
    const char *cursor = utf8;
    i32 x = penX;

    while (*cursor)
    {
        u32 codePoint = DecodeUtf8(&cursor);
        if (codePoint == 0)
        {
            break;
        }

        int advanceWidth = 0;
        int leftBearing = 0;
        stbtt_GetCodepointHMetrics(&face->info, (int)codePoint, &advanceWidth,
                                   &leftBearing);

        int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
        stbtt_GetCodepointBitmapBox(&face->info, (int)codePoint, scale, scale, &x0, &y0,
                                    &x1, &y1);

        i32 glyphWidth = x1 - x0;
        i32 glyphHeight = y1 - y0;
        if (glyphWidth > 0 && glyphHeight > 0)
        {
            u8 *bitmap = (u8 *)malloc((size_t)glyphWidth * glyphHeight);
            if (bitmap)
            {
                stbtt_MakeCodepointBitmap(&face->info, bitmap, glyphWidth, glyphHeight,
                                          glyphWidth, scale, scale, (int)codePoint);
                for (i32 row = 0; row < glyphHeight; row++)
                {
                    i32 y = penY + y0 + row;
                    if (y < 0 || y >= bufferHeight)
                    {
                        continue;
                    }
                    for (i32 col = 0; col < glyphWidth; col++)
                    {
                        i32 px = x + x0 + col;
                        if (px < 0 || px >= bufferWidth)
                        {
                            continue;
                        }
                        BlendPixel(&rgba[((size_t)y * bufferWidth + px) * 4], r, g, b,
                                   bitmap[row * glyphWidth + col]);
                    }
                }
                free(bitmap);
            }
        }

        x += (i32)((f32)advanceWidth * scale);
        int nextCodePoint = (u32)(unsigned char)*cursor;
        if (nextCodePoint)
        {
            x += (i32)((f32)stbtt_GetCodepointKernAdvance(&face->info, (int)codePoint,
                                                          nextCodePoint) *
                       scale);
        }
    }
}

i32 FontDrawText(FontFace *face, u8 *rgba, i32 bufferWidth, i32 bufferHeight,
                 i32 penX, i32 penY, const char *utf8, f32 pixelHeight,
                 RenderColor fillColor, RenderColor outlineColor, bool hasOutline)
{
    if (!face || !rgba || !utf8)
    {
        return 0;
    }

    if (hasOutline)
    {
        // Four one-pixel offsets cover the eight surrounding directions closely enough at
        // these sizes, and match the original's four shifted passes.
        u8 outlineR = (u8)((outlineColor >> 16) & 0xff);
        u8 outlineG = (u8)((outlineColor >> 8) & 0xff);
        u8 outlineB = (u8)(outlineColor & 0xff);
        StampString(face, rgba, bufferWidth, bufferHeight, penX + 1, penY, utf8,
                    pixelHeight, outlineR, outlineG, outlineB);
        StampString(face, rgba, bufferWidth, bufferHeight, penX - 1, penY, utf8,
                    pixelHeight, outlineR, outlineG, outlineB);
        StampString(face, rgba, bufferWidth, bufferHeight, penX, penY + 1, utf8,
                    pixelHeight, outlineR, outlineG, outlineB);
        StampString(face, rgba, bufferWidth, bufferHeight, penX, penY - 1, utf8,
                    pixelHeight, outlineR, outlineG, outlineB);
    }

    u8 fillR = (u8)((fillColor >> 16) & 0xff);
    u8 fillG = (u8)((fillColor >> 8) & 0xff);
    u8 fillB = (u8)(fillColor & 0xff);
    StampString(face, rgba, bufferWidth, bufferHeight, penX, penY, utf8, pixelHeight,
                fillR, fillG, fillB);


    return FontMeasureText(face, utf8, pixelHeight);
}
