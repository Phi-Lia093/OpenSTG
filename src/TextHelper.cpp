#include "TextHelper.hpp"

#include "RenderCompat.hpp"
#include "Renderer.hpp"

#include <stdio.h>
#include <wingdi.h>

#include "Supervisor.hpp"
#include "dxutil.hpp"
#include "i18n.hpp"
#include "inttypes.hpp"

// GLOBAL: TH07 0x0049ed98
FormatInfo g_FormatInfoArray[7] = {
    {
        1,
        0x20,
        0,
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
    },
    {
        1,
        0x20,
        0xFF000000,
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
    },
    {
        2,
        0x10,
        0,
        0x00007C00,
        0x000003E0,
        0x0000001F,
    },
    {
        3,
        0x10,
        0,
        0x0000F800,
        0x000007E0,
        0x0000001F,
    },
    {
        2,
        0x10,
        0x00008000,
        0x00007C00,
        0x000003E0,
        0x0000001F,
    },
    {
        5,
        0x10,
        0x0000F000,
        0x00000F00,
        0x000000F0,
        0x0000000F,
    },
    {
        (u32)-1,
        0x0,
        0,
        0,
        0,
        0,
    },
};

// GLOBAL: TH07 0x0049fe28
SurfaceHandle *g_TextBufferSurface;

// FUNCTION: TH07 0x00431a0f
TextHelper::TextHelper()
{
    this->format = (u32)-1;
    this->width = 0;
    this->height = 0;
    this->hdc = NULL;
    this->gdiobj2 = NULL;
    this->gdiobj = NULL;
    this->buffer = NULL;
}

// FUNCTION: TH07 0x00431a4b
TextHelper::~TextHelper()
{
    ReleaseBuffer();
}

// FUNCTION: TH07 0x00431a5c
bool TextHelper::ReleaseBuffer()
{
    if (this->hdc)
    {
        SelectObject((HDC)this->hdc, this->gdiobj);
        DeleteDC((HDC)this->hdc);
        DeleteObject(this->gdiobj2);
        this->format = (u32)-1;
        this->width = 0;
        this->height = 0;
        this->hdc = NULL;
        this->gdiobj2 = NULL;
        this->gdiobj = NULL;
        this->buffer = NULL;
        return true;
    }

    return false;
}

// FUNCTION: TH07 0x00431ace
bool TextHelper::AllocateBufferWithFallback(i32 width, i32 height,
                                            u32 format)
{
    if (TryAllocateBuffer(width, height, format))
    {
        return true;
    }

    if (format == 2 || format == 5)
    {
        return TryAllocateBuffer(width, height, 1);
    }

    if (format == 3)
    {
        return TryAllocateBuffer(width, height, 1);
    }

    return false;
}

struct ThBitmapInfo
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[17];
};
C_ASSERT(sizeof(ThBitmapInfo) == 0x6c);

#pragma var_order(imageWidthInBytes, deviceContext, originalBitmapObj, \
                  bitmapInfo, formatInfo, bitmapObj, bitmapData)
// FUNCTION: TH07 0x00431b2d
bool TextHelper::TryAllocateBuffer(i32 width, i32 height, u32 format)
{
    i32 imageWidthInBytes;
    HDC deviceContext;
    HGDIOBJ originalBitmapObj;
    u8 *bitmapData;
    HBITMAP bitmapObj;
    FormatInfo *formatInfo;
    ThBitmapInfo bitmapInfo;

    ReleaseBuffer();
    memset(&bitmapInfo, 0, sizeof(ThBitmapInfo));
    formatInfo = GetFormatInfo(format);
    if (!formatInfo)
    {
        return false;
    }
    imageWidthInBytes = (width * formatInfo->bitCount / 8 + 3) / 4 * 4;
    bitmapInfo.bmiHeader.biSize = sizeof(ThBitmapInfo);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -(height + 1);
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = (WORD)formatInfo->bitCount;
    bitmapInfo.bmiHeader.biSizeImage = height * imageWidthInBytes;
    if (format != 2 && format != 1)
    {
        bitmapInfo.bmiHeader.biCompression = 3;
        ((u32 *)bitmapInfo.bmiColors)[0] = formatInfo->redMask;
        ((u32 *)bitmapInfo.bmiColors)[1] = formatInfo->greenMask;
        ((u32 *)bitmapInfo.bmiColors)[2] = formatInfo->blueMask;
        ((u32 *)bitmapInfo.bmiColors)[3] = formatInfo->alphaMask;
    }
    bitmapObj = CreateDIBSection(NULL, (BITMAPINFO *)&bitmapInfo, 0,
                                 (void **)&bitmapData, NULL, 0);
    if (!bitmapObj)
    {
        return false;
    }
    memset(bitmapData, 0, bitmapInfo.bmiHeader.biSizeImage);
    deviceContext = CreateCompatibleDC(NULL);
    originalBitmapObj = SelectObject(deviceContext, bitmapObj);
    this->hdc = deviceContext;
    this->gdiobj2 = bitmapObj;
    this->buffer = bitmapData;
    this->imageSizeInBytes = bitmapInfo.bmiHeader.biSizeImage;
    this->gdiobj = originalBitmapObj;
    this->width = width;
    this->height = height;
    this->format = format;
    this->imageWidthInBytes = imageWidthInBytes;
    return true;
}

// FUNCTION: TH07 0x00431cec
FormatInfo *TextHelper::GetFormatInfo(u32 format)
{
    i32 i;
    for (i = 0; g_FormatInfoArray[i].format != -1 &&
                g_FormatInfoArray[i].format != format;
         i++)
    {
    }

    if (format == -1)
    {
        return NULL;
    }

    return &g_FormatInfoArray[i];
}

struct A1R5G5B5
{
    u16 blue : 5;
    u16 green : 5;
    u16 red : 5;
    u16 alpha : 1;
};

#pragma var_order(bufferRegion, i, doubleArea, bufferCursor, bufferStart)
// FUNCTION: TH07 0x00431d3c
bool TextHelper::InvertAlpha(i32 x, i32 y, i32 spriteWidth, i32 fontHeight,
                             i32 param5)
{
    u8 *bufferStart;
    A1R5G5B5 *bufferCursor;
    i32 doubleArea;
    i32 i;
    u8 *bufferRegion;

    doubleArea = spriteWidth * fontHeight * 2;
    bufferStart = &this->buffer[0];
    bufferRegion = &bufferStart[y * spriteWidth * 2];
    switch (this->format)
    {
    case 1:
        for (i = 3; i < doubleArea; i += 4)
        {
            bufferRegion[i] ^= 255;
        }
        break;
    case 2:
        for (bufferCursor = (A1R5G5B5 *)bufferRegion, i = 0; i < doubleArea;
             i += 2, bufferCursor++)
        {
            bufferCursor->alpha ^= 1;
            if (bufferCursor->alpha)
            {
                if (!param5)
                {
                    if (bufferCursor->red >= bufferCursor->blue)
                    {
                        bufferCursor->red = bufferCursor->red -
                                            bufferCursor->red * i * 2 /
                                                doubleArea / 3;
                        bufferCursor->green = bufferCursor->green -
                                              bufferCursor->green * i * 2 /
                                                  doubleArea / 3;
                    }
                    else
                    {
                        bufferCursor->blue = bufferCursor->blue -
                                             bufferCursor->blue * i /
                                                 doubleArea / 2;
                        bufferCursor->green = bufferCursor->green -
                                              bufferCursor->green * i /
                                                  doubleArea / 2;
                    }
                }
                else
                {
                    if (bufferCursor->red >= bufferCursor->blue)
                    {
                        bufferCursor->red = bufferCursor->red -
                                            bufferCursor->red * i /
                                                doubleArea / 4;
                        bufferCursor->green = bufferCursor->green -
                                              bufferCursor->green * i /
                                                  doubleArea / 4;
                    }
                    else
                    {
                        bufferCursor->blue = bufferCursor->blue -
                                             bufferCursor->blue * i /
                                                 doubleArea / 4;
                        bufferCursor->green = bufferCursor->green -
                                              bufferCursor->green * i /
                                                  doubleArea / 4;
                    }
                }
            }
            else
            {
                bufferCursor->red = 0;
                bufferCursor->green = 0;
                bufferCursor->blue = 0;
            }
        }
        break;
    case 5:
        for (i = 1; i < doubleArea; i += 2)
        {
            bufferRegion[i] ^= 240;
        }
        break;
    default:
        return false;
    }
    return true;
}

// FUNCTION: TH07 0x00432164
bool TextHelper::CopyTextToSurface(SurfaceHandle *surface)
{
    u8 *srcBuf;
    i32 i;
    i32 srcWidthBytes;
    i32 dstWidthBytes;
    u8 *dstBuf;
    i32 copyWidth;

    if (!(u8)(u32)(this->gdiobj2 != NULL))
    {
        return false;
    }
    if (!surface || !surface->surface || !surface->surface->pixels || !this->buffer)
    {
        return false;
    }

    // The glyphs are drawn with GDI into a 32-bit buffer whose coverage ends up in the
    // red channel; that coverage becomes the alpha of the sprite, and the requested
    // colour is applied uniformly. Doing it this way avoids the original's alpha-bit
    // fiddling entirely, which relied on an exact A1R5G5B5 bit layout.
    srcWidthBytes = this->imageWidthInBytes;
    srcBuf = this->buffer;
    dstWidthBytes = surface->width * 4;
    dstBuf = surface->surface->pixels;

    copyWidth = this->GetWidth();
    if (copyWidth > surface->width)
    {
        copyWidth = surface->width;
    }

    for (i = 0; i < this->GetHeight() && i < surface->height; i++)
    {
        ConvertToRGBA8(srcBuf, dstBuf, (u32)copyWidth,
                       (TextureFormat)this->GetFormat(), 0, false);
        srcBuf += srcWidthBytes;
        dstBuf += dstWidthBytes;
    }

    // GDI never writes an alpha channel, so a 32-bit DIB comes back fully transparent and
    // the glyphs disappear. Their coverage is in the colour channels instead: take the
    // brightest channel as opacity.
    if (this->GetFormat() == 1)
    {
        u8 *dst = surface->surface->pixels;
        for (i32 row = 0; row < this->GetHeight() && row < surface->height; row++)
        {
            for (i32 col = 0; col < copyWidth; col++)
            {
                u8 *px = &dst[((size_t)row * surface->width + col) * 4];
                u8 coverage = px[0] > px[1] ? px[0] : px[1];
                if (px[2] > coverage)
                {
                    coverage = px[2];
                }
                px[3] = coverage;
            }
        }
    }

    surface->UnlockRect();
    return true;
}

// FUNCTION: TH07 0x0043225b
void TextHelper::CreateTextBuffer()
{
    // A 1024x64 scratch surface the GDI text is composed into. Format 1 is 32-bit RGBA,
    // which GDI writes as BGRA; reading the coverage from one channel there is far more
    // dependable than interpreting 16-bit alpha bits.
    g_TextBufferSurface = SurfaceHandleCreate(1024, 64, 1);
}

// FUNCTION: TH07 0x0043227e
void TextHelper::ReleaseTextBuffer()
{
    if (g_TextBufferSurface)
    {
        g_TextBufferSurface->Release();
        g_TextBufferSurface = NULL;
    }
}

// FUNCTION: TH07 0x004322a3
void TextHelper::RenderTextToTextureBold(i32 xPos, i32 yPos, i32 spriteWidth,
                                         i32 spriteHeight, i32 fontHeight,
                                         i32 fontWidth, D3DCOLOR textColor,
                                         u32 outlineType, char *string,
                                         TextureHandle *outTexture)
{
    RenderTexture *dst;
    FontFace *face;
    i32 textWidth;
    i32 drawX;
    i32 drawY;

    if (!outTexture || !outTexture->texture || !string)
    {
        return;
    }

    face = FontGetDefault();
    if (!face)
    {
        return;
    }

    if (fontHeight <= 0)
    {
        fontHeight = 15;
    }
    if (fontWidth <= 0)
    {
        fontWidth = 15;
    }

    dst = outTexture->texture;
    if (!dst->pixels)
    {
        dst->pixels = (u8 *)malloc((size_t)dst->width * dst->height * 4);
        if (!dst->pixels)
        {
            return;
        }
        memset(dst->pixels, 0, (size_t)dst->width * dst->height * 4);
    }

    // Clear only the band this sprite occupies. Several sprites share one texture (every
    // menu description does), so clearing the whole thing would erase the lines drawn for
    // the sprites before this one.
    {
        i32 clearTop = yPos;
        i32 clearBottom = yPos + fontHeight + 2;
        if (clearTop < 0)
        {
            clearTop = 0;
        }
        if (clearBottom > dst->height)
        {
            clearBottom = dst->height;
        }
        if (clearBottom > clearTop)
        {
            memset(&dst->pixels[(size_t)clearTop * dst->width * 4], 0,
                   (size_t)(clearBottom - clearTop) * dst->width * 4);
        }
    }

    // Centre the string inside the sprite. The glyph height is the line height the caller
    // asked for; stb_truetype scales by character height, which is what fontHeight names.
    textWidth = FontMeasureText(face, string, (f32)fontHeight);
    drawX = (spriteWidth - textWidth) / 2;
    if (drawX < 0)
    {
        drawX = 0;
    }
    // stb_truetype positions glyphs relative to the baseline, and a glyph extends upwards
    // from it. Placing the baseline at the sprite's top would draw the whole character
    // above the visible area, so the baseline is pushed down by the ascent.
    drawY = yPos + FontAscent(face, (f32)fontHeight);

    // outlineType comes in as an RGB colour; 0xffffffff means the text has no outline.
    // Painting the outline in that colour and the glyph in textColor reproduces the
    // original's bordered look.
    FontDrawText(face, dst->pixels, dst->width, dst->height, drawX, drawY, string,
                 (f32)fontHeight, textColor, outlineType,
                 outlineType != 0xffffffff);

    RenderTextureUpload(dst, dst->pixels);

}
