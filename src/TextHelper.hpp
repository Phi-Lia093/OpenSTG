#pragma once

#include "RenderCompat.hpp"
#include "RenderTexture.hpp"

#include "inttypes.hpp"

// A description of the pixel layout a text buffer is allocated with. The renderer
// uploads RGBA8 only, so this is now mostly bookkeeping: the GDI text path still wants
// to know the bit depth it drew into.
struct FormatInfo
{
    u32 format;
    i32 bitCount;
    u32 alphaMask;
    u32 redMask;
    u32 greenMask;
    u32 blueMask;
};

struct TextHelper
{
    TextHelper();
    ~TextHelper();
    bool AllocateBufferWithFallback(i32 width, i32 height, u32 format);
    bool CopyTextToSurface(SurfaceHandle *surface);
    static void CreateTextBuffer();
    FormatInfo *GetFormatInfo(u32 format);
    bool InvertAlpha(i32 x, i32 y, i32 spriteWidth, i32 fontHeight, i32 someFlag);
    bool ReleaseBuffer();
    static void ReleaseTextBuffer();
    static void RenderTextToTextureBold(i32 xPos, i32 yPos, i32 spriteWidth,
                                        i32 spriteHeight, i32 fontHeight,
                                        i32 fontWidth, D3DCOLOR textColor,
                                        u32 outlineType, char *string,
                                        TextureHandle *outTexture);
    bool TryAllocateBuffer(i32 width, i32 height, u32 format);

    u32 GetFormat()
    {
        return this->format;
    }

    i32 GetWidth()
    {
        return this->width;
    }

    i32 GetHeight()
    {
        return this->height;
    }

    u32 format;
    i32 width;
    i32 height;
    u32 imageSizeInBytes;
    i32 imageWidthInBytes;
    HDC hdc;
    HGDIOBJ gdiobj;
    HGDIOBJ gdiobj2;
    u8 *buffer;
};
