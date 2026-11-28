#include "script/AnmManager.hpp"

#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "script/AnmVm.hpp"
#include "data/FileSystem.hpp"
#include "core/GameErrorContext.hpp"
#include "core/GameWindow.hpp"
#include "core/Rng.hpp"
#include "game/Stage.hpp"
#include "core/Supervisor.hpp"
#include "ui/TextHelper.hpp"
#include "core/ZunMath.hpp"
#include "render/ZunGraphics.hpp"
#include "core/utils.hpp"
#include "script/AnmEntryFile.hpp"
#include "script/AnmOp.hpp"
#include "script/AnmScript.hpp"
#include "script/AnmVmDebug.hpp"

AnmManager *g_AnmManager;

VertexTex1DiffuseXyzrhw g_QuadVertices[4];

VertexTex1Xyzrhw g_QuadTemplate[4];

VertexTex1DiffuseXyz g_Quad3DFallback[4];

AnmManager::AnmManager()
{
    memset((void *)this, 0, sizeof(AnmManager));

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(this->sprites); i++)
    {
        this->sprites[i].sourceFileIndex = -1;
    }
    g_QuadTemplate[0].w = g_QuadTemplate[1].w = g_QuadTemplate[2].w = g_QuadTemplate[3].w = 1.0f;
    g_QuadTemplate[0].textureUV.x = 0.0f;
    g_QuadTemplate[0].textureUV.y = 0.0f;
    g_QuadTemplate[1].textureUV.x = 1.0f;
    g_QuadTemplate[1].textureUV.y = 0.0f;
    g_QuadTemplate[2].textureUV.x = 0.0f;
    g_QuadTemplate[2].textureUV.y = 1.0f;
    g_QuadTemplate[3].textureUV.x = 1.0f;
    g_QuadTemplate[3].textureUV.y = 1.0f;
    g_QuadVertices[0].w = g_QuadVertices[1].w = g_QuadVertices[2].w = g_QuadVertices[3].w = 1.0f;
    g_QuadVertices[0].textureUV.x = 0.0f;
    g_QuadVertices[0].textureUV.y = 0.0f;
    g_QuadVertices[1].textureUV.x = 1.0f;
    g_QuadVertices[1].textureUV.y = 0.0f;
    g_QuadVertices[2].textureUV.x = 0.0f;
    g_QuadVertices[2].textureUV.y = 1.0f;
    g_QuadVertices[3].textureUV.x = 1.0f;
    g_QuadVertices[3].textureUV.y = 1.0f;

    this->currentTexture = 0;
    this->currentBlendMode = 0;
    this->currentColorOp = 0;
    this->currentTextureFactor.color = 1;
    this->currentVertexShader = 0;
    this->currentCameraMode = 255;
    this->currentZWriteDisable = 0;
    this->screenshotTextureId = -1;
}

AnmManager::~AnmManager()
{
}

void AnmManager::SetupVertexBuffer()
{
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

    g_Quad3DFallback[0].pos = this->vertexBufferContents[0].pos;
    g_Quad3DFallback[1].pos = this->vertexBufferContents[1].pos;
    g_Quad3DFallback[2].pos = this->vertexBufferContents[2].pos;
    g_Quad3DFallback[3].pos = this->vertexBufferContents[3].pos;
    g_Quad3DFallback[0].textureUV.x = this->vertexBufferContents[0].textureUV.x;
    g_Quad3DFallback[0].textureUV.y = this->vertexBufferContents[0].textureUV.y;
    g_Quad3DFallback[1].textureUV.x = this->vertexBufferContents[1].textureUV.x;
    g_Quad3DFallback[1].textureUV.y = this->vertexBufferContents[1].textureUV.y;
    g_Quad3DFallback[2].textureUV.x = this->vertexBufferContents[2].textureUV.x;
    g_Quad3DFallback[2].textureUV.y = this->vertexBufferContents[2].textureUV.y;
    g_Quad3DFallback[3].textureUV.x = this->vertexBufferContents[3].textureUV.x;
    g_Quad3DFallback[3].textureUV.y = this->vertexBufferContents[3].textureUV.y;
}

ZunResult AnmManager::LoadTexture(i32 textureIdx, const char *texturePath, u32 colorKey)
{
    u8 *srcData;

    ReleaseTexture(textureIdx);

    // Through the asset store, like every other file: the textures are converted assets under
    // assets/, and the store is what knows where a name lives. Asking the filesystem directly
    // looked for them relative to the working directory, where they are not.
    srcData = FileSystem::OpenFile(texturePath, 0);
    if (!srcData)
    {
        return ZUN_ERROR;
    }

    SDL_IOStream *rw = SDL_IOFromMem(srcData, g_LastFileSize);
    SDL_Surface *surface = IMG_Load_IO(rw, 1);
    free(srcData);

    if (!surface)
    {
        return ZUN_ERROR;
    }

    if (colorKey != 0)
    {
        SDL_SetSurfaceColorKey(surface, true,
                               SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), NULL,
                                          (colorKey >> 16) & 0xFF, (colorKey >> 8) & 0xFF,
                                          colorKey & 0xFF));
    }

    SDL_Surface *converted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surface);

    this->textures[textureIdx] = g_Supervisor.gfxDevice->CreateTexture();
    g_Supervisor.gfxDevice->BindTexture(this->textures[textureIdx]);
    g_Supervisor.gfxDevice->SetTextureImage(converted->w, converted->h, PIXEL_RGBA,
                                            PIXEL_UNSIGNED_BYTE, converted->pixels);

    this->imageDataArray[textureIdx] = malloc(converted->pitch * converted->h);
    memcpy(this->imageDataArray[textureIdx], converted->pixels, converted->pitch * converted->h);

    textureWidths[textureIdx] = converted->w;
    textureHeights[textureIdx] = converted->h;
    texturePitches[textureIdx] = converted->pitch;

    SDL_DestroySurface(converted);
    return ZUN_SUCCESS;
}

ZunResult AnmManager::CreateEmptyTexture(i32 textureIdx, u32 width, u32 height)
{
    ReleaseTexture(textureIdx);
    this->textures[textureIdx] = g_Supervisor.gfxDevice->CreateTexture();

    void *emptyData = calloc(width * height, 4);
    g_Supervisor.gfxDevice->BindTexture(this->textures[textureIdx]);
    g_Supervisor.gfxDevice->SetTextureImage(width, height, PIXEL_RGBA, PIXEL_UNSIGNED_BYTE,
                                            emptyData);

    this->imageDataArray[textureIdx] = emptyData;

    textureWidths[textureIdx] = width;
    textureHeights[textureIdx] = height;
    texturePitches[textureIdx] = width * 4;

    return ZUN_SUCCESS;
}

i32 AnmManager::LoadAnms(i32 anmIdx, const char *path, i32 spriteIdxOffset)
{
    i32 res;
    u32 ownsMemory;
    AnmRawEntry *entry;

    // An animation with a Lua form is loaded from anm/<name>/; anything else comes from the
    // binary file below. The sprite table lives beside either form, named after the file.
    std::string anmName = path;
    const size_t slash = anmName.find_last_of('/');
    if (slash != std::string::npos)
    {
        anmName = anmName.substr(slash + 1);
    }
    if (anmName.size() > 4 && anmName.compare(anmName.size() - 4, 4, ".anm") == 0)
    {
        anmName.resize(anmName.size() - 4);
    }
    // The constructor memsets the object, so this buffer is plain bytes and copying into it
    // is the only safe way to fill it.
    strncpy(this->currentAnmName, anmName.c_str(), sizeof(this->currentAnmName) - 1);
    this->currentAnmName[sizeof(this->currentAnmName) - 1] = '\0';

    i32 startIdx = anmIdx;
    const int entryCount = AnmEntryFile::Count(this->currentAnmName);
    if (entryCount <= 0)
    {
        g_GameErrorContext.Fatal("アニメ %s の記述が見つかりません\n", this->currentAnmName);
        return ZUN_ERROR;
    }

    for (i32 index = 0; index < entryCount; index++)
    {
        AnmEntryFile::Entry description;
        std::string error;
        if (!AnmEntryFile::Load(this->currentAnmName, index, description, error))
        {
            g_GameErrorContext.Fatal("%s\n", error.c_str());
            this->anmFiles[startIdx].childCount = anmIdx - startIdx;
            return ZUN_ERROR;
        }

        res = LoadAnm(anmIdx, description, spriteIdxOffset, index);
        if (res < 0)
        {
            this->anmFiles[startIdx].childCount = anmIdx - startIdx;
            return res;
        }
        anmIdx++;
        spriteIdxOffset = spriteIdxOffset + res;
    }

    this->anmFiles[startIdx].childCount = anmIdx - startIdx;
    return ZUN_SUCCESS;
}

i32 AnmManager::LoadAnm(i32 textureIdx, const AnmEntryFile::Entry &description,
                        i32 spriteIdxOffset, i32 entryIndex)
{
    AnmLoadedSprite loadedSprite;
    i32 i;
    i32 id;

    id = 0;
    if (textureIdx >= ARRAY_SIZE_SIGNED(this->anmFiles))
    {
        g_GameErrorContext.Fatal("テクスチャ格納先が足りません\n");
        return ZUN_ERROR;
    }
    ReleaseAnm(textureIdx);

    // "@" marks an entry with no image of its own: the descriptor asks for an empty texture of
    // the given size rather than a file.
    if (description.texture == "@")
    {
        CreateEmptyTexture(textureIdx, description.width, description.height);
    }
    else
    {
        char texturePath[192];
        // Relative to the asset root, not to th07.data/: the textures are converted assets and
        // live under assets/, and the store decides which root a name comes from. Writing the
        // data directory here would name a directory the store does not know about.
        snprintf(texturePath, sizeof(texturePath), "anm/%s/%s", this->currentAnmName,
                 description.texture.c_str());
        if (LoadTexture(textureIdx, texturePath, description.format) != ZUN_SUCCESS)
        {
            g_GameErrorContext.Fatal(
                "テクスチャ %s が読み込めません。データが失われてるか壊れています\n",
                texturePath);
            return ZUN_ERROR;
        }
    }

    // Held so ReleaseTexture can name what it is releasing; the description lives only for
    // this call, so the pointer has to be to storage that outlives it.
    snprintf(this->textureNamesOwned[textureIdx], sizeof(this->textureNamesOwned[0]), "%s",
             description.texture.c_str());
    this->textureNames[textureIdx] = this->textureNamesOwned[textureIdx];

    const u32 texWidth =
        this->textureWidths[textureIdx] ? this->textureWidths[textureIdx] : description.width;
    const u32 texHeight =
        this->textureHeights[textureIdx] ? this->textureHeights[textureIdx] : description.height;

    for (i = 0; i < (i32)description.sprites.size(); i++)
    {
        const AnmEntryFile::Sprite &rawSprite = description.sprites[i];
        loadedSprite.sourceFileIndex = textureIdx;
        loadedSprite.cols = (f32)texWidth / (f32)description.width;
        loadedSprite.rows = (f32)texHeight / (f32)description.height;
        loadedSprite.startPixelInclusive.x = loadedSprite.cols * rawSprite.offsetX;
        loadedSprite.startPixelInclusive.y = loadedSprite.rows * rawSprite.offsetY;
        loadedSprite.endPixelInclusive.x =
            (rawSprite.offsetX + rawSprite.sizeX) * loadedSprite.cols;
        loadedSprite.endPixelInclusive.y =
            (rawSprite.offsetY + rawSprite.sizeY) * loadedSprite.rows;
        loadedSprite.textureWidth = (f32)texWidth;
        loadedSprite.textureHeight = (f32)texHeight;
        if (id < rawSprite.id)
        {
            id = rawSprite.id;
        }
        if (rawSprite.id + spriteIdxOffset >= ARRAY_SIZE_SIGNED(this->sprites))
        {
            g_GameErrorContext.Fatal("スプライトが格納できません。テーブルが不足しています\n");
            return ZUN_ERROR;
        }
        LoadSprite(rawSprite.id + spriteIdxOffset, &loadedSprite);
    }

    // The instructions are the Lua functions in each animation's animation.lua, reached by slot.
    // Nothing is read from a separate file, so all that is taken from the description here is
    // which slot each stream occupies.

    for (i32 scriptIdx = 0; scriptIdx < (i32)description.scripts.size(); scriptIdx++)
    {
        const AnmEntryFile::Script &script = description.scripts[scriptIdx];
        if (script.index + spriteIdxOffset >= ARRAY_SIZE_SIGNED(this->sprites))
        {
            g_GameErrorContext.Fatal("アニメが格納できません。テーブルが不足しています\n");
            return ZUN_ERROR;
        }
        if (id < script.index)
        {
            id = script.index;
        }
        this->scripts[script.index + spriteIdxOffset] = (AnmRawInstr *)1;
        this->spriteIndices[script.index + spriteIdxOffset] = spriteIdxOffset;

        // What this slot names. The Lua form is keyed by animation, entry and stream, none of
        // which can be recovered from the slot later: the slot is a number, and by the time a VM
        // starts a different animation may have been loaded.
        const i32 globalSlot = script.index + spriteIdxOffset;
        if (globalSlot >= 0 && globalSlot < ARRAY_SIZE_SIGNED(this->scriptOrigins))
        {
            strncpy(this->scriptOrigins[globalSlot].anmName, this->currentAnmName,
                    sizeof(this->scriptOrigins[0].anmName) - 1);
            this->scriptOrigins[globalSlot]
                .anmName[sizeof(this->scriptOrigins[0].anmName) - 1] = '\0';
            this->scriptOrigins[globalSlot].entryIndex = entryIndex;
            // The position in the list, not the index: the index is a slot number and can be
            // negative -- title01's later entries use negative indices to place their streams
            // just after the earlier entries -- while the Lua table keys its functions by
            // position, in the order they appear.
            this->scriptOrigins[globalSlot].streamIndex = scriptIdx;
        }
    }


    this->anmFiles[textureIdx].raw = NULL;
    this->anmFiles[textureIdx].spriteIndexOffset = spriteIdxOffset;
    // What this entry occupies, so releasing it later does not need the description, which
    // goes out of scope when this function returns.
    this->anmFiles[textureIdx].textureIdx = textureIdx;
    this->anmFiles[textureIdx].spriteCount = (i32)description.sprites.size();
    this->anmFiles[textureIdx].scriptCount = (i32)description.scripts.size();
    const i32 slotCapacity = ARRAY_SIZE_SIGNED(this->anmFiles[textureIdx].scriptSlots);
    for (i32 slot = 0; slot < (i32)description.scripts.size() && slot < slotCapacity; slot++)
    {
        this->anmFiles[textureIdx].scriptSlots[slot] = description.scripts[slot].index;
    }
    return id + 1;
}

void AnmManager::ReleaseAnm(i32 anmIdx)
{
    i32 uvX;
    i32 i;

    if (anmIdx < 0 || (u32)anmIdx >= ARRAY_SIZE_SIGNED(this->anmFiles))
    {
        return;
    }

    // Nothing to release unless something was loaded. The children go first: each entry in
    // the chain owns the slots its own index covers.
    if (this->anmFiles[anmIdx].raw != NULL || this->anmFiles[anmIdx].spriteCount > 0)
    {
        uvX = anmIdx + 1;
        for (i = 1; i < this->anmFiles[anmIdx].childCount; i++, uvX++)
        {
            ReleaseAnm(uvX);
        }

        const i32 spriteIdxOffset = this->anmFiles[anmIdx].spriteIndexOffset;
        for (i = 0; i < this->anmFiles[anmIdx].spriteCount; i++)
        {
            if (i + spriteIdxOffset < ARRAY_SIZE_SIGNED(this->sprites))
            {
                memset(&this->sprites[i + spriteIdxOffset], 0, sizeof(AnmLoadedSprite));
                this->sprites[i + spriteIdxOffset].sourceFileIndex = -1;
            }
        }

        for (i = 0; i < this->anmFiles[anmIdx].scriptCount; i++)
        {
            const i32 slot = this->anmFiles[anmIdx].scriptSlots[i] + spriteIdxOffset;
            if (slot >= 0 && slot < ARRAY_SIZE_SIGNED(this->scripts))
            {
                this->scripts[slot] = NULL;
                this->spriteIndices[slot] = 0;
            }
        }

        this->anmFiles[anmIdx].spriteIndexOffset = 0;
        if (this->anmFiles[anmIdx].textureIdx >= 0 &&
            this->anmFiles[anmIdx].textureIdx < MAX_TEXTURES)
        {
            this->textureNames[this->anmFiles[anmIdx].textureIdx] = NULL;
            ReleaseTexture(this->anmFiles[anmIdx].textureIdx);
        }

        this->anmFiles[anmIdx].raw = NULL;
        this->anmFiles[anmIdx].spriteCount = 0;
        this->anmFiles[anmIdx].scriptCount = 0;
        this->anmFiles[anmIdx].childCount = 0;
        this->currentBlendMode = 255;
        this->currentColorOp = 255;
        this->currentVertexShader = 0;
        this->currentTexture = 0;
    }
}

void AnmManager::ReleaseTexture(i32 textureIdx)
{
    if (textureIdx < 0 || textureIdx >= ARRAY_SIZE(this->textures))
    {
        return;
    }

    g_Supervisor.gfxDevice->DeleteTexture(this->textures[textureIdx]);
    this->textures[textureIdx].id = 0;
    this->textureWidths[textureIdx] = 0;
    this->textureHeights[textureIdx] = 0;
    this->texturePitches[textureIdx] = 0;

    free(this->imageDataArray[textureIdx]);
    this->imageDataArray[textureIdx] = NULL;
}

void AnmManager::LoadSprite(u32 spriteIdx, AnmLoadedSprite *sprite)
{
    this->sprites[spriteIdx] = *sprite;
    this->sprites[spriteIdx].spriteId = this->loadedSpriteCount++;

    this->sprites[spriteIdx].uvStart.x =
        this->sprites[spriteIdx].startPixelInclusive.x / (this->sprites[spriteIdx].textureWidth);
    this->sprites[spriteIdx].uvEnd.x =
        this->sprites[spriteIdx].endPixelInclusive.x / (this->sprites[spriteIdx].textureWidth);
    this->sprites[spriteIdx].uvStart.y =
        this->sprites[spriteIdx].startPixelInclusive.y / (this->sprites[spriteIdx].textureHeight);
    this->sprites[spriteIdx].uvEnd.y =
        this->sprites[spriteIdx].endPixelInclusive.y / (this->sprites[spriteIdx].textureHeight);
    this->sprites[spriteIdx].widthPx = (this->sprites[spriteIdx].endPixelInclusive.x -
                                        this->sprites[spriteIdx].startPixelInclusive.x) /
                                       sprite->cols;
    this->sprites[spriteIdx].heightPx = (this->sprites[spriteIdx].endPixelInclusive.y -
                                         this->sprites[spriteIdx].startPixelInclusive.y) /
                                        sprite->rows;
}

ZunResult AnmManager::SetActiveSprite(AnmVm *vm, i32 spriteIdx)
{
    if (this->sprites[spriteIdx].sourceFileIndex < 0)
    {
        return ZUN_ERROR;
    }

    vm->activeSpriteIdx = (i16)spriteIdx;
    vm->sprite = &this->sprites[spriteIdx];
    vm->baseTransformMatrix.Identity();
    vm->baseTransformMatrix.m[0][0] = vm->sprite->widthPx / 256.0f;
    vm->baseTransformMatrix.m[1][1] = vm->sprite->heightPx / 256.0f;
    vm->uvMatrix.Identity();
    vm->uvMatrix.m[0][0] = vm->sprite->widthPx / vm->sprite->textureWidth * vm->sprite->cols;
    vm->uvMatrix.m[1][1] = vm->sprite->heightPx / vm->sprite->textureHeight * vm->sprite->rows;
    vm->worldTransformMatrix = vm->matrix = vm->baseTransformMatrix;
    return ZUN_SUCCESS;
}

void AnmManager::SetAndExecuteScript(AnmVm *vm, bool hasScript)
{
    if (!hasScript)
    {
        // Nothing to run in this slot, so the VM is cleared and draws nothing -- which is what a
        // slot with no stream did before. Deliberately not memset: the slot number was just
        // written to this VM and the caller still needs it.
        vm->sprite = NULL;
        vm->activeSpriteIdx = -1;
        vm->visible = 0;
        vm->currentInstruction = NULL;
    }
    else
    {
        vm->flip = 0;
        vm->Initialize();
        vm->currentInstruction = (AnmRawInstr *)1;
        vm->beginningOfScript = vm->currentInstruction;
        vm->currentTimeInScript = 0;
        vm->visible = 0;

        // The Lua form needs the animation, the entry and the stream, none of which can be
        // recovered from the slot number alone; the loader recorded them when the animation was
        // loaded, and this is where a VM asks for the stream its slot names.
        if (vm->anmFileIdx >= 0 && vm->anmFileIdx < ARRAY_SIZE_SIGNED(this->scriptOrigins))
        {
            const ScriptOrigin &origin = this->scriptOrigins[vm->anmFileIdx];
            if (AnmScript::ShouldUseLua(vm->anmFileIdx))
            {
                if (!AnmScript::Start(vm, origin.anmName, origin.entryIndex, origin.streamIndex))
                {
                    AnmVmDebug::Note("cannot start %s entry %d stream %d", origin.anmName,
                                     (int)origin.entryIndex, (int)origin.streamIndex);
                }
            }
        }

        ExecuteScript(vm);
        vm->UpdatePrev();
        this->scriptsExecutedThisFrame++;
    }
}

void AnmManager::SetRenderStateForVm(AnmVm *vm)
{
    ZunColor color;
    ZunColor prevColor;

    if ((u32)this->currentBlendMode != vm->blendMode)
    {
        Flush();
        this->currentBlendMode = vm->blendMode;
        if (!this->currentBlendMode)
        {
            g_Supervisor.gfxDevice->SetBlendMode(BLEND_ALPHA, BLEND_ALPHA);
        }
        else
        {
            g_Supervisor.gfxDevice->SetBlendMode(BLEND_ALPHA, BLEND_ONE);
        }
    }
    color.color = vm->useColor2 ? vm->color2.color : vm->color.color;
    prevColor.color = vm->prevUseColor2 ? vm->prevColor2.color : vm->prevColor.color;

    color = ZunColor::Lerp(prevColor, color, g_RenderAlpha);

    if (!g_Supervisor.cfg.noVertexBuffers)
    {
        if (this->colorMulEnabled)
        {
            color.bytes.r = ZunColor::Multiply(color.bytes.r, this->color.bytes.r);
            color.bytes.g = ZunColor::Multiply(color.bytes.g, this->color.bytes.g);
            color.bytes.b = ZunColor::Multiply(color.bytes.b, this->color.bytes.b);
            color.bytes.a = ZunColor::Multiply(color.bytes.a, this->color.bytes.a);
        }
        if (this->currentTextureFactor.color != color.color)
        {
            this->currentTextureFactor.color = color.color;
            g_Supervisor.gfxDevice->SetTextureFactor(this->currentTextureFactor);
        }
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
    if (!g_Supervisor.cfg.disableZBuffer && (u32)this->currentZWriteDisable != vm->zWriteDisable)
    {
        Flush();
        this->currentZWriteDisable = vm->zWriteDisable;
        g_Supervisor.gfxDevice->SetDepthMask(this->currentZWriteDisable == 0);
    }
    if ((u32)this->currentCameraMode != vm->cameraMode)
    {
        Flush();
        this->currentCameraMode = vm->cameraMode;
        if (!this->currentCameraMode)
        {
            g_Stage.SetupCameraStageBackground();
            g_Supervisor.gfxDevice->SetViewport(g_Supervisor.viewport);
        }
        else
        {
            g_Stage.UpdateCamera();
            g_Supervisor.gfxDevice->SetViewport(g_Supervisor.viewport);
        }
    }
    this->renderStateChangesThisFrame++;
}

void AnmManager::SyncRenderState(AnmVm *vm)
{
    if ((u32)this->currentBlendMode != vm->blendMode)
    {
        Flush();
        this->currentBlendMode = vm->blendMode;
        if (!this->currentBlendMode)
        {
            g_Supervisor.gfxDevice->SetBlendMode(BLEND_ALPHA, BLEND_ALPHA);
        }
        else
        {
            g_Supervisor.gfxDevice->SetBlendMode(BLEND_ALPHA, BLEND_ONE);
        }
    }
    if (!g_Supervisor.cfg.disableZBuffer && (u32)this->currentZWriteDisable != vm->zWriteDisable)
    {
        Flush();
        this->currentZWriteDisable = vm->zWriteDisable;
        g_Supervisor.gfxDevice->SetDepthMask(this->currentZWriteDisable == 0);
    }
    this->renderStateChangesThisFrame++;
}

ZunResult AnmManager::DrawInner(AnmVm *vm, u32 drawFlags)
{
    ZunColor color, prevColor;
    f32 triangleX1, triangleX2, triangleY1, triangleY2;

    Float2 drawUv = vm->prevUvScrollPos.LerpUv(vm->uvScrollPos, g_RenderAlpha);
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
        g_QuadVertices[0].pos.x = floorf(g_QuadVertices[0].pos.x + 0.5f);
        g_QuadVertices[1].pos.x = floorf(g_QuadVertices[1].pos.x + 0.5f);
        g_QuadVertices[0].pos.y = floorf(g_QuadVertices[0].pos.y + 0.5f);
        g_QuadVertices[2].pos.y = floorf(g_QuadVertices[2].pos.y + 0.5f);
        g_QuadVertices[1].pos.y = g_QuadVertices[0].pos.y;
        g_QuadVertices[2].pos.x = g_QuadVertices[0].pos.x;
        g_QuadVertices[3].pos.x = g_QuadVertices[1].pos.x;
        g_QuadVertices[3].pos.y = g_QuadVertices[2].pos.y;
    }

    g_QuadVertices[0].textureUV.x = g_QuadVertices[2].textureUV.x =
        vm->sprite->uvStart.x + drawUv.x;
    g_QuadVertices[1].textureUV.x = g_QuadVertices[3].textureUV.x = vm->sprite->uvEnd.x + drawUv.x;
    g_QuadVertices[0].textureUV.y = g_QuadVertices[1].textureUV.y =
        vm->sprite->uvStart.y + drawUv.y;
    g_QuadVertices[2].textureUV.y = g_QuadVertices[3].textureUV.y = vm->sprite->uvEnd.y + drawUv.y;

    triangleX1 = std::max(g_QuadVertices[0].pos.x, g_QuadVertices[1].pos.x);
    triangleX1 = std::max(g_QuadVertices[2].pos.x, triangleX1);
    triangleX1 = std::max(g_QuadVertices[3].pos.x, triangleX1);

    triangleY1 = std::max(g_QuadVertices[0].pos.y, g_QuadVertices[1].pos.y);
    triangleY1 = std::max(g_QuadVertices[2].pos.y, triangleY1);
    triangleY1 = std::max(g_QuadVertices[3].pos.y, triangleY1);

    triangleX2 = std::min(g_QuadVertices[0].pos.x, g_QuadVertices[1].pos.x);
    triangleX2 = std::min(g_QuadVertices[2].pos.x, triangleX2);
    triangleX2 = std::min(g_QuadVertices[3].pos.x, triangleX2);

    triangleY2 = std::min(g_QuadVertices[0].pos.y, g_QuadVertices[1].pos.y);
    triangleY2 = std::min(g_QuadVertices[2].pos.y, triangleY2);
    triangleY2 = std::min(g_QuadVertices[3].pos.y, triangleY2);

    if (triangleX1 < g_Supervisor.viewport.x || triangleY1 < g_Supervisor.viewport.y ||
        triangleX2 > g_Supervisor.viewport.x + g_Supervisor.viewport.width ||
        triangleY2 > g_Supervisor.viewport.y + g_Supervisor.viewport.height)
    {
        return ZUN_SUCCESS;
    }

    if (this->currentTexture != this->textures[vm->sprite->sourceFileIndex])
    {
        this->currentTexture = this->textures[vm->sprite->sourceFileIndex];
        this->Flush();
        g_Supervisor.gfxDevice->BindTexture(this->currentTexture);
    }
    if (this->currentVertexShader != 1)
    {
        this->Flush();
        this->currentVertexShader = 1;
    }
    if ((drawFlags & 2) == 0)
    {
        color.color = vm->useColor2 ? vm->color2.color : vm->color.color;
        prevColor.color = vm->prevUseColor2 ? vm->prevColor2.color : vm->prevColor.color;

        color = ZunColor::Lerp(prevColor, color, g_RenderAlpha);
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

void AnmManager::ResetVertexBuffer()
{
    this->spritesToDraw = 0;
    this->vertexBufferCurPtr = this->spriteVertexBuffer;
    this->vertexBufferStartPtr = this->vertexBufferCurPtr;
}

void AnmManager::Flush()
{
    if (!this->spritesToDraw)
    {
        return;
    }

    g_Supervisor.gfxDevice->SetTextureArg(TEX_ARG_DIFFUSE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_ALPHA, COLOR_OP_MODULATE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_RGB, COLOR_OP_MODULATE);

    g_Supervisor.gfxDevice->DrawPrimitiveUP(PRIM_TRIANGLES, this->spritesToDraw << 1,
                                            this->vertexBufferStartPtr,
                                            sizeof(VertexTex1DiffuseXyzrhw));
    this->vertexBufferStartPtr = this->vertexBufferCurPtr;
    this->spritesToDraw = 0;
    this->flushesThisFrame++;
}

ZunResult AnmManager::PushSprite(VertexTex1DiffuseXyzrhw *spriteVertex)
{
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

    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    centerX = vm->sprite->widthPx * drawScale.x / 2.0f;
    centerY = vm->sprite->heightPx * drawScale.y / 2.0f;

    if ((vm->anchor & 1) == 0)
    {
        g_QuadVertices[0].pos.x = g_QuadVertices[2].pos.x = vm->pos.x - centerX;
        g_QuadVertices[1].pos.x = g_QuadVertices[3].pos.x = centerX + vm->pos.x;
    }
    else
    {
        g_QuadVertices[0].pos.x = g_QuadVertices[2].pos.x = vm->pos.x;
        g_QuadVertices[1].pos.x = g_QuadVertices[3].pos.x = centerX + vm->pos.x + centerX;
    }

    if ((vm->anchor & 2) == 0)
    {
        g_QuadVertices[0].pos.y = g_QuadVertices[1].pos.y = vm->pos.y - centerY;
        g_QuadVertices[2].pos.y = g_QuadVertices[3].pos.y = centerY + vm->pos.y;
    }
    else
    {
        g_QuadVertices[0].pos.y = g_QuadVertices[1].pos.y = vm->pos.y;
        g_QuadVertices[2].pos.y = g_QuadVertices[3].pos.y = centerY + vm->pos.y + centerY;
    }

    g_QuadVertices[0].pos.z = g_QuadVertices[1].pos.z = g_QuadVertices[2].pos.z =
        g_QuadVertices[3].pos.z = vm->pos.z;

    return DrawInner(vm, 1);
}

void AnmManager::TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, f32 width, f32 height, f32 sine,
                                   f32 cosine, f32 xOffset, f32 yOffset)
{
    vertex->pos.x = width * cosine - height * sine + xOffset;
    vertex->pos.y = width * sine + height * cosine + yOffset;
}

ZunResult AnmManager::Draw(AnmVm *vm)
{
    f32 cosZ;
    f32 sinZ;
    f32 xOffset;
    f32 yOffset;
    f32 z;
    f32 width;
    f32 height;

    if (vm->rotation.z == 0.0f && vm->prevRotation.z == 0.0f)
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

    f32 drawRotZ = utils::LerpAngle(vm->prevRotation.z, vm->rotation.z, g_RenderAlpha);
    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    z = drawRotZ;
    sincosf(&sinZ, &cosZ, z);
    xOffset = vm->pos.x;
    yOffset = vm->pos.y;
    width = vm->sprite->widthPx * drawScale.x / 2.0f;
    height = vm->sprite->heightPx * drawScale.y / 2.0f;

    TranslateRotation(&g_QuadVertices[0], -width, -height, sinZ, cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[1], width, -height, sinZ, cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[2], -width, height, sinZ, cosZ, xOffset, yOffset);
    TranslateRotation(&g_QuadVertices[3], width, height, sinZ, cosZ, xOffset, yOffset);

    g_QuadVertices[0].pos.z = g_QuadVertices[1].pos.z = g_QuadVertices[2].pos.z =
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

    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    centerX = vm->sprite->widthPx * drawScale.x / 2.0f;
    centerY = vm->sprite->heightPx * drawScale.y / 2.0f;

    if ((vm->anchor & 1) == 0)
    {
        g_QuadVertices[0].pos.x = g_QuadVertices[2].pos.x = vm->pos.x - centerX;
        g_QuadVertices[1].pos.x = g_QuadVertices[3].pos.x = centerX + vm->pos.x;
    }
    else
    {
        g_QuadVertices[0].pos.x = g_QuadVertices[2].pos.x = vm->pos.x;
        g_QuadVertices[1].pos.x = g_QuadVertices[3].pos.x = centerX + vm->pos.x + centerX;
    }

    if ((vm->anchor & 2) == 0)
    {
        g_QuadVertices[0].pos.y = g_QuadVertices[1].pos.y = vm->pos.y - centerY;
        g_QuadVertices[2].pos.y = g_QuadVertices[3].pos.y = centerY + vm->pos.y;
    }
    else
    {
        g_QuadVertices[0].pos.y = g_QuadVertices[1].pos.y = vm->pos.y;
        g_QuadVertices[2].pos.y = g_QuadVertices[3].pos.y = centerY + vm->pos.y + centerY;
    }

    g_QuadVertices[0].pos.z = g_QuadVertices[1].pos.z = g_QuadVertices[2].pos.z =
        g_QuadVertices[3].pos.z = vm->pos.z;

    return DrawInner(vm, 0);
}

ZunResult AnmManager::CalcBillboardTransform(AnmVm *vm)
{
    f32 halfWidth;
    f32 halfHeight;
    f32 screenCenterY;
    f32 halfLength; // also used as screen center x
    f32 sinZ;
    ZunMatrix matrix;
    f32 z = vm->rotation.z;
    ZunVec3 projectRight;
    ZunVec3 projectCenter;
    ZunVec3 projectRightOffset;
    f32 cosZ;

    f32 drawRotZ = utils::LerpAngle(vm->prevRotation.z, vm->rotation.z, g_RenderAlpha);
    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    sincosf(&sinZ, &cosZ, drawRotZ);

    ZunVec3 origin(0.0f, 0.0f, 0.0f);

    matrix.Identity();
    matrix.m[3][0] = vm->pos.x;
    matrix.m[3][1] = vm->pos.y;
    matrix.m[3][2] = vm->pos.z;

    ZunMatrix wvp = matrix * g_Supervisor.viewProjectionMatrix;
    projectCenter.Project(&origin, &g_Supervisor.viewport, &wvp);

    if (projectCenter.z < 0.0f || projectCenter.z > 1.0f)
    {
        return ZUN_ERROR;
    }

    projectRight.Project(&g_Stage.cam.right, &g_Supervisor.viewport, &wvp);

    projectRightOffset = projectRight - projectCenter;

    halfLength = projectRightOffset.Length() * 0.5f;
    halfWidth = halfLength * vm->sprite->widthPx * drawScale.x;
    halfHeight = halfLength * vm->sprite->heightPx * drawScale.y;

    halfLength = projectCenter.x; // used as screen center x here
    screenCenterY = projectCenter.y;

    TranslateRotation(&g_QuadVertices[0], -halfWidth, -halfHeight, sinZ, cosZ, halfLength,
                      screenCenterY);
    TranslateRotation(&g_QuadVertices[1], halfWidth, -halfHeight, sinZ, cosZ, halfLength,
                      screenCenterY);
    TranslateRotation(&g_QuadVertices[2], -halfWidth, halfHeight, sinZ, cosZ, halfLength,
                      screenCenterY);
    TranslateRotation(&g_QuadVertices[3], halfWidth, halfHeight, sinZ, cosZ, halfLength,
                      screenCenterY);

    g_QuadVertices[0].pos.z = g_QuadVertices[1].pos.z = g_QuadVertices[2].pos.z =
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

static ZunMatrix BuildInterpolatedTransform(const AnmVm *vm)
{
    const Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);
    const ZunVec3 drawRotation = {
        utils::LerpAngle(vm->prevRotation.x, vm->rotation.x, g_RenderAlpha),
        utils::LerpAngle(vm->prevRotation.y, vm->rotation.y, g_RenderAlpha),
        utils::LerpAngle(vm->prevRotation.z, vm->rotation.z, g_RenderAlpha),
    };

    ZunMatrix world = vm->baseTransformMatrix;

    world.m[0][0] *= drawScale.x;
    world.m[1][1] *= drawScale.y;

    ZunMatrix rot;

    if (drawRotation.x != 0.0f)
    {
        rot.RotateX(drawRotation.x);
        world *= rot;
    }

    if (drawRotation.y != 0.0f)
    {
        rot.RotateY(drawRotation.y);
        world *= rot;
    }

    if (drawRotation.z != 0.0f)
    {
        rot.RotateZ(drawRotation.z);
        world *= rot;
    }

    return world;
}

void AnmManager::CalcProjectedTransform(AnmVm *vm)
{
    ZunMatrix world;
    ZunMatrix rot;

    if (vm->skipTransform == 0 && (vm->updateScale || vm->updateRotation))
    {
        vm->worldTransformMatrix = vm->baseTransformMatrix;
        vm->worldTransformMatrix.m[0][0] *= vm->scale.x;
        vm->worldTransformMatrix.m[1][1] *= vm->scale.y;
        vm->updateScale = 0;
        if (vm->rotation.x != 0.0)
        {
            rot.RotateX(vm->rotation.x);
            vm->worldTransformMatrix *= rot;
        }
        if (vm->rotation.y != 0.0)
        {
            rot.RotateY(vm->rotation.y);
            vm->worldTransformMatrix *= rot;
        }
        if (vm->rotation.z != 0.0)
        {
            rot.RotateZ(vm->rotation.z);
            vm->worldTransformMatrix *= rot;
        }
        vm->updateRotation = 0;
    }

    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    world = vm->skipTransform == 0 ? BuildInterpolatedTransform(vm) : vm->worldTransformMatrix;

    if ((vm->anchor & 1) == 0)
    {
        world.m[3][0] = vm->pos.x;
    }
    else
    {
        world.m[3][0] = fabsf(vm->sprite->widthPx * drawScale.x / 2.0f) + vm->pos.x;
    }

    if ((vm->anchor & 2) == 0)
    {
        world.m[3][1] = vm->pos.y;
    }
    else
    {
        world.m[3][1] = fabsf(vm->sprite->heightPx * drawScale.y / 2.0f) + vm->pos.y;
    }
    world.m[3][2] = vm->pos.z;

    ZunMatrix wvp = world * g_Supervisor.viewProjectionMatrix;

    g_QuadVertices[0].pos.Project(&this->vertexBufferContents[0].pos, &g_Supervisor.viewport, &wvp);
    g_QuadVertices[1].pos.Project(&this->vertexBufferContents[1].pos, &g_Supervisor.viewport, &wvp);
    g_QuadVertices[2].pos.Project(&this->vertexBufferContents[2].pos, &g_Supervisor.viewport, &wvp);
    g_QuadVertices[3].pos.Project(&this->vertexBufferContents[3].pos, &g_Supervisor.viewport, &wvp);
}

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

ZunResult AnmManager::Draw3(AnmVm *vm)
{
    ZunMatrix world;
    ZunMatrix rot;
    ZunMatrix uv;

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

    if (vm->skipTransform == 0 && (vm->updateScale || vm->updateRotation))
    {
        vm->worldTransformMatrix = vm->baseTransformMatrix;
        vm->worldTransformMatrix.m[0][0] *= vm->scale.x;
        vm->worldTransformMatrix.m[1][1] *= vm->scale.y;
        vm->updateScale = 0;

        // double intentionally used here
        if (vm->rotation.x != 0.0)
        {
            rot.RotateX(vm->rotation.x);
            vm->worldTransformMatrix *= rot;
        }
        if (vm->rotation.y != 0.0)
        {
            rot.RotateY(vm->rotation.y);
            vm->worldTransformMatrix *= rot;
        }
        if (vm->rotation.z != 0.0)
        {
            rot.RotateZ(vm->rotation.z);
            vm->worldTransformMatrix *= rot;
        }
        vm->updateRotation = 0;
    }

    Float2 drawScale = vm->prevScale.Lerp(vm->scale, g_RenderAlpha);

    world = vm->skipTransform == 0 ? BuildInterpolatedTransform(vm) : vm->worldTransformMatrix;

    if ((vm->anchor & 1) == 0)
    {
        world.m[3][0] = vm->pos.x;
    }
    else
    {
        world.m[3][0] = fabsf(vm->sprite->widthPx * drawScale.x / 2.0f) + vm->pos.x;
    }

    if ((vm->anchor & 2) == 0)
    {
        world.m[3][1] = vm->pos.y;
    }
    else
    {
        world.m[3][1] = fabsf(vm->sprite->heightPx * drawScale.y / 2.0f) + vm->pos.y;
    }

    world.m[3][0] += this->offset.x;
    world.m[3][1] += this->offset.y;

    SetRenderStateForVm(vm);
    world.m[3][2] = vm->pos.z;

    g_Supervisor.gfxDevice->SetTransformMatrix(MATRIX_MODEL, world);

    if (this->currentSprite != vm->sprite)
    {
        this->currentSprite = vm->sprite;
        uv = vm->uvMatrix;

        Float2 drawUv = vm->prevUvScrollPos.LerpUv(vm->uvScrollPos, g_RenderAlpha);

        uv.m[2][0] = vm->sprite->uvStart.x + drawUv.x;
        uv.m[2][1] = vm->sprite->uvStart.y + drawUv.y;

        g_Supervisor.gfxDevice->SetTransformMatrix(MATRIX_TEXTURE, uv);

        if (this->currentTexture != this->textures[vm->sprite->sourceFileIndex])
        {
            this->currentTexture = this->textures[vm->sprite->sourceFileIndex];
            g_Supervisor.gfxDevice->BindTexture(this->currentTexture);
        }
    }

    if (this->currentVertexShader != 2)
    {
        g_Supervisor.gfxDevice->SetColorOp(COMPONENT_ALPHA, COLOR_OP_MODULATE);
        g_Supervisor.gfxDevice->SetColorOp(COMPONENT_RGB, COLOR_OP_MODULATE);
        g_Supervisor.gfxDevice->SetTextureArg(TEX_ARG_TFACTOR);
        this->currentVertexShader = 2;
    }

    if (!g_Supervisor.cfg.noVertexBuffers)
    {
        g_Supervisor.gfxDevice->DrawPrimitive(PRIM_TRIANGLE_STRIP, 0, 2);
    }
    else
    {
        g_Supervisor.gfxDevice->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, g_Quad3DFallback,
                                                sizeof(VertexTex1DiffuseXyz));
    }
    return ZUN_SUCCESS;
}

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

void AnmManager::JumpConditional(AnmVm *vm, const AnmRawInstr *instr)
{
    // The twelve conditional jumps carry (value, value, target, time) and land here when the
    // comparison succeeds. The body was the `jump:` label inside ExecuteScript.
    vm->currentTimeInScript = instr->args[3].i;
    vm->currentInstruction =
        (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[2].i);
}

void AnmManager::StartPositionInterp(AnmVm *vm, const AnmRawInstr *instr, i32 easeMode)
{
    // The three POS_TIME_* opcodes set their easing and then run this. The body was the
    // `interp_pos:` label inside ExecuteScript.
    vm->easeModes[0] = (i8)easeMode;

    if (!vm->useOffset)
    {
        vm->posInterpInitial = vm->pos;
    }
    else
    {
        vm->posInterpInitial = vm->offset;
    }

    vm->posInterpFinal = ZunVec3(
        ((instr->flags & (1 << 0)) != 0) ? vm->GetFloatVarValue(instr->args[0].f)
                                         : instr->args[0].f,
        ((instr->flags & (1 << 1)) != 0) ? vm->GetFloatVarValue(instr->args[1].f)
                                         : instr->args[1].f,
        ((instr->flags & (1 << 2)) != 0) ? vm->GetFloatVarValue(instr->args[2].f)
                                         : instr->args[2].f);
    vm->interpEndTimes[0] =
        ((instr->flags & (1 << 3)) != 0) ? vm->GetVarValue(instr->args[3].i)
                                         : instr->args[3].i;
    vm->interpStartTimes[0] = 0;
}




i32 AnmManager::ExecuteScript(AnmVm *vm)
{
    AnmRawInstr *instr;
    AnmRawInstr *nextInstr;
    i32 i;
    f32 t;

#define GET_INT_PTR(argIdx) vm->GetVar(&instr->args[argIdx].i, instr->flags, argIdx)

#define GET_FLOAT_PTR(argIdx) vm->GetFloatVar(&instr->args[argIdx].f, instr->flags, argIdx)

#define GET_INT_VALUE(argIdx)                                                                      \
    (((instr->flags & (1 << argIdx)) != 0) ? vm->GetVarValue(instr->args[argIdx].i)                \
                                           : instr->args[argIdx].i)

#define GET_FLOAT_VALUE(argIdx)                                                                    \
    (((instr->flags & (1 << argIdx)) != 0) ? vm->GetFloatVarValue(instr->args[argIdx].f)           \
                                           : instr->args[argIdx].f)

    if (!vm->currentInstruction)
    {
        return 1;
    }

    if (g_SuppressAnmAdvance)
    {
        return 0;
    }

    // A VM whose slot runs through Lua takes that path for the whole frame: the coroutine holds
    // the position, at(t) holds the time comparison, and AnmOp::Run is reached from the runtime.
    // Everything after the instruction stream -- the interpolation update at `stop:` -- still
    // runs, because it is the same for both forms.
    if (AnmScript::HasCoroutine(vm))
    {
        // Recorded before and after: a VM that enters here and never reaches the dump below is
        // stuck in the runtime rather than in the per-frame update.
        AnmVmDebug::Note("slot %d takes the Lua path at t=%u", (int)vm->anmFileIdx,
                         (unsigned)vm->currentTimeInScript.GetCurrent());
        const i32 ended = AnmScript::Run(vm, (i32)vm->currentTimeInScript.GetCurrent());
        AnmVmDebug::Note("slot %d Lua returned %d at t=%u", (int)vm->anmFileIdx, (int)ended,
                         (unsigned)vm->currentTimeInScript.GetCurrent());
        if (ended)
        {
            vm->currentInstruction = NULL;
            return 1;
        }
        goto stop;
    }

    // The frame ends here. Everything that was above this point read the instruction stream;
    // the stream is a coroutine now, and Run has already been as far through it as the clock
    // allows. What remains is the update every VM needs whatever ran it.
stop:
    if (vm->angleVel.x != 0.0f)
    {
        vm->rotation.x = utils::AddNormalizeAngle(
            vm->rotation.x, g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.x);
        vm->updateRotation = 1;
    }
    if (vm->angleVel.y != 0.0f)
    {
        vm->rotation.y = utils::AddNormalizeAngle(
            vm->rotation.y, g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.y);
        vm->updateRotation = 1;
    }
    if (vm->angleVel.z != 0.0f)
    {
        vm->rotation.z = utils::AddNormalizeAngle(
            vm->rotation.z, g_Supervisor.effectiveFramerateMultiplier * vm->angleVel.z);
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
                t = vm->interpStartTimes[i].AsFloat() / vm->interpEndTimes[i].AsFloat();
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
                    vm->pos.x = (vm->posInterpFinal.x - vm->posInterpInitial.x) * t +
                                vm->posInterpInitial.x;
                    vm->pos.y = (vm->posInterpFinal.y - vm->posInterpInitial.y) * t +
                                vm->posInterpInitial.y;
                    vm->pos.z = (vm->posInterpFinal.z - vm->posInterpInitial.z) * t +
                                vm->posInterpInitial.z;
                }
                else
                {
                    vm->offset.x = (vm->posInterpFinal.x - vm->posInterpInitial.x) * t +
                                   vm->posInterpInitial.x;
                    vm->offset.y = (vm->posInterpFinal.y - vm->posInterpInitial.y) * t +
                                   vm->posInterpInitial.y;
                    vm->offset.z = (vm->posInterpFinal.z - vm->posInterpInitial.z) * t +
                                   vm->posInterpInitial.z;
                }
                break;
            case 1:
                vm->color.bytes.r = (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.r -
                                               (i32)vm->colorInterpInitialColor.bytes.r) *
                                             t +
                                         (f32)vm->colorInterpInitialColor.bytes.r);
                vm->color.bytes.g = (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.g -
                                               (i32)vm->colorInterpInitialColor.bytes.g) *
                                             t +
                                         (f32)vm->colorInterpInitialColor.bytes.g);
                vm->color.bytes.b = (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.b -
                                               (i32)vm->colorInterpInitialColor.bytes.b) *
                                             t +
                                         (f32)vm->colorInterpInitialColor.bytes.b);
                break;
            case 2:
                vm->color.bytes.a = (u8)((f32)((i32)vm->colorInterpFinalColor.bytes.a -
                                               (i32)vm->colorInterpInitialColor.bytes.a) *
                                             t +
                                         (f32)vm->colorInterpInitialColor.bytes.a);
                break;
            case 3:
                vm->rotation.x = utils::AddNormalizeAngle(
                    (vm->rotateInterpFinal.x - vm->rotateInterpInitial.x) * t,
                    vm->rotateInterpInitial.x);
                vm->rotation.y = utils::AddNormalizeAngle(
                    (vm->rotateInterpFinal.y - vm->rotateInterpInitial.y) * t,
                    vm->rotateInterpInitial.y);
                vm->rotation.z = utils::AddNormalizeAngle(
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
        vm->scale.y += g_Supervisor.effectiveFramerateMultiplier * vm->scaleGrowth.y;
        vm->updateScale = 1;
    }
    if (vm->scaleGrowth.x != 0.0f)
    {
        vm->scale.x += g_Supervisor.effectiveFramerateMultiplier * vm->scaleGrowth.x;
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
    AnmVmDebug::Dump(vm);
    return 0;
}

void AnmManager::DrawTextToSprite(u32 spriteDstIdx, i32 x, i32 y, i32 width, i32 height,
                                  i32 fontWidth, i32 fontHeight, u32 textColor, u32 outlineType,
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
    TextHelper::RenderTextToTextureBold(x, y, width, height, (f32)fontWidth * scaleY,
                                        (f32)fontHeight * scaleX, textColor, outlineType,
                                        strToPrint, this->textures[spriteDstIdx]);
}

void AnmManager::DrawVmTextFmt(AnmManager *manager, AnmVm *vm, u32 textColor, u32 outlineType,
                               const char *str, ...)
{
    u32 fontWidth;
    char text[256];
    va_list args;

    fontWidth = vm->fontWidth;

    va_start(args, str);
    vsnprintf(text, sizeof(text), str, args);
    va_end(args);

    manager->DrawTextToSprite(vm->sprite->sourceFileIndex, vm->sprite->startPixelInclusive.x,
                              vm->sprite->startPixelInclusive.y, vm->sprite->textureWidth,
                              vm->sprite->textureHeight, fontWidth, vm->fontHeight, textColor,
                              outlineType, text, vm->sprite->cols, vm->sprite->rows);

    vm->visible = 1;
}

void AnmManager::DrawStringFormat(AnmVm *vm, u32 textColor, u32 outlineType, const char *text, ...)
{
    i32 fontWidth;
    char buf[256];
    i32 x;
    va_list args;

    fontWidth = vm->fontWidth <= 0 ? 15 : (u32)vm->fontWidth;
    va_start(args, text);
    vsnprintf(buf, sizeof(buf), text, args);
    va_end(args);

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, vm->sprite->startPixelInclusive.x,
                           vm->sprite->startPixelInclusive.y, vm->sprite->textureWidth,
                           vm->sprite->textureHeight, fontWidth, vm->fontHeight, textColor,
                           outlineType, (char *)" ", vm->sprite->cols, vm->sprite->rows);

    x = vm->sprite->startPixelInclusive.x + vm->sprite->widthPx * vm->sprite->cols -
        (f32)TextHelper::GetLogicalStringWidth(buf) * (f32)fontWidth * vm->sprite->cols / 2.0f;

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, x, vm->sprite->startPixelInclusive.y,
                           vm->sprite->textureWidth, vm->sprite->textureHeight, fontWidth,
                           vm->fontHeight, textColor, outlineType, buf, vm->sprite->cols,
                           vm->sprite->rows);

    vm->visible = 1;
}

void AnmManager::DrawStringFormat2(AnmVm *vm, u32 textColor, u32 outlineType, const char *text, ...)
{
    i32 fontWidth;
    char buf[256];
    i32 x;
    va_list args;

    fontWidth = vm->fontWidth <= 0 ? 15 : (i32)vm->fontWidth;
    va_start(args, text);
    vsnprintf(buf, sizeof(buf), text, args);
    va_end(args);

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, vm->sprite->startPixelInclusive.x,
                           vm->sprite->startPixelInclusive.y, vm->sprite->textureWidth,
                           vm->sprite->textureHeight, fontWidth, vm->fontHeight, textColor,
                           outlineType, (char *)" ", vm->sprite->cols, vm->sprite->rows);

    x = (i32)(vm->sprite->startPixelInclusive.x + vm->sprite->widthPx * vm->sprite->cols / 2.0f -
              (f32)TextHelper::GetLogicalStringWidth(buf) * fontWidth * vm->sprite->cols / 4.0f);

    this->DrawTextToSprite(vm->sprite->sourceFileIndex, x, vm->sprite->startPixelInclusive.y,
                           vm->sprite->textureWidth, vm->sprite->textureHeight, fontWidth,
                           vm->fontHeight, textColor, outlineType, buf, vm->sprite->cols,
                           vm->sprite->rows);

    vm->visible = 1;
}

ZunResult AnmManager::LoadSurface(i32 surfaceIdx, const char *path)
{
    if (this->surfaces[surfaceIdx])
    {
        ReleaseSurface(surfaceIdx);
    }
    u8 *data = FileSystem::OpenFile(path, 0);
    if (!data)
    {
        g_GameErrorContext.Fatal("%sが読み込めないです。\n", path);
        return ZUN_ERROR;
    }

    SDL_IOStream *rw = SDL_IOFromMem(data, g_LastFileSize);
    SDL_Surface *surf = IMG_Load_IO(rw, 1);
    free(data);

    if (!surf)
    {
        return ZUN_ERROR;
    }

    this->surfaces[surfaceIdx] = SDL_ConvertSurface(surf, SDL_PIXELFORMAT_RGBA32);
    SDL_DestroySurface(surf);

    this->surfaceSourceInfo[surfaceIdx].width = this->surfaces[surfaceIdx]->w;
    this->surfaceSourceInfo[surfaceIdx].height = this->surfaces[surfaceIdx]->h;

    this->surfacesBis[surfaceIdx] =
        SDL_ConvertSurface(this->surfaces[surfaceIdx], SDL_PIXELFORMAT_RGBA32);
    if (this->surfaceTextures[surfaceIdx].id != 0)
    {
        g_Supervisor.gfxDevice->DeleteTexture(this->surfaceTextures[surfaceIdx]);
    }
    this->surfaceTextures[surfaceIdx] = g_Supervisor.gfxDevice->CreateTexture();
    g_Supervisor.gfxDevice->BindTexture(this->surfaceTextures[surfaceIdx]);
    g_Supervisor.gfxDevice->SetTextureImage(
        this->surfacesBis[surfaceIdx]->w, this->surfacesBis[surfaceIdx]->h, PIXEL_RGBA,
        PIXEL_UNSIGNED_BYTE, this->surfacesBis[surfaceIdx]->pixels);
    return ZUN_SUCCESS;
}

void AnmManager::ReleaseSurface(i32 surfaceIdx)
{
    if (this->surfaces[surfaceIdx])
    {
        SDL_DestroySurface(this->surfaces[surfaceIdx]);
        this->surfaces[surfaceIdx] = nullptr;
    }
    if (this->surfacesBis[surfaceIdx])
    {
        SDL_DestroySurface(this->surfacesBis[surfaceIdx]);
        this->surfacesBis[surfaceIdx] = nullptr;
    }
    if (this->surfaceTextures[surfaceIdx].id != 0)
    {
        g_Supervisor.gfxDevice->DeleteTexture(this->surfaceTextures[surfaceIdx]);
        this->surfaceTextures[surfaceIdx] = 0;
    }
}

void AnmManager::CopySurfaceToBackBuffer(i32 surfaceIdx, i32 left, i32 top, i32 x, i32 y)
{
    if (!this->surfacesBis[surfaceIdx])
    {
        return;
    }

    SDL_Surface *surf = this->surfacesBis[surfaceIdx];

    g_Supervisor.gfxDevice->BindTexture(this->surfaceTextures[surfaceIdx]);

    VertexTex1DiffuseXyzrhw vertices[4];
    f32 width = (f32)this->surfaceSourceInfo[surfaceIdx].width;
    f32 height = (f32)this->surfaceSourceInfo[surfaceIdx].height;

    vertices[0].pos = ZunVec3(x, y, 0.0f);
    vertices[1].pos = ZunVec3(x + width, y, 0.0f);
    vertices[2].pos = ZunVec3(x, y + height, 0.0f);
    vertices[3].pos = ZunVec3(x + width, y + height, 0.0f);
    vertices[0].w = vertices[1].w = vertices[2].w = vertices[3].w = 1.0f;

    f32 u0 = (f32)left / surf->w;
    f32 v0 = (f32)top / surf->h;
    f32 u1 = (f32)this->surfaceSourceInfo[surfaceIdx].width / surf->w;
    f32 v1 = (f32)this->surfaceSourceInfo[surfaceIdx].height / surf->h;

    vertices[0].textureUV = {u0, v0};
    vertices[1].textureUV = {u1, v0};
    vertices[2].textureUV = {u0, v1};
    vertices[3].textureUV = {u1, v1};

    vertices[0].diffuse.color = vertices[1].diffuse.color = vertices[2].diffuse.color =
        vertices[3].diffuse.color = 0xFFFFFFFF;

    g_Supervisor.gfxDevice->SetDepthMask(false);
    g_Supervisor.gfxDevice->SetBlendMode(BLEND_NONE, BLEND_NONE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_RGB, COLOR_OP_MODULATE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_ALPHA, COLOR_OP_MODULATE);
    g_Supervisor.gfxDevice->Disable(CAPS_ALPHA_TEST);
    g_Supervisor.gfxDevice->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, vertices,
                                            sizeof(VertexTex1DiffuseXyzrhw));
    g_Supervisor.gfxDevice->Enable(CAPS_ALPHA_TEST);
    this->SetBlendMode(255);
}

void AnmManager::DrawEndingRect(i32 surfaceIdx, f32 rectX, f32 rectY, f32 rectLeft, f32 rectTop,
                                i32 width, i32 height)
{
    if (!this->surfacesBis[surfaceIdx])
    {
        return;
    }

    SDL_Surface *surf =
        this->surfaces[surfaceIdx] ? this->surfaces[surfaceIdx] : this->surfacesBis[surfaceIdx];

    g_Supervisor.gfxDevice->BindTexture(this->surfaceTextures[surfaceIdx]);

    VertexTex1DiffuseXyzrhw vertices[4];
    f32 drawWidth = width;
    f32 drawHeight = height;

    vertices[0].pos = ZunVec3(rectX, rectY, 0.0f);
    vertices[1].pos = ZunVec3(rectX + drawWidth, rectY, 0.0f);
    vertices[2].pos = ZunVec3(rectX, rectY + drawHeight, 0.0f);
    vertices[3].pos = ZunVec3(rectX + drawWidth, rectY + drawHeight, 0.0f);
    vertices[0].w = vertices[1].w = vertices[2].w = vertices[3].w = 1.0f;

    f32 u0 = rectLeft / surf->w;
    f32 v0 = rectTop / surf->h;
    f32 u1 = (rectLeft + width) / surf->w;
    f32 v1 = (rectTop + height) / surf->h;

    vertices[0].textureUV = {u0, v0};
    vertices[1].textureUV = {u1, v0};
    vertices[2].textureUV = {u0, v1};
    vertices[3].textureUV = {u1, v1};

    vertices[0].diffuse.color = vertices[1].diffuse.color = vertices[2].diffuse.color =
        vertices[3].diffuse.color = 0xFFFFFFFF;

    g_Supervisor.gfxDevice->SetDepthMask(false);
    g_Supervisor.gfxDevice->SetBlendMode(BLEND_NONE, BLEND_NONE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_RGB, COLOR_OP_MODULATE);
    g_Supervisor.gfxDevice->SetColorOp(COMPONENT_ALPHA, COLOR_OP_MODULATE);
    g_Supervisor.gfxDevice->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, 2, vertices,
                                            sizeof(VertexTex1DiffuseXyzrhw));
    this->SetBlendMode(255);
}

void AnmManager::TakeScreenshot(i32 textureId, i32 srcLeft, i32 srcTop, i32 srcWidth, i32 srcHeight,
                                i32 dstLeft, i32 dstTop, i32 dstWidth, i32 dstHeight)
{
    if (!this->textures[textureId])
    {
        return;
    }

    Flush();

    u32 *pixelData = new u32[srcWidth * srcHeight];
    g_Supervisor.gfxDevice->ReadPixels(srcLeft, srcTop, srcWidth, srcHeight, pixelData);

    if (srcWidth == dstWidth && srcHeight == dstHeight)
    {
        g_Supervisor.gfxDevice->BindTexture(this->textures[textureId]);
        g_Supervisor.gfxDevice->SetTextureSubImage(dstLeft, dstTop, dstWidth, dstHeight, pixelData);
    }
    else
    {
        SDL_Surface *srcSurf = SDL_CreateSurfaceFrom(srcWidth, srcHeight, SDL_PIXELFORMAT_RGBA32,
                                                     pixelData, srcWidth * 4);
        SDL_Surface *dstSurf = SDL_CreateSurface(dstWidth, dstHeight, SDL_PIXELFORMAT_RGBA32);

        if (srcSurf && dstSurf)
        {
            SDL_BlitSurfaceScaled(srcSurf, NULL, dstSurf, NULL, SDL_SCALEMODE_LINEAR);
            g_Supervisor.gfxDevice->BindTexture(this->textures[textureId]);
            g_Supervisor.gfxDevice->SetTextureSubImage(dstLeft, dstTop, dstWidth, dstHeight,
                                                       dstSurf->pixels);
        }

        if (srcSurf)
        {
            SDL_DestroySurface(srcSurf);
        }
        if (dstSurf)
        {
            SDL_DestroySurface(dstSurf);
        }
    }

    delete[] pixelData;
}

void AnmManager::CopyTexture(i32 dstIdx, i32 srcIdx, SDL_Rect *dstRect, SDL_Rect *srcRect)
{
    if (!this->textures[dstIdx])
    {
        return;
    }
    if (!this->textures[srcIdx])
    {
        return;
    }

    this->Flush();

    u8 *dstPixels = (u8 *)this->imageDataArray[dstIdx];
    u8 *srcPixels = (u8 *)this->imageDataArray[srcIdx];

    if (!srcPixels || !dstPixels)
    {
        return;
    }

    SDL_Surface *srcSurface =
        SDL_CreateSurfaceFrom(this->textureWidths[srcIdx], this->textureHeights[srcIdx],
                              SDL_PIXELFORMAT_RGBA32, srcPixels, this->texturePitches[srcIdx]);

    SDL_Surface *dstSurface =
        SDL_CreateSurfaceFrom(this->textureWidths[dstIdx], this->textureHeights[dstIdx],
                              SDL_PIXELFORMAT_RGBA32, dstPixels, this->texturePitches[dstIdx]);

    if (srcSurface && dstSurface)
    {
        SDL_SetSurfaceBlendMode(srcSurface, SDL_BLENDMODE_NONE);

        SDL_BlitSurfaceScaled(srcSurface, srcRect, dstSurface, dstRect, SDL_SCALEMODE_LINEAR);

        g_Supervisor.gfxDevice->BindTexture(this->textures[dstIdx]);
        g_Supervisor.gfxDevice->SetTextureImage(this->textureWidths[dstIdx],
                                                this->textureHeights[dstIdx], PIXEL_RGBA,
                                                PIXEL_UNSIGNED_BYTE, dstPixels);
    }

    if (srcSurface)
    {
        SDL_DestroySurface(srcSurface);
    }
    if (dstSurface)
    {
        SDL_DestroySurface(dstSurface);
    }
}

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
            shouldSetInterrupt = g_AnmManager->textures[vm->sprite->sourceFileIndex].id != 0;
        }
        if (shouldSetInterrupt)
        {
            vm->pendingInterrupt = interrupt;
        }
        vm++;
        vmCount--;
    }
}

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

ZunResult AnmManager::UpdateTrail(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, i32 count)
{
    f32 num;
    f32 fVar4;
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
    fVar4 = num / (f32)((i32)((count + 1) / 2) - 1);

    for (i = 0, uvX = startuvX; i < count; i += 2, vertex += 2, uvX = uvX - fVar4)
    {
        vertex->textureUV.x = uvX;
        vertex->textureUV.y = uvY;
        vertex->diffuse.color = vm->color.color;
        vertex->w = 1.0f;
    }

    uvY = vm->sprite->uvEnd.y + vm->uvScrollPos.y;
    vertex = vertices + 1;

    for (i = 1, uvX = startuvX; i < count; i += 2, vertex += 2, uvX = uvX - fVar4)
    {
        vertex->textureUV.x = uvX;
        vertex->textureUV.y = uvY;
        vertex->diffuse.color = vm->color.color;
        vertex->w = 1.0f;
    }

    return ZUN_SUCCESS;
}

ZunResult AnmManager::DrawTriangleStrip(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, i32 count)
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

    if (this->currentTexture != this->textures[vm->sprite->sourceFileIndex])
    {
        this->currentTexture = this->textures[vm->sprite->sourceFileIndex];
        g_Supervisor.gfxDevice->BindTexture(this->currentTexture);
    }

    if (this->currentVertexShader != 3)
    {
        this->currentVertexShader = 3;
    }

    SetRenderStateForVm(vm);
    g_Supervisor.gfxDevice->DrawPrimitiveUP(PRIM_TRIANGLE_STRIP, count - 2, vertices,
                                            sizeof(VertexTex1DiffuseXyzrhw));
    return ZUN_SUCCESS;
}
