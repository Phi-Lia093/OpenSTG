#include "AnmManager.hpp"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Global.hpp"
#include "RenderCompat.hpp"
#include "Renderer.hpp"
#include "Stage.hpp"
#include "Supervisor.hpp"
#include "TextHelper.hpp"
#include "ZunMath.hpp"
#include "dxutil.hpp"
#include "i18n.hpp"
#include "utils.hpp"

// Counters for the sprite batcher, written out by Flush() so a frame can be inspected
// without attaching a debugger.

// GLOBAL: TH07 0x004b9e44
AnmManager *g_AnmManager;

// GLOBAL: TH07 0x004b9fa8
VertexTex1DiffuseXyzrhw g_QuadVertices[4];

// GLOBAL: TH07 0x004ba018
VertexTex1Xyzrhw g_QuadTemplate[4];

// GLOBAL: TH07 0x004ba078
VertexTex1DiffuseXyz g_Quad3DFallback[4];

// THTX pixel formats, in the order the game indexes them. Converted to RGBA8 at load
// time, so the GL side only ever sees one format.
const u32 g_TextureFormatD3D8Mapping[6] = {
    TEXTURE_FORMAT_UNKNOWN,
    TEXTURE_FORMAT_ARGB8888,
    TEXTURE_FORMAT_ARGB1555,
    TEXTURE_FORMAT_RGB565,
    TEXTURE_FORMAT_RGB888,
    TEXTURE_FORMAT_ARGB4444,
};

// GLOBAL: TH07 0x00495144
const i32 g_TextureBytesPerPixel[7] = {4, 4, 2, 2, 3, 2, 0};

// FUNCTION: TH07 0x0044d3e0
AnmManager::AnmManager()
{
    memset(this, 0, sizeof(AnmManager));

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->sprites); i++)
    {
        this->sprites[i].sourceFileIndex = -1;
    }
    g_QuadTemplate[0].w =
        g_QuadTemplate[1].w =
            g_QuadTemplate[2].w =
                g_QuadTemplate[3].w = 1.0f;
    g_QuadTemplate[0].textureUV.x = 0.0f;
    g_QuadTemplate[0].textureUV.y = 0.0f;
    g_QuadTemplate[1].textureUV.x = 1.0f;
    g_QuadTemplate[1].textureUV.y = 0.0f;
    g_QuadTemplate[2].textureUV.x = 0.0f;
    g_QuadTemplate[2].textureUV.y = 1.0f;
    g_QuadTemplate[3].textureUV.x = 1.0f;
    g_QuadTemplate[3].textureUV.y = 1.0f;
    g_QuadVertices[0].w =
        g_QuadVertices[1].w =
            g_QuadVertices[2].w =
                g_QuadVertices[3].w = 1.0f;
    g_QuadVertices[0].textureUV.x = 0.0f;
    g_QuadVertices[0].textureUV.y = 0.0f;
    g_QuadVertices[1].textureUV.x = 1.0f;
    g_QuadVertices[1].textureUV.y = 0.0f;
    g_QuadVertices[2].textureUV.x = 0.0f;
    g_QuadVertices[2].textureUV.y = 1.0f;
    g_QuadVertices[3].textureUV.x = 1.0f;
    g_QuadVertices[3].textureUV.y = 1.0f;

    this->vertexBuffer = NULL;
    this->currentTexture = NULL;
    this->currentBlendMode = 0;
    this->currentColorOp = 0;
    this->currentTextureFactor.color = 1;
    this->currentVertexShader = 0;
    this->currentCameraMode = 255;
    this->currentZWriteDisable = 0;
    this->screenshotTextureId = -1;
}

// FUNCTION: TH07 0x0044d620 FOLDED
AnmManager::~AnmManager()
{
}

// FUNCTION: TH07 0x0044d630
void AnmManager::SetupVertexBuffer()
{
    RenderVertexInfo *vertexData;

    this->vertexBufferContents[2].pos.x = -128.0f;
    this->vertexBufferContents[0].pos.x = -128.0f;
    this->vertexBufferContents[3].pos.x = 128.0f;
    this->vertexBufferContents[1].pos.x = 128.0f;
    this->vertexBufferContents[1].pos.y = -128.0f;
    this->vertexBufferContents[0].pos.y = -128.0f;
    this->vertexBufferContents[3].pos.y = 128.0f;
    this->vertexBufferContents[2].pos.y = 128.0f;
    this->vertexBufferContents[3].pos.z = 0.0f;
    this->vertexBufferContents[2].pos.z = 0.0f;
    this->vertexBufferContents[1].pos.z = 0.0f;
    this->vertexBufferContents[0].pos.z = 0.0f;
    this->vertexBufferContents[2].textureUV.x = 0.0f;
    this->vertexBufferContents[0].textureUV.x = 0.0f;
    this->vertexBufferContents[3].textureUV.x = 1.0f;
    this->vertexBufferContents[1].textureUV.x = 1.0f;
    this->vertexBufferContents[1].textureUV.y = 0.0f;
    this->vertexBufferContents[0].textureUV.y = 0.0f;
    this->vertexBufferContents[3].textureUV.y = 1.0f;
    this->vertexBufferContents[2].textureUV.y = 1.0f;
    g_Quad3DFallback[0].pos =
        this->vertexBufferContents[0].pos;
    g_Quad3DFallback[1].pos =
        this->vertexBufferContents[1].pos;
    g_Quad3DFallback[2].pos =
        this->vertexBufferContents[2].pos;
    g_Quad3DFallback[3].pos =
        this->vertexBufferContents[3].pos;
    g_Quad3DFallback[0].textureUV.x =
        this->vertexBufferContents[0].textureUV.x;
    g_Quad3DFallback[0].textureUV.y =
        this->vertexBufferContents[0].textureUV.y;
    g_Quad3DFallback[1].textureUV.x =
        this->vertexBufferContents[1].textureUV.x;
    g_Quad3DFallback[1].textureUV.y =
        this->vertexBufferContents[1].textureUV.y;
    g_Quad3DFallback[2].textureUV.x =
        this->vertexBufferContents[2].textureUV.x;
    g_Quad3DFallback[2].textureUV.y =
        this->vertexBufferContents[2].textureUV.y;
    g_Quad3DFallback[3].textureUV.x =
        this->vertexBufferContents[3].textureUV.x;
    g_Quad3DFallback[3].textureUV.y =
        this->vertexBufferContents[3].textureUV.y;
    // The unit quad used to live in a D3D vertex buffer and be drawn with a world
    // matrix. Draw3 now projects that quad itself and hands screen-space triangles to
    // the renderer, so there is no buffer to create here.
    (void)vertexData;
    this->vertexBuffer = NULL;
}

// FUNCTION: TH07 0x0044d8f0
ZunResult AnmManager::LoadTexture(i32 textureIdx, const char *texturePath,
                                  i32 formatIdx, D3DCOLOR colorKey)
{
    u8 *srcData;
    i32 width;
    i32 height;
    u8 *rgba;

    ReleaseTexture(textureIdx);

    srcData = FileSystem::OpenFile(texturePath, 1);
    if (!srcData)
    {
        return ZUN_ERROR;
    }

    // The external textures are encoded images (PNG/JPG) stored inside th07.dat.
    // D3DX decoded them before; SDL_image does it now, straight to RGBA8.
    rgba = DecodeImageToRGBA(srcData, g_LastFileSize, &width, &height);
    if (!rgba)
    {
        ZUN_FREE(srcData);
        return ZUN_ERROR;
    }

    (void)formatIdx;
    (void)colorKey;

    ReleaseTexture(textureIdx);
    free(rgba);

    this->imageDataArray[textureIdx] = srcData;
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044d9e0
ZunResult AnmManager::LoadTextureEmbedded(u32 textureIdx,
                                          ZunImageInfoEmbedded *imageInfo,
                                          D3DCOLOR formatIdx)
{
    u32 pixelCount;
    u32 rgbaSize;
    u8 *rgba;

    (void)formatIdx;

    ReleaseTexture(textureIdx);

    if (imageInfo->width <= 0 || imageInfo->height <= 0)
    {
        return ZUN_ERROR;
    }

    pixelCount = (u32)imageInfo->width * (u32)imageInfo->height;
    rgbaSize = pixelCount * 4;
    rgba = (u8 *)malloc(rgbaSize);
    if (!rgba)
    {
        return ZUN_ERROR;
    }

    // An embedded THTX image is raw pixels in one of the game's own formats; convert
    // once at load time so the GPU only ever sees RGBA8.
    ConvertToRGBA8(imageInfo->data, rgba, pixelCount,
                   (TextureFormat)imageInfo->format, 0, false);

    this->textures[textureIdx] =
        TextureHandleFromRGBA((u32)imageInfo->width, (u32)imageInfo->height, rgba);

    free(rgba);

    return this->textures[textureIdx] ? ZUN_SUCCESS : ZUN_ERROR;
}

// The alpha-only companion image: a block whose red channel becomes the destination
// texture's alpha. D3DX did this on the GPU before; now it is a plain per-pixel pass
// over the RGBA8 image, which is the only format the renderer uploads.
ZunResult AnmManager::LoadTextureAlphaChannel(i32 textureIdx,
                                             const char *texturePath,
                                             i32 formatIdx, D3DCOLOR colorKey)
{
    i32 width;
    i32 height;
    u8 *data;
    u8 *alpha;
    u32 i;
    u32 pixelCount;
    TextureHandle *handle;

    (void)formatIdx;
    (void)colorKey;

    data = FileSystem::OpenFile(texturePath, 0);
    if (!data)
    {
        return ZUN_ERROR;
    }

    handle = this->textures[textureIdx];
    if (!handle || !handle->texture || !handle->texture->pixels)
    {
        ZUN_FREE(data);
        return ZUN_ERROR;
    }

    alpha = DecodeImageToRGBA(data, g_LastFileSize, &width, &height);
    ZUN_FREE(data);
    if (!alpha)
    {
        return ZUN_ERROR;
    }

    if (handle->width != width || handle->height != height)
    {
        free(alpha);
        return ZUN_ERROR;
    }

    // Take the red channel of the companion image as alpha, keeping the destination's
    // colour untouched.
    pixelCount = (u32)(width * height);
    u8 *pixels = handle->texture->pixels;
    for (i = 0; i < pixelCount; i++)
    {
        pixels[i * 4 + 3] = alpha[i * 4 + 0];
    }
    free(alpha);

    RenderTextureUpload(handle->texture, pixels);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044df40
ZunResult AnmManager::CreateEmptyTexture(i32 textureIdx, u32 width, u32 height,
                                         i32 textureFormat)
{
    (void)textureFormat;
    ReleaseTexture(textureIdx);
    this->textures[textureIdx] = TextureHandleCreate(width, height, 0);
    return this->textures[textureIdx] ? ZUN_SUCCESS : ZUN_ERROR;
}

#pragma var_order(res, startIdx, ownsMemory, entry)
// FUNCTION: TH07 0x0044df90
i32 AnmManager::LoadAnms(i32 anmIdx, const char *path, i32 spriteIdxOffset)
{
    i32 res;
    u32 ownsMemory;
    AnmRawEntry *entry;

    entry = (AnmRawEntry *)FileSystem::OpenFile(path, 0);
    ownsMemory = 1;
    i32 startIdx = anmIdx;
    if (!entry)
    {
        g_GameErrorContext.Fatal(TH_ERR_ANM_LOAD_FAIL);
        return ZUN_ERROR;
    }
    while (true)
    {
        res = LoadAnm(anmIdx, entry, spriteIdxOffset, ownsMemory);
        if (res < 0)
        {
            this->anmFiles[startIdx].childCount = anmIdx - startIdx;
            return res;
        }
        anmIdx++;
        if (entry->nextOffset == 0)
        {
            this->anmFiles[startIdx].childCount = anmIdx - startIdx;
            return ZUN_SUCCESS;
        }
        entry = (AnmRawEntry *)((u8 *)entry + entry->nextOffset);
        ownsMemory = 0;
        spriteIdxOffset = spriteIdxOffset + res;
    }
}

#pragma var_order(id, data, desc, name, rawSprite, i, curSprite, \
                  loadedSprite)
// FUNCTION: TH07 0x0044e070
i32 AnmManager::LoadAnm(i32 textureIdx, AnmRawEntry *rawEntry,
                        i32 spriteIdxOffset, u32 ownsMemory)
{
    char *name;
    AnmRawSprite *rawSprite;
    AnmLoadedSprite loadedSprite;
    i32 *curSprite;
    i32 i;
    SurfaceDesc desc;
    AnmRawEntry *data;
    i32 id;

    id = 0;
    if (!rawEntry)
    {
        g_GameErrorContext.Fatal(TH_ERR_ANM_LOAD_FAIL);
        return ZUN_ERROR;
    }
    if (textureIdx >= ARRAY_SIZE_SIGNED(this->anmFiles))
    {
        g_GameErrorContext.Fatal(TH_ERR_LOW_TEXTURE_STORAGE);
        return ZUN_ERROR;
    }
    ReleaseAnm(textureIdx);
    data = rawEntry;
    if (data->version != 2)
    {
        g_GameErrorContext.Fatal(TH_ERR_DIFF_ANM_VER);
        return ZUN_ERROR;
    }
    data->textureIdx = textureIdx;
    data->ownsMemory = ownsMemory;
    if (!data->hasData)
    {
        name = (char *)((u8 *)data + data->nameOffset);
        if (*name == '@')
        {
            CreateEmptyTexture(data->textureIdx, data->width, data->height,
                               data->format);
        }
        else
        {
            if (LoadTexture(data->textureIdx, name, data->format, data->color_key) !=
                ZUN_SUCCESS)
            {
                g_GameErrorContext.Fatal(TH_ERR_TEX_LOAD_FAIL, name);
                return ZUN_ERROR;
            }
        }
        if (data->mipmapNameOffset != 0)
        {
            name = (char *)((u8 *)data + data->mipmapNameOffset);
            if (LoadTextureAlphaChannel(data->textureIdx, name, data->format,
                                        data->color_key) != ZUN_SUCCESS)
            {
                g_GameErrorContext.Fatal(TH_ERR_TEX_LOAD_FAIL, name);
                return ZUN_ERROR;
            }
        }
    }
    else
    {
        if (LoadTextureEmbedded(
                data->textureIdx,
                (ZunImageInfoEmbedded *)((u8 *)data + data->textureOffset),
                data->format) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Fatal(TH_ERR_EMBED_TEX_LOAD_FAIL);
            return ZUN_ERROR;
        }
    }
    this->textureNames[textureIdx] = (char *)((u8 *)data + data->nameOffset);
    this->textures[textureIdx]->SetPriority(data->priority);
    this->textures[textureIdx]->PreLoad();
    desc = this->textures[textureIdx]->GetDesc();
    data->spriteIdxOffset = spriteIdxOffset;
    curSprite = data->spriteOffsets;
    for (i = 0; i < data->numSprites; i++, curSprite++)
    {
        rawSprite = (AnmRawSprite *)((u8 *)data + *curSprite);
        loadedSprite.sourceFileIndex = data->textureIdx;
        loadedSprite.cols = (f32)desc.Width / (f32)data->width;
        loadedSprite.rows = (f32)desc.Height / (f32)data->height;
        loadedSprite.startPixelInclusive.x =
            loadedSprite.cols * rawSprite->offset.x;
        loadedSprite.startPixelInclusive.y =
            loadedSprite.rows * rawSprite->offset.y;
        loadedSprite.endPixelInclusive.x =
            (rawSprite->offset.x + rawSprite->size.x) * loadedSprite.cols;
        loadedSprite.endPixelInclusive.y =
            (rawSprite->offset.y + rawSprite->size.y) * loadedSprite.rows;
        loadedSprite.textureWidth = (f32)desc.Width;
        loadedSprite.textureHeight = (f32)desc.Height;
        if (id < rawSprite->id)
        {
            id = rawSprite->id;
        }
        if (rawSprite->id + spriteIdxOffset >= ARRAY_SIZE_SIGNED(this->sprites))
        {
            g_GameErrorContext.Fatal(TH_ERR_LOW_SPRITE_TABLE);
            return ZUN_ERROR;
        }
        LoadSprite(rawSprite->id + spriteIdxOffset, &loadedSprite);
    }
    for (i = 0; i < data->numScripts; i++, curSprite += 2)
    {
        if (*curSprite + spriteIdxOffset >= ARRAY_SIZE_SIGNED(this->sprites))
        {
            g_GameErrorContext.Fatal(TH_ERR_LOW_SCRIPT_TABLE);
            return ZUN_ERROR;
        }
        if (id < *curSprite)
        {
            id = *curSprite;
        }
        this->scripts[*curSprite + spriteIdxOffset] =
            (AnmRawInstr *)((u8 *)data + curSprite[1]);
        this->spriteIndices[*curSprite + spriteIdxOffset] = spriteIdxOffset;
    }
    this->anmFiles[textureIdx].raw = data;
    this->anmFiles[textureIdx].spriteIndexOffset = spriteIdxOffset;
    return id + 1;
}

#pragma var_order(spriteIdx, spriteIdxOffset, i, uvX, afterHdr, rawEntry)
// FUNCTION: TH07 0x0044e4e0
void AnmManager::ReleaseAnm(i32 anmIdx)
{
    AnmRawEntry *rawEntry;
    i32 *afterHdr;
    i32 uvX;
    i32 i;
    i32 spriteIdxOffset;
    i32 *spriteIdx;

    if (anmIdx < 0 || (u32)anmIdx >= ARRAY_SIZE_SIGNED(this->anmFiles))
    {
        return;
    }

    if (this->anmFiles[anmIdx].raw)
    {
        afterHdr = this->anmFiles[anmIdx].raw->spriteOffsets;
        spriteIdxOffset = this->anmFiles[anmIdx].spriteIndexOffset;
        rawEntry = this->anmFiles[anmIdx].raw;
        uvX = anmIdx + 1;
        for (i = 1; i < this->anmFiles[anmIdx].childCount; i++, uvX++)
        {
            ReleaseAnm(uvX);
        }
        for (i = 0; i < rawEntry->numSprites; i++, afterHdr++)
        {
            spriteIdx = (i32 *)((u8 *)rawEntry + *afterHdr);
            memset(&this->sprites[*spriteIdx + spriteIdxOffset], 0,
                   sizeof(AnmLoadedSprite));
            this->sprites[*spriteIdx + spriteIdxOffset].sourceFileIndex = -1;
        }
        for (i = 0; i < rawEntry->numScripts; i++, afterHdr += 2)
        {
            this->scripts[*afterHdr + spriteIdxOffset] = NULL;
            this->spriteIndices[*afterHdr + spriteIdxOffset] = 0;
        }
        this->anmFiles[anmIdx].spriteIndexOffset = 0;
        ReleaseTexture(rawEntry->textureIdx);
        if (rawEntry->ownsMemory)
        {
            ZUN_FREE(rawEntry);
        }
        this->anmFiles[anmIdx].raw = NULL;
        this->currentBlendMode = 255;
        this->currentColorOp = 255;
        this->currentVertexShader = 0;
        this->currentTexture = NULL;
        this->anmFiles[anmIdx].childCount = 0;
    }
}

// FUNCTION: TH07 0x0044e6f0
void AnmManager::ReleaseTexture(i32 textureIdx)
{
    if (textureIdx < 0 || textureIdx >= ARRAY_SIZE(this->textures))
    {
        return;
    }

    SAFE_RELEASE(this->textures[textureIdx]);
    ZUN_FREE(this->imageDataArray[textureIdx]);
    this->imageDataArray[textureIdx] = NULL;
}

// FUNCTION: TH07 0x0044e780
void AnmManager::LoadSprite(u32 spriteIdx, AnmLoadedSprite *sprite)
{
    this->sprites[spriteIdx] = *sprite;
    this->sprites[spriteIdx].spriteId = this->loadedSpriteCount++;

    this->sprites[spriteIdx].uvStart.x =
        this->sprites[spriteIdx].startPixelInclusive.x /
        (this->sprites[spriteIdx].textureWidth);
    this->sprites[spriteIdx].uvEnd.x =
        this->sprites[spriteIdx].endPixelInclusive.x /
        (this->sprites[spriteIdx].textureWidth);
    this->sprites[spriteIdx].uvStart.y =
        this->sprites[spriteIdx].startPixelInclusive.y /
        (this->sprites[spriteIdx].textureHeight);
    this->sprites[spriteIdx].uvEnd.y =
        this->sprites[spriteIdx].endPixelInclusive.y /
        (this->sprites[spriteIdx].textureHeight);
    this->sprites[spriteIdx].widthPx =
        (this->sprites[spriteIdx].endPixelInclusive.x -
         this->sprites[spriteIdx].startPixelInclusive.x) /
        sprite->cols;
    this->sprites[spriteIdx].heightPx =
        (this->sprites[spriteIdx].endPixelInclusive.y -
         this->sprites[spriteIdx].startPixelInclusive.y) /
        sprite->rows;
}

// FUNCTION: TH07 0x0044e8e0
ZunResult AnmManager::SetActiveSprite(AnmVm *vm, i32 spriteIdx)
{
    if (this->sprites[spriteIdx].sourceFileIndex < 0)
    {
        return ZUN_ERROR;
    }

    vm->activeSpriteIdx = (i16)spriteIdx;
    vm->sprite = &this->sprites[spriteIdx];
    D3DXMatrixIdentity(&vm->matrix);
    D3DXMatrixIdentity(&vm->uvMatrix);
    if (vm->sprite->cols < 1.0f)
    {
        // ZUN bloat: This is despite the fact that spriteIdx is never used ever again
        spriteIdx = 0;
    }
    vm->matrix.m[0][0] = vm->sprite->widthPx / 256.0f;
    vm->matrix.m[1][1] = vm->sprite->heightPx / 256.0f;
    vm->uvMatrix.m[0][0] =
        vm->sprite->widthPx / vm->sprite->textureWidth * vm->sprite->cols;
    vm->uvMatrix.m[1][1] =
        vm->sprite->heightPx / vm->sprite->textureHeight * vm->sprite->rows;
    vm->worldTransformMatrix = vm->matrix;
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044ea20
void AnmManager::SetAndExecuteScript(AnmVm *vm, AnmRawInstr *beginningOfScript)
{
    if (!beginningOfScript)
    {
        memset(vm, 0, sizeof(AnmVm));
    }
    else
    {
        vm->flip = 0;
        vm->Initialize();
        vm->beginningOfScript = beginningOfScript;
        vm->currentInstruction = vm->beginningOfScript;
        vm->currentTimeInScript = 0;
        vm->visible = 0;
        ExecuteScript(vm);
        this->scriptsExecutedThisFrame++;
    }
}

// FUNCTION: TH07 0x0044eae0
void AnmManager::SetRenderStateForVm(AnmVm *vm)
{
    ZunColor color;

    if ((u32)this->currentBlendMode != vm->blendMode)
    {
        this->currentBlendMode = vm->blendMode;
        RendererSetBlend(this->currentBlendMode ? RENDER_BLEND_ADD : RENDER_BLEND_ALPHA);
    }
    color.color = vm->useColor2 ? vm->color2.color : vm->color.color;
    if (!g_Supervisor.cfg.noVertexBuffers)
    {
        if (this->colorMulEnabled)
        {
            color.bytes.r = ZunColor::Multiply(color.bytes.r, this->color.bytes.r);
            color.bytes.g = ZunColor::Multiply(color.bytes.g, this->color.bytes.g);
            color.bytes.b = ZunColor::Multiply(color.bytes.b, this->color.bytes.b);
            color.bytes.a = ZunColor::Multiply(color.bytes.a, this->color.bytes.a);
        }
        // The texture factor is now carried per-vertex when the sprite is pushed, so
        // there is nothing to set on the device here.
        this->currentTextureFactor.color = color.color;
    }
    else
    {
        if (this->colorMulEnabled)
        {
            color.bytes.r = ZunColor::Multiply(color.bytes.r, this->color.bytes.r);
            color.bytes.g = ZunColor::Multiply(color.bytes.g, this->color.bytes.g);
            color.bytes.b = ZunColor::Multiply(color.bytes.b, this->color.bytes.b);
            color.bytes.a = ZunColor::Multiply(color.bytes.a, this->color.bytes.a);
        }
        g_QuadVertices[0].diffuse = color;
        g_QuadVertices[1].diffuse = color;
        g_QuadVertices[2].diffuse = color;
        g_QuadVertices[3].diffuse = color;
        g_Quad3DFallback[0].diffuse = color;
        g_Quad3DFallback[1].diffuse = color;
        g_Quad3DFallback[2].diffuse = color;
        g_Quad3DFallback[3].diffuse = color;
    }
    if (!g_Supervisor.cfg.disableZBuffer &&
        (u32)this->currentZWriteDisable != vm->zWriteDisable)
    {
        this->currentZWriteDisable = vm->zWriteDisable;
        RendererSetZWrite(this->currentZWriteDisable == 0);
    }
    if ((u32)this->currentCameraMode != vm->cameraMode)
    {
        g_AnmManager->Flush();
        this->currentCameraMode = vm->cameraMode;
        if (!this->currentCameraMode)
        {
            g_Stage.SetupCameraStageBackground();
        }
        else
        {
            g_Stage.UpdateCamera();
        }
        RendererSetViewport((i32)g_Supervisor.viewport.X, (i32)g_Supervisor.viewport.Y,
                            (i32)g_Supervisor.viewport.Width,
                            (i32)g_Supervisor.viewport.Height);
    }
    this->renderStateChangesThisFrame++;
}

// FUNCTION: TH07 0x0044eec0
void AnmManager::SyncRenderState(AnmVm *vm)
{
    if ((u32)this->currentBlendMode != vm->blendMode)
    {
        this->currentBlendMode = vm->blendMode;
        RendererSetBlend(this->currentBlendMode ? RENDER_BLEND_ADD : RENDER_BLEND_ALPHA);
    }
    if (!g_Supervisor.cfg.disableZBuffer &&
        (u32)this->currentZWriteDisable != vm->zWriteDisable)
    {
        this->currentZWriteDisable = vm->zWriteDisable;
        RendererSetZWrite(this->currentZWriteDisable == 0);
    }
    this->renderStateChangesThisFrame++;
}

static const f32 g_ZeroPointFive = 0.5;

#pragma var_order(triangleY1, triangleY2, triangleX2, triangleX1, color)
// FUNCTION: TH07 0x0044efb0
ZunResult AnmManager::DrawInner(AnmVm *vm, u32 drawFlags)
{
    ZunColor color;
    f32 triangleX1, triangleX2, triangleY1, triangleY2;

    g_QuadVertices[0].pos.x += this->offset.x;
    g_QuadVertices[0].pos.y += this->offset.y;
    g_QuadVertices[1].pos.x += this->offset.x;
    g_QuadVertices[1].pos.y += this->offset.y;
    g_QuadVertices[2].pos.x += this->offset.x;
    g_QuadVertices[2].pos.y += this->offset.y;
    g_QuadVertices[3].pos.x += this->offset.x;
    g_QuadVertices[3].pos.y += this->offset.y;

    if ((drawFlags & 1) != 0)
    {
        /*g_QuadVertices[0].pos.x =
            roundf(g_QuadVertices[0].pos.x) - 0.5f;
        g_QuadVertices[1].pos.x =
            roundf(g_QuadVertices[1].pos.x) - 0.5f;
        g_QuadVertices[0].pos.y =
            roundf(g_QuadVertices[0].pos.y) - 0.5f;
        g_QuadVertices[2].pos.y =
            roundf(g_QuadVertices[2].pos.y) - 0.5f;
        g_QuadVertices[1].pos.y =
            g_QuadVertices[0].pos.y;
        g_QuadVertices[2].pos.x =
            g_QuadVertices[0].pos.x;
        g_QuadVertices[3].pos.x =
            g_QuadVertices[1].pos.x;
        g_QuadVertices[3].pos.y =
        g_QuadVertices[2].pos.y;*/
        __asm {
        fld g_QuadVertices[0 * TYPE g_QuadVertices].pos.x
        frndint
        fsub g_ZeroPointFive
        fld g_QuadVertices[1 * TYPE g_QuadVertices].pos.x
        frndint
        fsub g_ZeroPointFive
        fld g_QuadVertices[0 * TYPE g_QuadVertices].pos.y
        frndint
        fsub g_ZeroPointFive
        fld g_QuadVertices[2 * TYPE g_QuadVertices].pos.y
        frndint
        fsub g_ZeroPointFive
        fst g_QuadVertices[2 * TYPE g_QuadVertices].pos.y
        fstp g_QuadVertices[3 * TYPE g_QuadVertices].pos.y
        fst g_QuadVertices[0 * TYPE g_QuadVertices].pos.y
        fstp g_QuadVertices[1 * TYPE g_QuadVertices].pos.y
        fst g_QuadVertices[1 * TYPE g_QuadVertices].pos.x
        fstp g_QuadVertices[3 * TYPE g_QuadVertices].pos.x
        fst g_QuadVertices[0 * TYPE g_QuadVertices].pos.x
        fstp g_QuadVertices[2 * TYPE g_QuadVertices].pos.x
        }
    }

    // uvStart/uvEnd are the sprite's absolute position in the atlas, so they are already
    // the texture coordinates to sample. (The original reached the same numbers by
    // feeding 0..1 through a texture matrix whose scale was uvEnd - uvStart and whose
    // offset was uvStart; the arithmetic is done once here instead.)
    g_QuadVertices[0].textureUV.x = g_QuadVertices[2].textureUV.x =
        vm->sprite->uvStart.x + vm->uvScrollPos.x;
    g_QuadVertices[1].textureUV.x = g_QuadVertices[3].textureUV.x =
        vm->sprite->uvEnd.x + vm->uvScrollPos.x;
    g_QuadVertices[0].textureUV.y = g_QuadVertices[1].textureUV.y =
        vm->sprite->uvStart.y + vm->uvScrollPos.y;
    g_QuadVertices[2].textureUV.y = g_QuadVertices[3].textureUV.y =
        vm->sprite->uvEnd.y + vm->uvScrollPos.y;

    triangleX1 = max(g_QuadVertices[0].pos.x,
                     g_QuadVertices[1].pos.x);
    triangleX1 = max(g_QuadVertices[2].pos.x, triangleX1);
    triangleX1 = max(g_QuadVertices[3].pos.x, triangleX1);

    triangleY1 = max(g_QuadVertices[0].pos.y,
                     g_QuadVertices[1].pos.y);
    triangleY1 = max(g_QuadVertices[2].pos.y, triangleY1);
    triangleY1 = max(g_QuadVertices[3].pos.y, triangleY1);

    triangleX2 = min(g_QuadVertices[0].pos.x,
                     g_QuadVertices[1].pos.x);
    triangleX2 = min(g_QuadVertices[2].pos.x, triangleX2);
    triangleX2 = min(g_QuadVertices[3].pos.x, triangleX2);

    triangleY2 = min(g_QuadVertices[0].pos.y,
                     g_QuadVertices[1].pos.y);
    triangleY2 = min(g_QuadVertices[2].pos.y, triangleY2);
    triangleY2 = min(g_QuadVertices[3].pos.y, triangleY2);

    if (triangleX1 < g_Supervisor.viewport.X ||
        triangleY1 < g_Supervisor.viewport.Y ||
        triangleX2 > g_Supervisor.viewport.X + g_Supervisor.viewport.Width ||
        triangleY2 > g_Supervisor.viewport.Y + g_Supervisor.viewport.Height)
    {
        return ZUN_SUCCESS;
    }

    if (this->currentTexture != this->textures[vm->sprite->sourceFileIndex])
    {
        // Flush the queued sprites while currentTexture still names the texture they
        // were built against; switching first would submit the old batch with the new
        // texture and scramble every sprite in it.
        TextureHandle *next = this->textures[vm->sprite->sourceFileIndex];
        this->Flush();
        this->currentTexture = next;
    }
    if (this->currentVertexShader != 1)
    {
        this->Flush();
        this->currentVertexShader = 1;
    }
    if ((drawFlags & 2) == 0)
    {
        color.color =
            vm->useColor2 ? vm->color2.color : vm->color.color;
        if (this->colorMulEnabled)
        {
            color.bytes.r = ZunColor::Multiply(color.bytes.r, this->color.bytes.r);
            color.bytes.g = ZunColor::Multiply(color.bytes.g, this->color.bytes.g);
            color.bytes.b = ZunColor::Multiply(color.bytes.b, this->color.bytes.b);
            color.bytes.a = ZunColor::Multiply(color.bytes.a, this->color.bytes.a);
        }
        g_QuadVertices[0].diffuse = color;
        g_QuadVertices[1].diffuse = color;
        g_QuadVertices[2].diffuse = color;
        g_QuadVertices[3].diffuse = color;
    }
    SyncRenderState(vm);
    PushSprite(g_QuadVertices);


    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x0044f580
void AnmManager::ResetVertexBuffer()
{
    this->spritesToDraw = 0;
    this->vertexBufferCurPtr = this->spriteVertexBuffer;
    this->vertexBufferStartPtr = this->vertexBufferCurPtr;
}

// FUNCTION: TH07 0x0044f5c0
void AnmManager::Flush()
{
    if (!this->spritesToDraw)
    {
        return;
    }

    // The batcher accumulates triangle-list vertices in 0xAARRGGBB + UV form, which is
    // exactly what the renderer takes once the colours are unpacked. The scratch array
    // is sized to the source buffer so the loop below can never read past it.
    u32 vertexCount = this->spritesToDraw * 6;
    static RenderVertex scratch[ARRAY_SIZE_SIGNED(this->spriteVertexBuffer)];
    u32 capacity = (u32)(sizeof(scratch) / sizeof(scratch[0]));
    if (vertexCount > capacity)
    {
        vertexCount = capacity;
    }
    u32 n = vertexCount;

    VertexTex1DiffuseXyzrhw *src = this->vertexBufferStartPtr;
    for (u32 i = 0; i < n; i++)
    {
        RenderColor color = src[i].diffuse.color;
        RenderVertex *dst = &scratch[i];
        dst->x = src[i].pos.x;
        dst->y = src[i].pos.y;
        dst->z = src[i].pos.z;
        dst->color = color;
        dst->u = src[i].textureUV.x;
        dst->v = src[i].textureUV.y;
    }

    // The batcher stores two triangles per sprite, 6 vertices total, all with the same
    // texture, so the whole batch is one draw.
    RenderTexture *texture =
        this->currentTexture ? this->currentTexture->texture : NULL;

    RendererDrawTriangles(scratch, (i32)n, texture, texture != NULL);
    RendererDrawTriangles(scratch, (i32)n, texture, texture != NULL);


    this->vertexBufferStartPtr = this->vertexBufferCurPtr;
    this->spritesToDraw = 0;
    this->flushesThisFrame++;
}

// FUNCTION: TH07 0x0044f690
ZunResult AnmManager::PushSprite(VertexTex1DiffuseXyzrhw *spriteVertex)
{
    // The original relied on the frame ending before this buffer filled. A dense spell
    // card pushes far more sprites than that, so flush and start a new batch when the
    // next quad would not fit: `spriteVertexBuffer` holds a fixed number of entries and
    // every push writes six of them.
    i32 capacity = ARRAY_SIZE_SIGNED(this->spriteVertexBuffer);
    if (this->vertexBufferCurPtr + 6 > this->spriteVertexBuffer + capacity)
    {
        this->Flush();
        // Flush reset the cursor to the start of the buffer, so there is room now
        // unless the buffer is smaller than a single quad.
        if (this->vertexBufferCurPtr + 6 > this->spriteVertexBuffer + capacity)
        {
            return ZUN_ERROR;
        }
    }

    this->vertexBufferCurPtr[0] = spriteVertex[0];
    this->vertexBufferCurPtr[1] = spriteVertex[1];
    this->vertexBufferCurPtr[2] = spriteVertex[2];
    this->vertexBufferCurPtr[3] = spriteVertex[1];
    this->vertexBufferCurPtr[4] = spriteVertex[2];
    this->vertexBufferCurPtr[5] = spriteVertex[3];

    this->vertexBufferCurPtr = this->vertexBufferCurPtr + 6;
    this->spritesToDraw++;
    return ZUN_SUCCESS;
}

// Text queued by DrawStringFormat2, keyed by the VM it belongs to. Drawing happens when
// the VM is drawn, because that is when its screen position is known.
struct StoredText
{
    AnmVm *vm;
    char utf8[256];
    i32 fontWidth;
    i32 fontHeight;
    D3DCOLOR color;
    u32 outlineType;
};

static StoredText g_StoredText[64];
static i32 g_StoredTextCount = 0;

// Drops every remembered string. Called once per frame before the draw chain runs, so a
// VM that is gone cannot leave a stale pointer behind; DrawStringFormat2 re-adds what is
// still current during the update that precedes it.
void AnmClearStoredText()
{
    g_StoredTextCount = 0;
}

static void AnmStoreText(AnmVm *vm, const char *utf8, i32 fontWidth, i32 fontHeight,
                        D3DCOLOR color, u32 outlineType)
{
    for (i32 i = 0; i < g_StoredTextCount; i++)
    {
        if (g_StoredText[i].vm == vm)
        {
            strncpy(g_StoredText[i].utf8, utf8, sizeof(g_StoredText[i].utf8) - 1);
            g_StoredText[i].utf8[sizeof(g_StoredText[i].utf8) - 1] = 0;
            g_StoredText[i].fontWidth = fontWidth;
            g_StoredText[i].fontHeight = fontHeight;
            g_StoredText[i].color = color;
            g_StoredText[i].outlineType = outlineType;
            return;
        }
    }
    if (g_StoredTextCount >= ARRAY_SIZE_SIGNED(g_StoredText))
    {
        return;
    }
    StoredText *slot = &g_StoredText[g_StoredTextCount++];
    slot->vm = vm;
    strncpy(slot->utf8, utf8, sizeof(slot->utf8) - 1);
    slot->utf8[sizeof(slot->utf8) - 1] = 0;
    slot->fontWidth = fontWidth;
    slot->fontHeight = fontHeight;
    slot->color = color;
    slot->outlineType = outlineType;
}

static StoredText *AnmFindText(AnmVm *vm)
{
    for (i32 i = 0; i < g_StoredTextCount; i++)
    {
        if (g_StoredText[i].vm == vm)
        {
            return &g_StoredText[i];
        }
    }
    return NULL;
}

// Emits the VM's queued text onto the overlay, centred on the VM's on-screen position.
static void AnmEmitText(AnmVm *vm)
{
    StoredText *stored = AnmFindText(vm);
    if (!stored)
    {
        return;
    }

    i32 fontHeight = stored->fontHeight <= 0 ? 15 : stored->fontHeight;
    f32 spriteWidth = vm->sprite ? vm->sprite->widthPx * vm->sprite->cols : 640.0f;

    // The VM sits at the centre of its line, so the text is centred on pos.x and its
    // vertical centre on pos.y.
    i32 left = (i32)vm->pos.x;
    i32 textWidth = 0;
    FontFace *face = FontGetDefault();
    if (face)
    {
        textWidth = FontMeasureText(face, stored->utf8, (f32)fontHeight);
    }
    left -= textWidth / 2;
    i32 top = (i32)(vm->pos.y - fontHeight / 2.0f);

    (void)spriteWidth;


    TextOverlayDraw(stored->utf8, left, top, (f32)fontHeight, stored->color,
                    stored->outlineType, stored->outlineType != 0xffffffff);
}

// FUNCTION: TH07 0x0044f770
ZunResult AnmManager::DrawNoRotation(AnmVm *vm)
{
    f32 centerY;
    f32 centerX;

    if (!vm->visible)
    {
        return ZUN_ERROR;
    }

    if (!vm->active)
    {
        return ZUN_ERROR;
    }

    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    // The sprite itself is only the text's bounding box; the glyphs go on the overlay.
    // Emitting here keeps the text tied to the VM's lifetime and position without going
    // through the shared text texture.
    AnmEmitText(vm);

    centerX = vm->sprite->widthPx * vm->scale.x / 2.0f;
    centerY = vm->sprite->heightPx * vm->scale.y / 2.0f;

    if ((vm->anchor & 1) == 0)
    {
        g_QuadVertices[0].pos.x =
            g_QuadVertices[2].pos.x = vm->pos.x - centerX;
        g_QuadVertices[1].pos.x =
            g_QuadVertices[3].pos.x = centerX + vm->pos.x;
    }
    else
    {
        g_QuadVertices[0].pos.x =
            g_QuadVertices[2].pos.x = vm->pos.x;
        g_QuadVertices[1].pos.x =
            g_QuadVertices[3].pos.x = centerX + vm->pos.x +
                                      centerX;
    }

    if ((vm->anchor & 2) == 0)
    {
        g_QuadVertices[0].pos.y =
            g_QuadVertices[1].pos.y = vm->pos.y - centerY;
        g_QuadVertices[2].pos.y =
            g_QuadVertices[3].pos.y = centerY + vm->pos.y;
    }
    else
    {
        g_QuadVertices[0].pos.y =
            g_QuadVertices[1].pos.y = vm->pos.y;
        g_QuadVertices[2].pos.y =
            g_QuadVertices[3].pos.y = centerY + vm->pos.y +
                                      centerY;
    }

    g_QuadVertices[0].pos.z =
        g_QuadVertices[1].pos.z =
            g_QuadVertices[2].pos.z =
                g_QuadVertices[3].pos.z = vm->pos.z;

    return DrawInner(vm, 1);
}

// FUNCTION: TH07 0x0044f960
void AnmManager::TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, f32 width,
                                   f32 height, f32 sine, f32 cosine,
                                   f32 xOffset, f32 yOffset)
{
    vertex->pos.x = width * cosine - height * sine + xOffset;
    vertex->pos.y = width * sine + height * cosine + yOffset;
}

#pragma var_order(sinZ, z, cosZ, width, height, yOffset, xOffset)
// FUNCTION: TH07 0x0044f9a0
ZunResult AnmManager::Draw(AnmVm *vm)
{
    f32 cosZ;
    f32 sinZ;
    f32 xOffset;
    f32 yOffset;
    f32 z;
    f32 width;
    f32 height;

    if (vm->rotation.z == 0.0f)
    {
        return DrawNoRotation(vm);
    }
    if (!vm->visible)
    {
        return ZUN_ERROR;
    }
    if (!vm->active)
    {
        return ZUN_ERROR;
    }
    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    z = vm->rotation.z;
    sincosf_macro(sinZ, cosZ, z);
    xOffset = vm->pos.x;
    yOffset = vm->pos.y;
    width = vm->sprite->widthPx * vm->scale.x / 2.0f;
    height = vm->sprite->heightPx * vm->scale.y / 2.0f;

    TranslateRotation(&g_QuadVertices[0], -width, -height, sinZ,
                      cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[1], width, -height, sinZ,
                      cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[2], -width, height, sinZ,
                      cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[3], width, height, sinZ,
                      cosZ, xOffset, yOffset);

    g_QuadVertices[0].pos.z =
        g_QuadVertices[1].pos.z =
            g_QuadVertices[2].pos.z =
                g_QuadVertices[3].pos.z = vm->pos.z;
    if ((vm->anchor & 1) != 0)
    {
        g_QuadVertices[0].pos.x += width;
        g_QuadVertices[1].pos.x += width;
        g_QuadVertices[2].pos.x += width;
        g_QuadVertices[3].pos.x += width;
    }
    if ((vm->anchor & 2) != 0)
    {
        g_QuadVertices[0].pos.y += height;
        g_QuadVertices[1].pos.y += height;
        g_QuadVertices[2].pos.y += height;
        g_QuadVertices[3].pos.y += height;
    }

    return DrawInner(vm, 0);
}

// FUNCTION: TH07 0x0044fc10
ZunResult AnmManager::DrawFacingCamera(AnmVm *vm)
{
    f32 centerY;
    f32 centerX;

    if (!vm->visible)
    {
        return ZUN_ERROR;
    }
    if (!vm->active)
    {
        return ZUN_ERROR;
    }
    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    centerX = vm->sprite->widthPx * vm->scale.x / 2.0f;
    centerY = vm->sprite->heightPx * vm->scale.y / 2.0f;

    if ((vm->anchor & 1) == 0)
    {
        g_QuadVertices[0].pos.x =
            g_QuadVertices[2].pos.x = vm->pos.x - centerX;
        g_QuadVertices[1].pos.x =
            g_QuadVertices[3].pos.x = centerX + vm->pos.x;
    }
    else
    {
        g_QuadVertices[0].pos.x =
            g_QuadVertices[2].pos.x = vm->pos.x;
        g_QuadVertices[1].pos.x =
            g_QuadVertices[3].pos.x = centerX + vm->pos.x + centerX;
    }

    if ((vm->anchor & 2) == 0)
    {
        g_QuadVertices[0].pos.y =
            g_QuadVertices[1].pos.y = vm->pos.y - centerY;
        g_QuadVertices[2].pos.y =
            g_QuadVertices[3].pos.y = centerY + vm->pos.y;
    }
    else
    {
        g_QuadVertices[0].pos.y =
            g_QuadVertices[1].pos.y = vm->pos.y;
        g_QuadVertices[2].pos.y =
            g_QuadVertices[3].pos.y = centerY + vm->pos.y + centerY;
    }

    g_QuadVertices[0].pos.z =
        g_QuadVertices[1].pos.z =
            g_QuadVertices[2].pos.z =
                g_QuadVertices[3].pos.z = vm->pos.z;

    return DrawInner(vm, 0);
}

#pragma var_order(halfWidth, halfHeight, screenCenterY, halfLength, sinZ, matrix, z, \
                  projectRight, projectCenter, projectRightOffset, cosZ, origin)
// FUNCTION: TH07 0x0044fe00
ZunResult AnmManager::CalcBillboardTransform(AnmVm *vm)
{
    f32 halfWidth;
    f32 halfHeight;
    f32 screenCenterY;
    f32 halfLength; // also used as screen center x
    f32 sinZ;
    D3DXMATRIX matrix;
    f32 z = vm->rotation.z;
    D3DXVECTOR3 projectRight;
    D3DXVECTOR3 projectCenter;
    D3DXVECTOR3 projectRightOffset;
    f32 cosZ;

    sincosf_macro(sinZ, cosZ, z);

    D3DXVECTOR3 origin(0.0f, 0.0f, 0.0f);

    D3DXMatrixIdentity(&matrix);
    matrix.m[3][0] = vm->pos.x;
    matrix.m[3][1] = vm->pos.y;
    matrix.m[3][2] = vm->pos.z;

    D3DXVec3Project(&projectCenter, &origin, &g_Supervisor.viewport,
                    &g_Supervisor.projectionMatrix, &g_Supervisor.viewMatrix,
                    &matrix);

    if (projectCenter.z < 0.0f || projectCenter.z > 1.0f)
    {
        return ZUN_ERROR;
    }

    D3DXVec3Project(&projectRight, &g_Stage.cam.right, &g_Supervisor.viewport,
                    &g_Supervisor.projectionMatrix, &g_Supervisor.viewMatrix,
                    &matrix);

    projectRightOffset = projectRight - projectCenter;

    halfLength = D3DXVec3Length(&projectRightOffset) * 0.5f;
    halfWidth = halfLength * vm->sprite->widthPx * vm->scale.x;
    halfHeight = halfLength * vm->sprite->heightPx * vm->scale.y;

    halfLength = projectCenter.x; // used as screen center x here
    screenCenterY = projectCenter.y;

    TranslateRotation(&g_QuadVertices[0], -halfWidth, -halfHeight, sinZ,
                      cosZ, halfLength, screenCenterY);
    TranslateRotation(&g_QuadVertices[1], halfWidth, -halfHeight, sinZ,
                      cosZ, halfLength, screenCenterY);
    TranslateRotation(&g_QuadVertices[2], -halfWidth, halfHeight, sinZ,
                      cosZ, halfLength, screenCenterY);
    TranslateRotation(&g_QuadVertices[3], halfWidth, halfHeight, sinZ,
                      cosZ, halfLength, screenCenterY);

    g_QuadVertices[0].pos.z =
        g_QuadVertices[1].pos.z =
            g_QuadVertices[2].pos.z =
                g_QuadVertices[3].pos.z = projectCenter.z;

    if ((vm->anchor & 1) != 0)
    {
        g_QuadVertices[0].pos.x += halfWidth;
        g_QuadVertices[1].pos.x += halfWidth;
        g_QuadVertices[2].pos.x += halfWidth;
        g_QuadVertices[3].pos.x += halfWidth;
    }
    if ((vm->anchor & 2) != 0)
    {
        g_QuadVertices[0].pos.y += halfHeight;
        g_QuadVertices[1].pos.y += halfHeight;
        g_QuadVertices[2].pos.y += halfHeight;
        g_QuadVertices[3].pos.y += halfHeight;
    }
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x00450130
ZunResult AnmManager::DrawBillboard(AnmVm *vm)
{
    if (!vm->visible)
    {
        return ZUN_ERROR;
    }

    if (!vm->active)
    {
        return ZUN_ERROR;
    }

    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    if (CalcBillboardTransform(vm) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    return DrawInner(vm, 0);
}

#pragma var_order(rot, world)
// FUNCTION: TH07 0x004501a0
void AnmManager::CalcProjectedTransform(AnmVm *vm)
{
    D3DXMATRIX world;
    D3DXMATRIX rot;

    if (vm->skipTransform == 0 &&
        (vm->updateScale || vm->updateRotation))
    {
        vm->worldTransformMatrix = vm->matrix;
        vm->worldTransformMatrix.m[0][0] *= vm->scale.x;
        vm->worldTransformMatrix.m[1][1] *= vm->scale.y;
        vm->updateScale = 0;
        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&rot, vm->rotation.x);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&rot, vm->rotation.y);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&rot, vm->rotation.z);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        vm->updateRotation = 0;
    }

    world = vm->worldTransformMatrix;
    if ((vm->anchor & 1) == 0)
    {
        world.m[3][0] = vm->pos.x;
    }
    else
    {
        world.m[3][0] = fabsf(vm->sprite->widthPx * vm->scale.x / 2.0f) + vm->pos.x;
    }

    if ((vm->anchor & 2) == 0)
    {
        world.m[3][1] = vm->pos.y;
    }
    else
    {
        world.m[3][1] = fabsf(vm->sprite->heightPx * vm->scale.y / 2.0f) + vm->pos.y;
    }
    world.m[3][2] = vm->pos.z;

    D3DXVec3Project((D3DXVECTOR3 *)&g_QuadVertices[0].pos,
                    &this->vertexBufferContents[0].pos,
                    &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                    &g_Supervisor.viewMatrix, &world);
    D3DXVec3Project((D3DXVECTOR3 *)&g_QuadVertices[1].pos,
                    &this->vertexBufferContents[1].pos,
                    &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                    &g_Supervisor.viewMatrix, &world);
    D3DXVec3Project((D3DXVECTOR3 *)&g_QuadVertices[2].pos,
                    &this->vertexBufferContents[2].pos,
                    &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                    &g_Supervisor.viewMatrix, &world);
    D3DXVec3Project((D3DXVECTOR3 *)&g_QuadVertices[3].pos,
                    &this->vertexBufferContents[3].pos,
                    &g_Supervisor.viewport, &g_Supervisor.projectionMatrix,
                    &g_Supervisor.viewMatrix, &world);

    this->matrix = world;
}

// FUNCTION: TH07 0x004504b0
ZunResult AnmManager::DrawProjected(AnmVm *vm)
{
    if (!vm->visible)
    {
        return ZUN_ERROR;
    }

    if (!vm->active)
    {
        return ZUN_ERROR;
    }

    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    CalcProjectedTransform(vm);
    return DrawInner(vm, 0);
}

#pragma var_order(uv, rot, world)
// FUNCTION: TH07 0x00450520
ZunResult AnmManager::Draw3(AnmVm *vm)
{
    D3DXMATRIX world;
    D3DXMATRIX rot;
    D3DXMATRIX uv;

    if (!vm->visible)
    {
        return ZUN_ERROR;
    }

    if (!vm->active)
    {
        return ZUN_ERROR;
    }

    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    if (this->spritesToDraw != 0)
    {
        this->Flush();
    }

    if (vm->skipTransform == 0 &&
        (vm->updateScale || vm->updateRotation))
    {
        vm->worldTransformMatrix = vm->matrix;
        vm->worldTransformMatrix.m[0][0] *= vm->scale.x;
        vm->worldTransformMatrix.m[1][1] *= vm->scale.y;
        vm->updateScale = 0;

        // double intentionally used here
        if (vm->rotation.x != 0.0)
        {
            D3DXMatrixRotationX(&rot, vm->rotation.x);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        if (vm->rotation.y != 0.0)
        {
            D3DXMatrixRotationY(&rot, vm->rotation.y);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        if (vm->rotation.z != 0.0)
        {
            D3DXMatrixRotationZ(&rot, vm->rotation.z);
            D3DXMatrixMultiply(&vm->worldTransformMatrix, &vm->worldTransformMatrix,
                               &rot);
        }
        vm->updateRotation = 0;
    }

    world = vm->worldTransformMatrix;
    if ((vm->anchor & 1) == 0)
    {
        world.m[3][0] = vm->pos.x;
    }
    else
    {
        world.m[3][0] = fabsf(vm->sprite->widthPx * vm->scale.x / 2.0f) + vm->pos.x;
    }

    if ((vm->anchor & 2) == 0)
    {
        world.m[3][1] = vm->pos.y;
    }
    else
    {
        world.m[3][1] = fabsf(vm->sprite->heightPx * vm->scale.y / 2.0f) + vm->pos.y;
    }

    world.m[3][0] += this->offset.x;
    world.m[3][1] += this->offset.y;

    SetRenderStateForVm(vm);
    world.m[3][2] = vm->pos.z;

    // D3D applied this world matrix on the device and drew a unit quad. The renderer
    // takes transformed vertices, so build the quad, run it through world * view *
    // projection here, and hand the result to the renderer.
    RenderTexture *texture =
        this->textures[vm->sprite->sourceFileIndex]
            ? this->textures[vm->sprite->sourceFileIndex]->texture
            : NULL;
    if (!texture)
    {
        return ZUN_ERROR;
    }

    RenderColor color = vm->useColor2 ? vm->color2.color : vm->color.color;
    if (this->colorMulEnabled)
    {
        color = RENDER_COLOR_RGBA(
            ZunColor::Multiply((color >> 16) & 0xff, this->color.bytes.r),
            ZunColor::Multiply((color >> 8) & 0xff, this->color.bytes.g),
            ZunColor::Multiply(color & 0xff, this->color.bytes.b),
            ZunColor::Multiply((color >> 24) & 0xff, this->color.bytes.a));
    }

    f32 uvX = vm->sprite->uvStart.x + vm->uvScrollPos.x;
    f32 uvY = vm->sprite->uvStart.y + vm->uvScrollPos.y;
    f32 uvW = vm->sprite->uvEnd.x - vm->sprite->uvStart.x;
    f32 uvH = vm->sprite->uvEnd.y - vm->sprite->uvStart.y;

    // Corner order matches g_Quad3DFallback: TL, TR, BL, BR.
    static const f32 cornerX[4] = {-128.0f, 128.0f, -128.0f, 128.0f};
    static const f32 cornerY[4] = {-128.0f, -128.0f, 128.0f, 128.0f};

    RenderVertex quad[4];
    for (i32 corner = 0; corner < 4; corner++)
    {
        D3DXVECTOR3 local;
        local.x = cornerX[corner];
        local.y = cornerY[corner];
        local.z = 0.0f;

        D3DXVECTOR3 projected;
        D3DXVec3Project(&projected, &local, &g_Supervisor.viewport,
                        &g_Supervisor.projectionMatrix, &g_Supervisor.viewMatrix,
                        &world);

        quad[corner].x = projected.x;
        quad[corner].y = projected.y;
        quad[corner].z = projected.z;
        quad[corner].color = color;
        quad[corner].u = uvX + (corner == 1 || corner == 3 ? uvW : 0.0f);
        quad[corner].v = uvY + (corner == 2 || corner == 3 ? uvH : 0.0f);
    }

    // Two triangles from the quad: TL, TR, BL and TR, BR, BL.
    RenderVertex triangles[6] = {quad[0], quad[1], quad[2],
                                 quad[1], quad[3], quad[2]};
    RendererDrawTriangles(triangles, 6, texture, true);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x00450a50
f32 AnmVm::GetFloatVarValue(f32 arg)
{
    switch ((i32)arg)
    {
    case ANM_VAR_INT1_1:
        return (f32)this->intVars1[0];
    case ANM_VAR_INT1_2:
        return (f32)this->intVars1[1];
    case ANM_VAR_INT1_3:
        return (f32)this->intVars1[2];
    case ANM_VAR_INT1_4:
        return (f32)this->intVars1[3];
    case ANM_VAR_FLOAT_1:
        return this->floatVars[0];
    case ANM_VAR_FLOAT_2:
        return this->floatVars[1];
    case ANM_VAR_FLOAT_3:
        return this->floatVars[2];
    case ANM_VAR_FLOAT_4:
        return this->floatVars[3];
    case ANM_VAR_INT2_1:
        return (f32)this->intVars2[0];
    case ANM_VAR_INT2_2:
        return (f32)this->intVars2[1];
    default:
        return arg;
    }
}

// FUNCTION: TH07 0x00450b20
i32 AnmVm::GetVarValue(i32 arg)
{
    switch (arg)
    {
    case ANM_VAR_INT1_1:
        return this->intVars1[0];
    case ANM_VAR_INT1_2:
        return this->intVars1[1];
    case ANM_VAR_INT1_3:
        return this->intVars1[2];
    case ANM_VAR_INT1_4:
        return this->intVars1[3];
    case ANM_VAR_FLOAT_1:
        return this->floatVars[0];
    case ANM_VAR_FLOAT_2:
        return this->floatVars[1];
    case ANM_VAR_FLOAT_3:
        return this->floatVars[2];
    case ANM_VAR_FLOAT_4:
        return this->floatVars[3];
    case ANM_VAR_INT2_1:
        return this->intVars2[0];
    case ANM_VAR_INT2_2:
        return this->intVars2[1];
    default:
        return arg;
    }
}

// FUNCTION: TH07 0x00450c10
f32 *AnmVm::GetFloatVar(f32 *paramId, u16 mask, u32 idx)
{
    if (((u32)mask & 1 << idx) == 0)
    {
        return paramId;
    }

    switch ((u32)*paramId)
    {
    case ANM_VAR_FLOAT_1:
        return &this->floatVars[0];
    case ANM_VAR_FLOAT_2:
        return &this->floatVars[1];
    case ANM_VAR_FLOAT_3:
        return &this->floatVars[2];
    case ANM_VAR_FLOAT_4:
        return &this->floatVars[3];
    default:
        return paramId;
    }
}

// FUNCTION: TH07 0x00450ca0
i32 *AnmVm::GetVar(i32 *paramId, u16 mask, u32 idx)
{
    if (((u32)mask & 1 << idx) == 0)
    {
        return paramId;
    }

    switch (*paramId)
    {
    case ANM_VAR_INT1_1:
        return &this->intVars1[0];
    case ANM_VAR_INT1_2:
        return &this->intVars1[1];
    case ANM_VAR_INT1_3:
        return &this->intVars1[2];
    case ANM_VAR_INT1_4:
        return &this->intVars1[3];
    case ANM_VAR_INT2_1:
        return &this->intVars2[0];
    case ANM_VAR_INT2_2:
        return &this->intVars2[1];
    default:
        return paramId;
    }
}

#pragma var_order(instr, nextInstr, i, t)
// FUNCTION: TH07 0x00450d60
i32 AnmManager::ExecuteScript(AnmVm *vm)
{
    AnmRawInstr *instr;
    AnmRawInstr *nextInstr;
    i32 i;
    f32 t;

#define GET_INT_PTR(argIdx) \
    vm->GetVar(&instr->args[argIdx].i, instr->flags, argIdx)

#define GET_FLOAT_PTR(argIdx) \
    vm->GetFloatVar(&instr->args[argIdx].f, instr->flags, argIdx)

#define GET_INT_VALUE(argIdx) \
    (((instr->flags & (1 << argIdx)) != 0) ? vm->GetVarValue(instr->args[argIdx].i) : instr->args[argIdx].i)

#define GET_FLOAT_VALUE(argIdx) \
    (((instr->flags & (1 << argIdx)) != 0) ? vm->GetFloatVarValue(instr->args[argIdx].f) : instr->args[argIdx].f)

    if (!vm->currentInstruction)
    {
        return 1;
    }

    if (vm->pendingInterrupt != 0)
    {
        goto handle_interrupt;
    }

WHY_NOT_JUST_CONTINUE:
    instr = vm->currentInstruction;
    while (instr->time <= vm->currentTimeInScript.GetCurrent())
    {
        switch (instr->opcode)
        {
        case ANM_EXIT_HIDE:
        case ANM_EXIT_HIDE2:
            vm->visible = 0;
        case ANM_EXIT:
            vm->currentInstruction = NULL;
            return 1;
        case ANM_SET_ACTIVE_SPRITE:
            vm->visible = 1;
            SetActiveSprite(vm, GET_INT_VALUE(0) + this->spriteIndices[vm->anmFileIdx]);
            vm->timeOfLastSpriteSet = vm->currentTimeInScript.GetCurrent();
            break;
        case ANM_SET_SCALE:
            vm->scale.x = GET_FLOAT_VALUE(0);
            vm->scale.y = GET_FLOAT_VALUE(1);
            vm->updateScale = 1;
            break;
        case ANM_SET_ALPHA:
            vm->color.bytes.a = instr->args[0].i & 255;
            break;
        case ANM_SET_COLOR:
            vm->color.color =
                (vm->color.color & 0xff000000) | (instr->args[0].i & 0xffffff);
            break;
        case ANM_JUMP:
            vm->currentTimeInScript = instr->args[1].i;
            vm->currentInstruction =
                (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[0].i);
            goto WHY_NOT_JUST_CONTINUE;
        case ANM_DEC_JUMP:
            (*GET_INT_PTR(0))--;
            if (GET_INT_VALUE(0) > 0)
            {
                vm->currentTimeInScript = instr->args[2].i;
                vm->currentInstruction =
                    (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[1].i);
                goto WHY_NOT_JUST_CONTINUE;
            }
            break;
        case ANM_FLIP_X:
            vm->flip ^= 1;
            vm->scale.x *= -1.0f;
            vm->updateScale = 1;
            break;
        case ANM_SET_USE_OFFSET:
            vm->useOffset = instr->args[0].i;
            break;
        case ANM_FLIP_Y:
            vm->flip ^= 2;
            vm->scale.y *= -1.0f;
            vm->updateScale = 1;
            break;
        case ANM_SET_ROTATION:
            vm->rotation.x = GET_FLOAT_VALUE(0);
            vm->rotation.y = GET_FLOAT_VALUE(1);
            vm->rotation.z = GET_FLOAT_VALUE(2);
            vm->updateRotation = 1;
            break;
        case ANM_SET_ANGLE_VEL:
            vm->angleVel.x = GET_FLOAT_VALUE(0);
            vm->angleVel.y = GET_FLOAT_VALUE(1);
            vm->angleVel.z = GET_FLOAT_VALUE(2);
            vm->updateRotation = 1;
            break;
        case ANM_SET_SCALE_SPEED:
            vm->scaleGrowth.x = GET_FLOAT_VALUE(0);
            vm->scaleGrowth.y = GET_FLOAT_VALUE(1);
            break;
        case ANM_INTERP_SCALE:
            vm->interpStartTimes[4] = 0;
            vm->interpEndTimes[4] = GET_INT_VALUE(2);
            vm->easeModes[4] = 0;
            vm->scaleInterpInitial = vm->scale;
            vm->scaleInterpFinal.x = GET_FLOAT_VALUE(0);
            vm->scaleInterpFinal.y = GET_FLOAT_VALUE(1);
            break;
        case ANM_FADE:
            vm->colorInterpInitialColor.bytes.a = vm->color.bytes.a;
            vm->colorInterpFinalColor.bytes.a = instr->args[0].b[0];
            vm->interpStartTimes[2] = 0;
            vm->interpEndTimes[2] = GET_INT_VALUE(1);
            vm->easeModes[2] = 0;
            break;
        case ANM_SET_BLEND:
            vm->blendMode = instr->args[0].i;
            break;
        case ANM_SET_TRANSLATION:
            if (!vm->useOffset)
            {
                vm->pos =
                    Float3(GET_FLOAT_VALUE(0), GET_FLOAT_VALUE(1), GET_FLOAT_VALUE(2));
            }
            else
            {
                vm->offset =
                    Float3(GET_FLOAT_VALUE(0), GET_FLOAT_VALUE(1), GET_FLOAT_VALUE(2));
            }
            break;
        case ANM_POS_TIME_ACCEL:
            vm->easeModes[0] = 6;
            goto interp_pos;
        case ANM_POS_TIME_DECEL:
            vm->easeModes[0] = 4;
            goto interp_pos;
        case ANM_POS_TIME_LINEAR:
            vm->easeModes[0] = 0;
        interp_pos:
            if (!vm->useOffset)
            {
                vm->posInterpInitial = vm->pos;
            }
            else
            {
                vm->posInterpInitial = vm->offset;
            }
            vm->posInterpFinal =
                Float3(GET_FLOAT_VALUE(0), GET_FLOAT_VALUE(1), GET_FLOAT_VALUE(2));
            vm->interpEndTimes[0] = GET_INT_VALUE(3);
            vm->interpStartTimes[0] = 0;
            break;
        case ANM_WAIT:
            if (vm->waitTimer == 0)
            {
                vm->waitTimer = GET_INT_VALUE(0);
            }
            else
            {
                vm->waitTimer--;
            }
            if (vm->waitTimer <= 0)
            {
                vm->waitTimer = 0;
                break;
            }
            vm->currentTimeInScript--;
            goto stop;
        case ANM_STOP_HIDE:
            vm->visible = 0;
        case ANM_STOP:
            if (!vm->pendingInterrupt)
            {
                vm->isStopped = 1;
                vm->currentTimeInScript--;
                goto stop;
            }
        handle_interrupt:
            nextInstr = NULL;
            instr = vm->beginningOfScript;
            while ((instr->opcode != ANM_INTERRUPT_LABEL ||
                    (i32)vm->pendingInterrupt != instr->args[0].i) &&
                   instr->opcode != ANM_EXIT_HIDE)
            {
                if (instr->opcode == ANM_INTERRUPT_LABEL &&
                    instr->args[0].i == 0xffffffff)
                {
                    nextInstr = instr;
                }
                instr = (AnmRawInstr *)((u8 *)instr + instr->size);
            }
            vm->pendingInterrupt = 0;
            vm->isStopped = 0;
            if (instr->opcode != ANM_INTERRUPT_LABEL)
            {
                if (!nextInstr)
                {
                    vm->currentTimeInScript--;
                    goto stop;
                }
                instr = nextInstr;
            }

            instr = (AnmRawInstr *)((u8 *)instr + instr->size);
            vm->currentInstruction = instr;
            vm->currentTimeInScript = vm->currentInstruction->time;
            vm->visible = 1;
            goto WHY_NOT_JUST_CONTINUE;
        case ANM_SET_VISIBILITY:
            vm->visible = instr->args[0].i;
            break;
        case ANM_22:
            vm->anchor = 3;
            break;
        case ANM_SET_AUTO_ROTATE:
            vm->autoRotate = instr->args[0].us[0];
            break;
        case ANM_SET_SCROLL_POS_X:
            vm->uvScrollPos.x += GET_FLOAT_VALUE(0);
            if (vm->uvScrollPos.x >= 1.0f)
            {
                vm->uvScrollPos.x -= 1.0f;
            }
            else
            {
                if (vm->uvScrollPos.x < 0.0f)
                {
                    vm->uvScrollPos.x += 1.0f;
                }
            }
            break;
        case ANM_SET_SCROLL_POS_Y:
            vm->uvScrollPos.y += GET_FLOAT_VALUE(0);
            if (vm->uvScrollPos.y >= 1.0f)
            {
                vm->uvScrollPos.y -= 1.0f;
            }
            else
            {
                if (vm->uvScrollPos.y < 0.0f)
                {
                    vm->uvScrollPos.y += 1.0f;
                }
            }
            break;
        case ANM_SET_SCROLLVEL_X:
            vm->uvScrollVel.x = GET_FLOAT_VALUE(0);
            break;
        case ANM_SET_SCROLLVEL_Y:
            vm->uvScrollVel.y = GET_FLOAT_VALUE(0);
            break;
        case ANM_SET_ZWRITE_DISABLE:
            vm->zWriteDisable = instr->args[0].i;
            break;
        case ANM_SET_CAMERA_MODE:
            vm->cameraMode = instr->args[0].i;
            break;
        case ANM_INTERP_POS:
            vm->interpStartTimes[0] = 0;
            vm->interpEndTimes[0] = GET_INT_VALUE(0);
            vm->easeModes[0] = instr->args[1].b[0];
            if (!vm->useOffset)
            {
                vm->posInterpInitial = vm->pos;
            }
            else
            {
                vm->posInterpInitial = vm->offset;
            }
            vm->posInterpFinal.x = GET_FLOAT_VALUE(2);
            vm->posInterpFinal.y = GET_FLOAT_VALUE(3);
            vm->posInterpFinal.z = GET_FLOAT_VALUE(4);
            break;
        case ANM_INTERP_COLOR:
            vm->interpStartTimes[1] = 0;
            vm->interpEndTimes[1] = GET_INT_VALUE(0);
            vm->easeModes[1] = instr->args[1].b[0];
            vm->colorInterpInitialColor.bytes.r = vm->color.bytes.r;
            vm->colorInterpInitialColor.bytes.g = vm->color.bytes.g;
            vm->colorInterpInitialColor.bytes.b = vm->color.bytes.b;
            vm->colorInterpFinalColor.bytes.r = instr->args[2].b[0];
            vm->colorInterpFinalColor.bytes.g = instr->args[2].b[1];
            vm->colorInterpFinalColor.bytes.b = instr->args[2].b[2];
            break;
        case ANM_INTERP_ALPHA:
            vm->interpStartTimes[2] = 0;
            vm->interpEndTimes[2] = GET_INT_VALUE(0);
            vm->easeModes[2] = instr->args[1].b[0];
            vm->colorInterpInitialColor.bytes.a = vm->color.bytes.a;
            vm->colorInterpFinalColor.bytes.a = instr->args[2].b[0];
            break;
        case ANM_INTERP_ROTATE:
            vm->interpStartTimes[3] = 0;
            vm->interpEndTimes[3] = GET_INT_VALUE(0);
            vm->easeModes[3] = instr->args[1].b[0];
            vm->rotateInterpInitial = vm->rotation;
            vm->rotateInterpFinal.x = GET_FLOAT_VALUE(2);
            vm->rotateInterpFinal.y = GET_FLOAT_VALUE(3);
            vm->rotateInterpFinal.z = GET_FLOAT_VALUE(4);
            vm->updateRotation = 1;
            break;
        case ANM_INTERP_SCALE_2:
            vm->interpStartTimes[4] = 0;
            vm->interpEndTimes[4] = GET_INT_VALUE(0);
            vm->easeModes[4] = instr->args[1].b[0];
            vm->scaleInterpInitial = vm->scale;
            vm->scaleInterpFinal.x = GET_FLOAT_VALUE(2);
            vm->scaleInterpFinal.y = GET_FLOAT_VALUE(3);
            vm->updateScale = 1;
            break;
        case ANM_MOV:
            *GET_INT_PTR(0) = GET_INT_VALUE(1);
            break;
        case ANM_MOV_FLOAT:
            *GET_FLOAT_PTR(0) = GET_FLOAT_VALUE(1);
            break;
        case ANM_ADD_2:
            *GET_INT_PTR(0) = GET_INT_VALUE(1) + GET_INT_VALUE(2);
            break;
        case ANM_ADD_FLOAT_2:
            *GET_FLOAT_PTR(0) = GET_FLOAT_VALUE(1) + GET_FLOAT_VALUE(2);
            break;
        case ANM_SUB_2:
            *GET_INT_PTR(0) = GET_INT_VALUE(1) - GET_INT_VALUE(2);
            break;
        case ANM_SUB_FLOAT_2:
            *GET_FLOAT_PTR(0) = GET_FLOAT_VALUE(1) - GET_FLOAT_VALUE(2);
            break;
        case ANM_MUL_2:
            *GET_INT_PTR(0) = GET_INT_VALUE(1) * GET_INT_VALUE(2);
            break;
        case ANM_MUL_FLOAT_2:
            *GET_FLOAT_PTR(0) = GET_FLOAT_VALUE(1) * GET_FLOAT_VALUE(2);
            break;
        case ANM_DIV_2:
            *GET_INT_PTR(0) = GET_INT_VALUE(1) / GET_INT_VALUE(2);
            break;
        case ANM_DIV_FLOAT_2:
            *GET_FLOAT_PTR(0) = GET_FLOAT_VALUE(1) / GET_FLOAT_VALUE(2);
            break;
        case ANM_MOD_2:
            *GET_INT_PTR(0) = GET_INT_VALUE(1) % GET_INT_VALUE(2);
            break;
        case ANM_MOD_FLOAT_2:
            *GET_FLOAT_PTR(0) = fmodf(GET_FLOAT_VALUE(1), GET_FLOAT_VALUE(2));
            break;
        case ANM_ADD:
            *GET_INT_PTR(0) += GET_INT_VALUE(1);
            break;
        case ANM_ADD_FLOAT:
            *GET_FLOAT_PTR(0) += GET_FLOAT_VALUE(1);
            break;
        case ANM_SUB:
            *GET_INT_PTR(0) -= GET_INT_VALUE(1);
            break;
        case ANM_SUB_FLOAT:
            *GET_FLOAT_PTR(0) -= GET_FLOAT_VALUE(1);
            break;
        case ANM_MUL:
            *GET_INT_PTR(0) *= GET_INT_VALUE(1);
            break;
        case ANM_MUL_FLOAT:
            *GET_FLOAT_PTR(0) *= GET_FLOAT_VALUE(1);
            break;
        case ANM_DIV:
            *GET_INT_PTR(0) /= GET_INT_VALUE(1);
            break;
        case ANM_DIV_FLOAT:
            *GET_FLOAT_PTR(0) /= GET_FLOAT_VALUE(1);
            break;
        case ANM_MOD:
            *GET_INT_PTR(0) %= GET_INT_VALUE(1);
            break;
        case ANM_MOD_FLOAT:
            *GET_FLOAT_PTR(0) = fmodf(GET_FLOAT_VALUE(0), GET_FLOAT_VALUE(1));
            break;
        case ANM_RAND:
            *GET_INT_PTR(0) = g_Rng.GetRandomU32InRange(GET_INT_VALUE(1));
            break;
        case ANM_RAND_FLOAT:
            *GET_FLOAT_PTR(0) = g_Rng.GetRandomFloatInRange(GET_FLOAT_VALUE(1));
            break;
        case ANM_SIN:
            *GET_FLOAT_PTR(0) = sinf(GET_FLOAT_VALUE(1));
            break;
        case ANM_COS:
            *GET_FLOAT_PTR(0) = cosf(GET_FLOAT_VALUE(1));
            break;
        case ANM_TAN:
            *GET_FLOAT_PTR(0) = tanf(GET_FLOAT_VALUE(1));
            break;
        case ANM_ACOS:
            *GET_FLOAT_PTR(0) = acosf(GET_FLOAT_VALUE(1));
            break;
        case ANM_ATAN:
            *GET_FLOAT_PTR(0) = atanf(GET_FLOAT_VALUE(1));
            break;
        case ANM_NORMALIZE_ANGLE:
            *GET_FLOAT_PTR(0) = AddNormalizeAngle(GET_FLOAT_VALUE(0), 0.0f);
            break;
        case ANM_JUMP_IF_EQ:
            if (GET_INT_VALUE(0) == GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_EQ_FLOAT:
            if (GET_FLOAT_VALUE(0) == GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_NEQ:
            if (GET_INT_VALUE(0) != GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_NEQ_FLOAT:
            if (GET_FLOAT_VALUE(0) != GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_LT:
            if (GET_INT_VALUE(0) < GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_LT_FLOAT:
            if (GET_FLOAT_VALUE(0) < GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_LEQ:
            if (GET_INT_VALUE(0) <= GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_LEQ_FLOAT:
            if (GET_FLOAT_VALUE(0) <= GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_GT:
            if (GET_INT_VALUE(0) > GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_GT_FLOAT:
            if (GET_FLOAT_VALUE(0) > GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_GEQ:
            if (GET_INT_VALUE(0) >= GET_INT_VALUE(1))
            {
                goto jump;
            }
            break;
        case ANM_JUMP_IF_GEQ_FLOAT:
            if (GET_FLOAT_VALUE(0) >= GET_FLOAT_VALUE(1))
            {
                goto jump;
            }
            break;
        jump:
            vm->currentTimeInScript = instr->args[3].i;
            vm->currentInstruction = (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[2].i);
            goto WHY_NOT_JUST_CONTINUE;
        default:
            break;
        }
        vm->currentInstruction = (AnmRawInstr *)((u8 *)instr + instr->size);
        goto WHY_NOT_JUST_CONTINUE;
    }

stop:
    if (vm->angleVel.x != 0.0f)
    {
        vm->rotation.x = AddNormalizeAngle(
            vm->rotation.x,
            g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.x);
        vm->updateRotation = 1;
    }
    if (vm->angleVel.y != 0.0f)
    {
        vm->rotation.y = AddNormalizeAngle(
            vm->rotation.y,
            g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.y);
        vm->updateRotation = 1;
    }
    if (vm->angleVel.z != 0.0f)
    {
        vm->rotation.z = AddNormalizeAngle(
            vm->rotation.z,
            g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.z);
        vm->updateRotation = 1;
    }
    for (i = 0; i < ARRAY_SIZE_SIGNED(vm->interpStartTimes); i++)
    {
        if (vm->interpEndTimes[i] > 0)
        {
            vm->interpStartTimes[i]++;
            if (vm->interpStartTimes[i] >= vm->interpEndTimes[i].GetCurrent())
            {
                t = 1.0f;
                vm->interpEndTimes[i] = 0;
            }
            else
            {
                t = vm->interpStartTimes[i].AsFloat() /
                    vm->interpEndTimes[i].AsFloat();
            }
            switch (vm->easeModes[i])
            {
            case ANM_EASE_IN_QUAD:
                t = t * t;
                break;
            case ANM_EASE_IN_CUBIC:
                t = t * t * t;
                break;
            case ANM_EASE_IN_QUART:
                t = t * t;
                t = t * t;
                break;
            case ANM_EASE_OUT_QUAD:
                t = 1.0f - t;
                t = t * t;
                t = 1.0f - t;
                break;
            case ANM_EASE_OUT_CUBIC:
                t = 1.0f - t;
                t = t * t * t;
                t = 1.0f - t;
                break;
            case ANM_EASE_OUT_QUART:
                t = 1.0f - t;
                t = t * t;
                t = t * t;
                t = 1.0f - t;
                break;
            }
            switch (i)
            {
            case 0:
                if (!vm->useOffset)
                {
                    vm->pos.x = (vm->posInterpFinal.x - vm->posInterpInitial.x) * t + vm->posInterpInitial.x;
                    vm->pos.y = (vm->posInterpFinal.y - vm->posInterpInitial.y) * t + vm->posInterpInitial.y;
                    vm->pos.z = (vm->posInterpFinal.z - vm->posInterpInitial.z) * t + vm->posInterpInitial.z;
                }
                else
                {
                    vm->offset.x = (vm->posInterpFinal.x - vm->posInterpInitial.x) * t + vm->posInterpInitial.x;
                    vm->offset.y = (vm->posInterpFinal.y - vm->posInterpInitial.y) * t + vm->posInterpInitial.y;
                    vm->offset.z = (vm->posInterpFinal.z - vm->posInterpInitial.z) * t + vm->posInterpInitial.z;
                }
                break;
            case 1:
                vm->color.bytes.r =
                    (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.r -
                               (i32)vm->colorInterpInitialColor.bytes.r) *
                             t +
                         (f32)vm->colorInterpInitialColor.bytes.r);
                vm->color.bytes.g =
                    (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.g -
                               (i32)vm->colorInterpInitialColor.bytes.g) *
                             t +
                         (f32)vm->colorInterpInitialColor.bytes.g);
                vm->color.bytes.b =
                    (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.b -
                               (i32)vm->colorInterpInitialColor.bytes.b) *
                             t +
                         (f32)vm->colorInterpInitialColor.bytes.b);
                break;
            case 2:
                vm->color.bytes.a =
                    (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.a -
                               (i32)vm->colorInterpInitialColor.bytes.a) *
                             t +
                         (f32)vm->colorInterpInitialColor.bytes.a);
                break;
            case 3:
                vm->rotation.x = AddNormalizeAngle(
                    (vm->rotateInterpFinal.x - vm->rotateInterpInitial.x) * t,
                    vm->rotateInterpInitial.x);
                vm->rotation.y = AddNormalizeAngle(
                    (vm->rotateInterpFinal.y - vm->rotateInterpInitial.y) * t,
                    vm->rotateInterpInitial.y);
                vm->rotation.z = AddNormalizeAngle(
                    (vm->rotateInterpFinal.z - vm->rotateInterpInitial.z) * t,
                    vm->rotateInterpInitial.z);
                vm->updateRotation = 1;
                break;
            case 4:
                vm->scale.x = (vm->scaleInterpFinal.x - vm->scaleInterpInitial.x) * t +
                              vm->scaleInterpInitial.x;
                vm->scale.y = (vm->scaleInterpFinal.y - vm->scaleInterpInitial.y) * t +
                              vm->scaleInterpInitial.y;
                vm->updateScale = 1;
                break;
            }
        }
    }
    if (vm->scaleGrowth.y != 0.0f)
    {
        vm->scale.y +=
            g_Supervisor.effectiveFramerateMultiplier * vm->scaleGrowth.y;
        vm->updateScale = 1;
    }
    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scale.x +=
            g_Supervisor.effectiveFramerateMultiplier * vm->scaleGrowth.x;
        vm->updateScale = 1;
        vm->updateRotation = 1;
    }
    vm->uvScrollPos.x += vm->uvScrollVel.x;
    if (vm->uvScrollPos.x >= 1.0f)
    {
        vm->uvScrollPos.x -= 1.0f;
    }
    else if (vm->uvScrollPos.x < 0.0f)
    {
        vm->uvScrollPos.x += 1.0f;
    }
    vm->uvScrollPos.y += vm->uvScrollVel.y;
    if (vm->uvScrollPos.y >= 1.0f)
    {
        vm->uvScrollPos.y -= 1.0f;
    }
    else if (vm->uvScrollPos.y < 0.0f)
    {
        vm->uvScrollPos.y += 1.0f;
    }
    vm->currentTimeInScript++;
    this->scriptTicksThisFrame++;
    return 0;
}

// FUNCTION: TH07 0x00454260
void AnmManager::DrawTextToSprite(u32 spriteDstIdx, i32 x, i32 y, i32 width,
                                  i32 height, i32 fontWidth, i32 fontHeight,
                                  D3DCOLOR textColor, u32 outlineType,
                                  char *strToPrint, f32 scaleY, f32 scaleX)
{
    if (fontWidth <= 0)
    {
        fontWidth = 15;
    }
    if (fontHeight <= 0)
    {
        fontHeight = 15;
    }

    // Hold each argument in a local before the call. Evaluating the casts inline let
    // the float arguments be laid out against the wrong integer parameters, and the
    // sprite geometry arrived at the text renderer as garbage.
    i32 dstX = x;
    i32 dstY = y;
    i32 dstWidth = width;
    i32 dstHeight = height;
    i32 scaledFontHeight = (i32)((f32)fontHeight * scaleX);
    i32 scaledFontWidth = (i32)((f32)fontWidth * scaleY);

    TextHelper::RenderTextToTextureBold(dstX, dstY, dstWidth, dstHeight,
                                        scaledFontHeight, scaledFontWidth, textColor,
                                        outlineType, strToPrint,
                                        this->textures[spriteDstIdx]);
}

#pragma var_order(args, text, fontWidth)
// FUNCTION: TH07 0x004542d0
void AnmManager::DrawVmTextFmt(AnmManager *manager, AnmVm *vm,
                               D3DCOLOR textColor, u32 outlineType,
                               const char *str, ...)
{
    u32 fontWidth;
    char text[72];
    va_list args;

    fontWidth = vm->fontWidth;

    va_start(args, str);
    vsprintf(text, str, args);
    va_end(args);

    manager->DrawTextToSprite(
        vm->sprite->sourceFileIndex,
        vm->sprite->startPixelInclusive.x,
        vm->sprite->startPixelInclusive.y,
        vm->sprite->textureWidth,
        vm->sprite->textureHeight,
        fontWidth,
        vm->fontHeight,
        textColor,
        outlineType,
        text,
        vm->sprite->cols,
        vm->sprite->rows);

    vm->visible = 1;
}

#pragma var_order(args, x, buf, fontWidth)
// FUNCTION: TH07 0x004543b0
void AnmManager::DrawStringFormat(AnmVm *vm, D3DCOLOR textColor,
                                  u32 outlineType, const char *text, ...)
{
    i32 fontWidth;
    char buf[72];
    i32 x;
    va_list args;

    fontWidth = vm->fontWidth <= 0 ? 15 : (u32)vm->fontWidth;
    va_start(args, text);
    vsprintf(buf, text, args);
    va_end(args);

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, vm->sprite->startPixelInclusive.x,
                           vm->sprite->startPixelInclusive.y, vm->sprite->textureWidth,
                           vm->sprite->textureHeight, fontWidth, vm->fontHeight, textColor,
                           outlineType, (char *)" ", vm->sprite->cols, vm->sprite->rows);

    x = vm->sprite->startPixelInclusive.x + vm->sprite->widthPx * vm->sprite->cols -
        (f32)strlen(buf) * (f32)fontWidth * vm->sprite->cols / 2.0f;

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, x, vm->sprite->startPixelInclusive.y,
                           vm->sprite->textureWidth, vm->sprite->textureHeight, fontWidth,
                           vm->fontHeight, textColor, outlineType, buf, vm->sprite->cols,
                           vm->sprite->rows);

    vm->visible = 1;
}

#pragma var_order(args, x, buf, fontWidth)
// FUNCTION: TH07 0x004545b0
void AnmManager::DrawStringFormat2(AnmVm *vm, D3DCOLOR textColor,
                                   u32 outlineType, const char *text, ...)
{
    i32 fontWidth;
    char buf[256];
    va_list args;

    fontWidth = vm->fontWidth <= 0 ? 15 : (i32)vm->fontWidth;

    va_start(args, text);
    vsprintf(buf, text, args);
    va_end(args);

    // Remember the string against this VM. The text is drawn when the VM itself is drawn,
    // because only then is its on-screen position known, and it is composited on the
    // text overlay rather than into a sprite texture.
    AnmStoreText(vm, buf, fontWidth, vm->fontHeight <= 0 ? 15 : (i32)vm->fontHeight,
                 textColor, outlineType);

    vm->visible = 1;
}

// FUNCTION: TH07 0x004547b0
ZunResult AnmManager::LoadSurface(i32 surfaceIdx, const char *path)
{
    i32 width;
    i32 height;
    u8 *data;
    u8 *rgba;

    if (this->surfaces[surfaceIdx])
    {
        ReleaseSurface(surfaceIdx);
    }

    data = FileSystem::OpenFile(path, 0);
    if (!data)
    {
        return ZUN_ERROR;
    }

    // The ending artwork is JPEG, decoded here instead of by D3DX.
    rgba = DecodeImageToRGBA(data, g_LastFileSize, &width, &height);
    ZUN_FREE(data);
    if (!rgba)
    {
        return ZUN_ERROR;
    }

    this->surfaceSourceInfo[surfaceIdx].width = (u32)width;
    this->surfaceSourceInfo[surfaceIdx].height = (u32)height;

    // Two surfaces keep the same content: the original uses one as the source that is
    // copied back to the screen and the other as the scratch both copies read from.
    this->surfaces[surfaceIdx] = SurfaceHandleCreate((u32)width, (u32)height, 0);
    this->surfacesBis[surfaceIdx] = SurfaceHandleCreate((u32)width, (u32)height, 0);
    if (!this->surfaces[surfaceIdx] || !this->surfacesBis[surfaceIdx])
    {
        free(rgba);
        ReleaseSurface(surfaceIdx);
        return ZUN_ERROR;
    }

    memcpy(this->surfaces[surfaceIdx]->surface->pixels, rgba,
           (size_t)width * (size_t)height * 4);
    memcpy(this->surfacesBis[surfaceIdx]->surface->pixels, rgba,
           (size_t)width * (size_t)height * 4);
    SurfaceHandleCommit(this->surfaces[surfaceIdx]);
    SurfaceHandleCommit(this->surfacesBis[surfaceIdx]);

    free(rgba);
    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x00454a10
void AnmManager::ReleaseSurface(i32 surfaceIdx)
{
    if (this->surfaces[surfaceIdx])
    {
        this->surfaces[surfaceIdx]->Release();
        this->surfaces[surfaceIdx] = NULL;
    }
    if (this->surfacesBis[surfaceIdx])
    {
        this->surfacesBis[surfaceIdx]->Release();
        this->surfacesBis[surfaceIdx] = NULL;
    }
}

// FUNCTION: TH07 0x00454aa0
void AnmManager::CopySurfaceToBackBuffer(i32 surfaceIdx, i32 left, i32 top,
                                         i32 x, i32 y)
{
    SurfaceHandle *source = this->surfaces[surfaceIdx];
    if (!source)
    {
        source = this->surfacesBis[surfaceIdx];
    }
    if (!source)
    {
        return;
    }

    // The original copied this region straight into the back buffer. Drawing it is the
    // GL equivalent, and it must not disturb the batcher's texture binding.
    this->Flush();
    RenderSurfaceBlitToScreen(source->surface, x, y, left, top,
                              (i32)this->surfaceSourceInfo[surfaceIdx].width,
                              (i32)this->surfaceSourceInfo[surfaceIdx].height);
}

// FUNCTION: TH07 0x00454c60
void AnmManager::DrawEndingRect(i32 surfaceIdx, i32 rectX, i32 rectY,
                                i32 rectLeft, i32 rectTop, i32 width,
                                i32 height)
{
    SurfaceHandle *source = this->surfaces[surfaceIdx];
    if (!source)
    {
        source = this->surfacesBis[surfaceIdx];
    }
    if (!source)
    {
        return;
    }

    this->Flush();
    RenderSurfaceBlitToScreen(source->surface, rectX, rectY, rectLeft, rectTop,
                              width, height);
}

// FUNCTION: TH07 0x00454e10
void AnmManager::TakeScreenshot(i32 textureId, i32 srcLeft, i32 srcTop,
                                i32 srcWidth, i32 srcHeight, i32 dstLeft,
                                i32 dstTop, i32 dstWidth, i32 dstHeight)
{
    TextureHandle *dst = this->textures[textureId];
    if (!dst || !dst->texture)
    {
        return;
    }

    // Everything queued must be in the framebuffer before it is captured.
    Flush();

    // The original copied a region of the back buffer into the destination texture at
    // an offset. GL can only read the current framebuffer back to (0,0), so capture
    // first and then copy the wanted sub-rectangle inside the CPU mirror.
    u32 captureWidth = (u32)dst->width;
    u32 captureHeight = (u32)dst->height;
    RenderTexture *capture = RenderSurfaceCreate(captureWidth, captureHeight);
    if (!capture)
    {
        return;
    }

    RendererCaptureBackbuffer(capture);

    // glCopyTexSubImage2D reads bottom-up; flip while copying into the destination.
    if (!dst->texture->pixels)
    {
        dst->texture->pixels = (u8 *)malloc((size_t)captureWidth * captureHeight * 4);
    }
    if (!dst->texture->pixels)
    {
        RenderTextureDestroy(capture);
        return;
    }

    // The destination texture is written at dstLeft/dstTop with the given size, so
    // scale the captured region into it row by row.
    for (i32 row = 0; row < dstHeight; row++)
    {
        i32 srcRow = srcTop + (i32)((f32)row * (f32)srcHeight / (f32)dstHeight);
        i32 dstRow = dstTop + row;
        if (srcRow < 0 || srcRow >= (i32)captureHeight)
        {
            continue;
        }
        if (dstRow < 0 || dstRow >= dst->height)
        {
            continue;
        }
        u8 *dstLine = dst->texture->pixels + (size_t)dstRow * dst->width * 4 +
                      (size_t)dstLeft * 4;
        u8 *srcLine = capture->pixels + (size_t)srcRow * captureWidth * 4 +
                      (size_t)srcLeft * 4;
        i32 copyPixels = dstWidth;
        if (dstLeft + copyPixels > dst->width)
        {
            copyPixels = dst->width - dstLeft;
        }
        if (srcLeft + copyPixels > (i32)captureWidth)
        {
            copyPixels = (i32)captureWidth - srcLeft;
        }
        if (copyPixels > 0)
        {
            memcpy(dstLine, srcLine, (size_t)copyPixels * 4);
        }
    }

    RenderTextureUpload(dst->texture, dst->texture->pixels);
    RenderTextureDestroy(capture);
}

// FUNCTION: TH07 0x00454f30
void AnmManager::CopyTexture(i32 dstIdx, i32 srcIdx, RECT *dstRect, RECT *srcRect)
{
    TextureHandle *dst = this->textures[dstIdx];
    TextureHandle *src = this->textures[srcIdx];
    if (!dst || !src || !dst->texture || !src->texture)
    {
        return;
    }

    this->Flush();

    // Both sides may be GPU-only, so give them CPU mirrors to copy through.
    if (!dst->texture->pixels)
    {
        dst->texture->pixels = (u8 *)malloc((size_t)dst->width * dst->height * 4);
        if (!dst->texture->pixels)
        {
            return;
        }
        memset(dst->texture->pixels, 0, (size_t)dst->width * dst->height * 4);
    }
    if (!src->texture->pixels)
    {
        src->texture->pixels = (u8 *)malloc((size_t)src->width * src->height * 4);
        if (!src->texture->pixels)
        {
            return;
        }
        memset(src->texture->pixels, 0, (size_t)src->width * src->height * 4);
    }

    i32 copyWidth = (i32)(dstRect->right - dstRect->left);
    i32 copyHeight = (i32)(dstRect->bottom - dstRect->top);
    RenderSurfaceCopy(dst->texture, (i32)dstRect->left, (i32)dstRect->top,
                      src->texture, (i32)srcRect->left, (i32)srcRect->top,
                      copyWidth, copyHeight);
    RenderTextureUpload(dst->texture, dst->texture->pixels);
}

// FUNCTION: TH07 0x00455030
void AnmManager::SetInterruptActiveVms(AnmVm *vm, i32 vmCount, i16 interrupt)
{
    i32 shouldSetInterrupt;

    while (vmCount != 0)
    {
        if (!vm->sprite)
        {
            shouldSetInterrupt = false;
        }
        else if (vm->sprite->sourceFileIndex < 0)
        {
            shouldSetInterrupt = false;
        }
        else
        {
            shouldSetInterrupt = g_AnmManager->textures[vm->sprite->sourceFileIndex] != NULL;
        }
        if (shouldSetInterrupt)
        {
            vm->pendingInterrupt = interrupt;
        }
        vm++;
        vmCount--;
    }
}

// FUNCTION: TH07 0x004550c0
void AnmManager::ExecuteScripts(AnmVm *startVm, i32 count)
{
    while (count != 0)
    {
        if (startVm->anmFileIdx >= 0)
        {
            g_AnmManager->ExecuteScript(startVm);
        }
        startVm++;
        count--;
    }
}

// FUNCTION: TH07 0x00455110
void AnmManager::ExecuteVmsAnms(AnmVm *vm, i32 idx, i32 vmCount)
{
    while (vmCount != 0)
    {
        g_AnmManager->ExecuteAnmIdx(vm, idx);
        vm->baseSpriteIdx = vm->activeSpriteIdx;
        idx++;
        vm++;
        vmCount--;
    }
}

#pragma var_order(uvY, i, vertex, startuvX, uvX, uvStepX, num)
// FUNCTION: TH07 0x00455170
ZunResult AnmManager::UpdateTrail(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices,
                                  i32 count)
{
    f32 num;
    f32 uvStepX;
    f32 uvX;
    f32 startuvX;
    VertexTex1DiffuseXyzrhw *vertex;
    i32 i;
    f32 uvY;
    if (count < 3)
    {
        return ZUN_ERROR;
    }

    startuvX = vm->sprite->uvEnd.x + vm->uvScrollPos.x;
    num = vm->sprite->uvEnd.x - vm->sprite->uvStart.x;
    uvY = vm->sprite->uvStart.y + vm->uvScrollPos.y;
    vertex = vertices;
    uvStepX = num / (float)((count + 1) / 2 - 1);

    for (i = 0, uvX = startuvX; i < count; i += 2, vertex += 2, uvX -= uvStepX)
    {
        vertex->textureUV.x = uvX;
        vertex->textureUV.y = uvY;
        vertex->diffuse.color = vm->color.color;
        vertex->w = 1.0f;
    }

    uvY = vm->sprite->uvEnd.y + vm->uvScrollPos.y;
    vertex = vertices + 1;

    for (i = 1, uvX = startuvX; i < count; i += 2, vertex += 2, uvX -= uvStepX)
    {
        vertex->textureUV.x = uvX;
        vertex->textureUV.y = uvY;
        vertex->diffuse.color = vm->color.color;
        vertex->w = 1.0f;
    }

    return ZUN_SUCCESS;
}

// FUNCTION: TH07 0x004552d0
ZunResult AnmManager::DrawTriangleStrip(AnmVm *vm,
                                        VertexTex1DiffuseXyzrhw *vertices,
                                        i32 count)
{
    if (!vm->visible)
    {
        return ZUN_ERROR;
    }

    if (!vm->active)
    {
        return ZUN_ERROR;
    }

    if (!vm->color.bytes.a)
    {
        return ZUN_ERROR;
    }

    if (this->spritesToDraw != 0)
    {
        this->Flush();
    }

    this->currentTexture = this->textures[vm->sprite->sourceFileIndex];
    SetRenderStateForVm(vm);

    // The caller supplies a triangle strip; GL wants triangles, so expand it here.
    RenderTexture *texture = this->currentTexture ? this->currentTexture->texture : NULL;
    if (!texture || count < 3)
    {
        return ZUN_ERROR;
    }

    for (i32 i = 0; i + 2 < count; i++)
    {
        RenderVertex triangle[3];
        for (i32 corner = 0; corner < 3; corner++)
        {
            const VertexTex1DiffuseXyzrhw &v = vertices[i + corner];
            triangle[corner].x = v.pos.x;
            triangle[corner].y = v.pos.y;
            triangle[corner].z = v.pos.z;
            triangle[corner].color = v.diffuse.color;
            triangle[corner].u = v.textureUV.x;
            triangle[corner].v = v.textureUV.y;
        }
        RendererDrawTriangles(triangle, 3, texture, true);
    }
    return ZUN_SUCCESS;
}
