// Texture and surface handles, standing in for IDirect3DTexture8 / IDirect3DSurface8.
//
// The game keeps textures in arrays and passes pointers around, so these are plain
// handles with the small set of operations the call sites use. A texture is GPU-only;
// a surface also carries a CPU mirror, because the game copies pixels between surfaces
// and reads the framebuffer back for its screenshots.
#pragma once

#include "Renderer.hpp"
#include "inttypes.hpp"

struct SurfaceDesc
{
    u32 Width;
    u32 Height;
    u32 Format;
};

struct TextureHandle
{
    RenderTexture *texture;
    u32 priority;
    i32 width;
    i32 height;
    u32 format;

    void Release();
    void SetPriority(u32 value) { this->priority = value; }
    void PreLoad() {}
    SurfaceDesc GetDesc() const;
};

struct LockedRect
{
    i32 Pitch;
    void *pBits;
};

struct SurfaceHandle
{
    RenderTexture *surface;
    i32 width;
    i32 height;
    u32 format;

    void Release();

    // Locking exposes the CPU mirror so GDI can draw into it, or code can poke pixels.
    // Unlocking uploads the mirror back to the GPU.
    bool LockRect(LockedRect *locked, void *rect, u32 flags);
    void UnlockRect();
    SurfaceDesc GetDesc() const;
};

TextureHandle *TextureHandleCreate(u32 width, u32 height, u32 format);
SurfaceHandle *SurfaceHandleCreate(u32 width, u32 height, u32 format);
TextureHandle *TextureHandleFromRGBA(u32 width, u32 height, const u8 *rgba);

// Uploads a surface's CPU pixels to its GPU texture. Needed after anything writes into
// the surface without going through LockRect/UnlockRect.
void SurfaceHandleCommit(SurfaceHandle *handle);
