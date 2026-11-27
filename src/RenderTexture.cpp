#include "RenderTexture.hpp"

#include <cstdlib>
#include <cstring>

SurfaceDesc TextureHandle::GetDesc() const
{
    SurfaceDesc desc;
    desc.Width = (u32)this->width;
    desc.Height = (u32)this->height;
    desc.Format = this->format;
    return desc;
}

void TextureHandle::Release()
{
    RenderTextureDestroy(this->texture);
    this->texture = NULL;
    delete this;
}

void SurfaceHandle::Release()
{
    RenderTextureDestroy(this->surface);
    this->surface = NULL;
    delete this;
}

SurfaceDesc SurfaceHandle::GetDesc() const
{
    SurfaceDesc desc;
    desc.Width = (u32)this->width;
    desc.Height = (u32)this->height;
    desc.Format = this->format;
    return desc;
}

TextureHandle *TextureHandleCreate(u32 width, u32 height, u32 format)
{
    TextureHandle *handle = new TextureHandle();
    handle->texture = RenderTextureCreate(width, height, NULL);
    handle->priority = 0;
    handle->width = (i32)width;
    handle->height = (i32)height;
    handle->format = format;
    return handle;
}

SurfaceHandle *SurfaceHandleCreate(u32 width, u32 height, u32 format)
{
    SurfaceHandle *handle = new SurfaceHandle();
    handle->surface = RenderSurfaceCreate(width, height);
    handle->width = (i32)width;
    handle->height = (i32)height;
    handle->format = format;
    return handle;
}

TextureHandle *TextureHandleFromRGBA(u32 width, u32 height, const u8 *rgba)
{
    TextureHandle *handle = new TextureHandle();
    handle->texture = RenderTextureCreate(width, height, rgba);
    handle->priority = 0;
    handle->width = (i32)width;
    handle->height = (i32)height;
    handle->format = 0;
    return handle;
}

bool SurfaceHandle::LockRect(LockedRect *locked, void *rect, u32 flags)
{
    if (!this->surface || !this->surface->pixels)
    {
        return false;
    }
    locked->Pitch = this->width * 4;
    locked->pBits = this->surface->pixels;
    return true;
}

void SurfaceHandle::UnlockRect()
{
    if (this->surface && this->surface->pixels)
    {
        RenderTextureUpload(this->surface, this->surface->pixels);
    }
}

void SurfaceHandleCommit(SurfaceHandle *handle)
{
    if (handle && handle->surface && handle->surface->pixels)
    {
        RenderTextureUpload(handle->surface, handle->surface->pixels);
    }
}
