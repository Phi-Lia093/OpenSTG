#pragma once

#include "script/AnmEntryFile.hpp"
#include "script/AnmVm.hpp"

#include <SDL3/SDL.h>
#include <assert.h>
#include <string>

#include "core/GameWindow.hpp"
#include "core/ZunColor.hpp"
#include "core/ZunMath.hpp"
#include "core/ZunResult.hpp"
#include "render/ZunGraphics.hpp"

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

struct VertexDiffuseXyzrhw
{
    ZunVec3 pos;
    f32 w;
    ZunColor diffuse;
};

struct VertexTex1DiffuseXyz
{
    ZunVec3 pos;
    ZunColor diffuse;
    Float2 textureUV;
};

extern VertexTex1DiffuseXyz g_Quad3DFallback[4];

struct VertexTex1Xyzrhw
{
    ZunVec3 pos;
    f32 w;
    Float2 textureUV;
};

extern VertexTex1Xyzrhw g_QuadTemplate[4];

struct VertexTex1DiffuseXyzrhw
{
    VertexTex1DiffuseXyzrhw()
    {
    }

    ZunVec3 pos;
    f32 w;
    ZunColor diffuse;
    Float2 textureUV;
};

extern VertexTex1DiffuseXyzrhw g_QuadVertices[4];

struct RenderVertexInfo
{
    ZunVec3 pos;
    Float2 textureUV;
};

struct ZunImageInfo
{
    u32 width;
    u32 height;
    u32 depth;
    u32 mipLevels;
    u32 format;
};

struct ZunImageInfoEmbedded
{
    i16 magic;
    i16 colorDepth;
    i16 imageType;
    i16 format;
    i16 width;
    i16 height;
    i32 unused_c;
    u8 data[];
};
static_assert(sizeof(ZunImageInfoEmbedded) == 0x10);

struct AnmRawSprite
{
    i32 id;
    Float2 offset;
    Float2 size;
};
static_assert(sizeof(AnmRawSprite) == 0x14);

struct AnmRawEntry
{
    i32 numSprites;
    i32 numScripts;
    i32 textureIdx;
    i32 width;
    i32 height;
    i32 format;
    i32 color_key;
    i32 nameOffset;
    i32 spriteIdxOffset;
    i32 mipmapNameOffset;
    i32 version;
    i32 priority;
    i32 textureOffset;
    u8 hasData;
    u8 ownsMemory;
    i16 unused_36;
    i32 nextOffset;
    i32 unused_3c;
    i32 dataOffsets[];
};
static_assert(sizeof(AnmRawEntry) == 0x40);

struct AnmEntry
{
    AnmRawEntry *raw;
    i32 spriteIndexOffset;
    i32 childCount;
    // What this entry occupies, so releasing it does not need the entry's description: the
    // description lives only for the duration of the load. The slot list is bounded by the
    // largest stream count in the shipped data, which is 66; the limit here is the same one
    // the script array uses.
    i32 textureIdx;
    i32 spriteCount;
    i32 scriptCount;
    i32 scriptSlots[320];
};

#define MAX_SCRIPTS_SPRITES 2560
#define MAX_TEXTURES 264
#define MAX_ANM_FILES 50
#define MAX_SURFACES 32

struct AnmManager
{
    AnmManager();
    ~AnmManager();

    ZunResult CalcBillboardTransform(AnmVm *vm);
    void CalcProjectedTransform(AnmVm *vm);
    void CopySurfaceToBackBuffer(i32 surfaceIdx, i32 left, i32 top, i32 x, i32 y);
    void CopyTexture(i32 dstIdx, i32 srcIdx, SDL_Rect *dstRect, SDL_Rect *srcRect);
    ZunResult CreateEmptyTexture(i32 textureIdx, u32 width, u32 height);
    ZunResult Draw(AnmVm *vm);
    ZunResult DrawBillboard(AnmVm *vm);
    ZunResult Draw3(AnmVm *vm);
    void DrawEndingRect(i32 surfaceIdx, f32 rectX, f32 rectY, f32 rectLeft, f32 rectTop, i32 width,
                        i32 height);
    ZunResult DrawFacingCamera(AnmVm *vm);
    ZunResult DrawInner(AnmVm *vm, u32 drawFlags);
    ZunResult DrawNoRotation(AnmVm *vm);
    ZunResult DrawProjected(AnmVm *vm);
    void DrawStringFormat(AnmVm *vm, u32 textColor, u32 outlineType, const char *text, ...);
    void DrawStringFormat2(AnmVm *vm, u32 textColor, u32 outlineType, const char *text, ...);
    void DrawTextToSprite(u32 spriteDstIdx, i32 x, i32 y, i32 width, i32 height, i32 fontWidth,
                          i32 fontHeight, u32 textColor, u32 outlineType, char *strToPrint,
                          f32 scaleY, f32 scaleX);
    ZunResult DrawTriangleStrip(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, i32 count);
    static void DrawVmTextFmt(AnmManager *manager, AnmVm *vm, u32 textColor, u32 outlineType,
                              const char *str, ...);
    i32 ExecuteScript(AnmVm *vm);

    // The two pieces of ExecuteScript that several opcodes share.
    //
    // They were labels inside the switch that the cases jumped to with goto. A Lua method
    // cannot jump into another function's body, so they become calls; the code they run is
    // unchanged, and the cases that reach them differ only in what they set first.
    //
    // JumpTo takes the target and time from the instruction's own arguments -- args[2] and
    // args[3] -- which is where all twelve conditional jumps carry them.
    void JumpConditional(AnmVm *vm, const AnmRawInstr *instr);
    // StartPositionInterp starts a position interpolation with the given easing; the three
    // POS_TIME_* opcodes differ only in that number.
    void StartPositionInterp(AnmVm *vm, const AnmRawInstr *instr, i32 easeMode);

    // Scans for the label a pending interrupt names, moving the VM there.
    //
    // This was the `handle_interrupt:` label, which both STOP and the check before the loop
    // jumped to. It returns what the caller should do next as an int: 1 when it found its
    // label and the loop should continue from there, 0 when there is nowhere to go.
    i32 HandleInterrupt(AnmVm *vm);

    void Flush();
    // `entryIndex` is which entry of the file this is, needed to find its sprite table: that
    // table is a TOML file beside the animation rather than a region inside it.
    // `description` is the entry as anm/<name>/<index>.toml describes it; there is no binary
    // header to pass any more. `entryIndex` names the files that belong to the entry.
    i32 LoadAnm(i32 textureIdx, const AnmEntryFile::Entry &description, i32 spriteIdxOffset,
                i32 entryIndex);
    i32 LoadAnms(i32 anmIdx, const char *path, i32 spriteIdxOffset);
    void LoadSprite(u32 spriteIdx, AnmLoadedSprite *sprite);
    ZunResult LoadSurface(i32 surfaceIdx, const char *path);
    ZunResult LoadTexture(i32 textureIdx, const char *texturePath, u32 colorKey);
    ZunResult LoadTextureAlphaChannel(i32 textureIdx, const char *texturePath);
    ZunResult LoadTextureEmbedded(u32 textureIdx, ZunImageInfoEmbedded *imageInfo);
    ZunResult PushSprite(VertexTex1DiffuseXyzrhw *spriteVertex);
    void ReleaseAnm(i32 anmIdx);
    void ReleaseSurface(i32 surfaceIdx);
    void ReleaseTexture(i32 textureIdx);
    void ReleaseVertexBuffer();
    void ResetVertexBuffer();
    ZunResult SetActiveSprite(AnmVm *vm, i32 spriteIdx);
    // Starts the stream a slot names, if it names one. Whether it does is answered by the record
    // the loader made, not by a pointer into a buffer: the instructions are Lua functions now,
    // reached by slot, so there is no buffer, and a null one still forms a pointer that reads as
    // "this slot has a script" when it has none.
    void SetAndExecuteScript(AnmVm *vm, bool hasScript);
    void SetRenderStateForVm(AnmVm *vm);
    void SetupVertexBuffer();
    void SyncRenderState(AnmVm *vm);
    void TakeScreenshot(i32 textureId, i32 srcLeft, i32 srcTop, i32 srcWidth, i32 srcHeight,
                        i32 dstLeft, i32 dstTop, i32 dstWidth, i32 dstHeight);
    void TakeScreenshotIfRequested();
    void TranslateRotation(VertexTex1DiffuseXyzrhw *vertex, f32 width, f32 height, f32 sine,
                           f32 cosine, f32 xOffset, f32 yOffset);

    void SetInterruptActiveVms(AnmVm *vm, i32 vmCount, i16 interrupt);
    void ExecuteScripts(AnmVm *startVm, i32 count);
    void ExecuteVmsAnms(AnmVm *vm, i32 idx, i32 vmCount);
    ZunResult UpdateTrail(AnmVm *vm, VertexTex1DiffuseXyzrhw *vertices, i32 count);

    void ExecuteAnmIdx(AnmVm *vm, i32 anmFileIdx)
    {
        vm->anmFileIdx = anmFileIdx;
        vm->pos = ZunVec3(0.0f, 0.0f, 0.0f);
        vm->offset = ZunVec3(0.0f, 0.0f, 0.0f);
        vm->fontHeight = 15;
        vm->fontWidth = 15;

        // Whether this slot holds a stream is answered by the record made when the animation
        // loaded, not by a pointer into a buffer. The instructions are Lua functions now, reached
        // by slot, so there is no buffer to point into -- and a null one would still produce a
        // pointer, which reads as "this slot has a script" when it has none.
        const bool hasScript =
            anmFileIdx >= 0 && anmFileIdx < ARRAY_SIZE_SIGNED(scriptOrigins) &&
            scriptOrigins[anmFileIdx].anmName[0] != '\0';

        SetAndExecuteScript(vm, hasScript);
    }

    void ReleaseSurfaces()
    {
        for (i32 i = 0; i < MAX_SURFACES; i++)
        {
            if (this->surfaces[i])
            {
                SDL_DestroySurface(this->surfaces[i]);
                this->surfaces[i] = nullptr;
            }
            if (this->surfacesBis[i])
            {
                SDL_DestroySurface(this->surfacesBis[i]);
                this->surfacesBis[i] = nullptr;
            }
        }
    }

    void SetColor(u32 color)
    {
        this->colorMulEnabled = 0;
        this->color.color = color;
    }

    void SetColorWithMulEnabled(u32 color)
    {
        this->colorMulEnabled = 1;
        this->color.color = color;
    }

    void SetAnmIdxAndExecuteScript(AnmVm *vm, i32 anmIdx)
    {
        vm->anmFileIdx = anmIdx;
        const bool hasScript =
            anmIdx >= 0 && anmIdx < ARRAY_SIZE_SIGNED(scriptOrigins) &&
            scriptOrigins[anmIdx].anmName[0] != '\0';
        this->SetAndExecuteScript(vm, hasScript);
    }

    void InitializeAndSetActiveSprite(AnmVm *vm, i32 spriteIdx)
    {
        vm->Initialize();
        this->SetActiveSprite(vm, spriteIdx);
        vm->UpdatePrev();
    }

    i32 CreateScreenshotTexture(i32 x, i32 y, i32 width, i32 height)
    {
        if (this->screenshotTextureId >= 0)
        {
            return -1;
        }
        else
        {
            this->screenshotTextureId = 4;
            this->screenshotSrcLeft = 32;
            this->screenshotSrcTop = 16;
            this->screenshotSrcWidth = 384;
            this->screenshotSrcHeight = 448;
            this->screenshotDstLeft = x;
            this->screenshotDstTop = y;

            this->screenshotDstWidth = width;
            this->screenshotDstHeight = height;
            return 0;
        }
    }

    i32 ShouldDraw(AnmVm *vm)
    {
        if (!vm->sprite)
        {
            return false;
        }
        else if (vm->sprite->sourceFileIndex < 0)
        {
            return false;
        }
        else
        {
            return this->textures[vm->sprite->sourceFileIndex].id != 0;
        }
    }

    void SetVertexShader(u8 value)
    {
        this->currentVertexShader = value;
    }

    void SetSprite(AnmLoadedSprite *value)
    {
        this->currentSprite = value;
    }

    void SetTexture(GfxTextureHandle value)
    {
        this->currentTexture = value;
    }

    void SetColorOp(u8 value)
    {
        this->currentColorOp = value;
    }

    void SetBlendMode(u8 value)
    {
        this->currentBlendMode = value;
    }

    void SetZWriteDisable(u8 value)
    {
        this->currentZWriteDisable = value;
    }

    void SetCameraMode(u8 value)
    {
        this->currentCameraMode = value;
    }

    void SetScriptTicks(i32 value)
    {
        this->scriptTicksThisFrame = value;
    }

    void SetRenderStateChanges(i32 value)
    {
        this->renderStateChangesThisFrame = value;
    }

    void SetScriptsExecuted(i32 value)
    {
        this->scriptsExecutedThisFrame = value;
    }

    void SetFlushes(i32 value)
    {
        this->flushesThisFrame = value;
    }

    void ClearFrameState()
    {
        this->scriptTicksThisFrame = 0;
        this->renderStateChangesThisFrame = 0;
        this->scriptsExecutedThisFrame = 0;
        this->flushesThisFrame = 0;
    }

    AnmLoadedSprite *GetSprite(i32 spriteIdx)
    {
        return &this->sprites[spriteIdx];
    }

    static void SetCameraModeStatic(AnmManager *mgr, i32 cameraMode)
    {
        mgr->currentCameraMode = cameraMode;
    }

    void DrawAndFlush(AnmVm *vm)
    {
        Draw(vm);
        Flush();
    }

    ZunResult DrawInterpNoRotation(AnmVm *vm)
    {
        ZunVec3 savedPos = vm->pos;
        vm->pos = vm->prevPos.Lerp(vm->pos, g_RenderAlpha);
        ZunResult result = DrawNoRotation(vm);
        vm->pos = savedPos;
        return result;
    }

    ZunResult DrawInterp(AnmVm *vm)
    {
        ZunVec3 savedPos = vm->pos;
        vm->pos = vm->prevPos.Lerp(vm->pos, g_RenderAlpha);
        ZunResult result = Draw(vm);
        vm->pos = savedPos;
        return result;
    }

    void DrawInterpAndFlush(AnmVm *vm)
    {
        DrawInterp(vm);
        Flush();
    }

    ZunColor color;
    i32 colorMulEnabled;
    i32 scriptsExecutedThisFrame;
    i32 scriptTicksThisFrame;
    i32 renderStateChangesThisFrame;
    u32 flushesThisFrame;
    Float2 offset;
    Float2 shakeOffset;
    Float2 prevShakeOffset;
    ZunMatrix matrix;
    AnmLoadedSprite sprites[MAX_SCRIPTS_SPRITES];
    AnmVm vm;
    GfxTextureHandle textures[MAX_TEXTURES];
    void *imageDataArray[256];
    // What each script slot names, filled as animations load.
    //
    // scripts[] is indexed by a stream's number plus the animation's offset, which is what a VM's
    // anmFileIdx is -- a different space from anmFiles, which is indexed by the texture slot.
    // Starting a stream through Lua needs the animation's name, its entry and its stream, and
    // none of those can be recovered from the slot number alone.
    struct ScriptOrigin
    {
        char anmName[64];
        i32 entryIndex;
        i32 streamIndex;
    };
    ScriptOrigin scriptOrigins[MAX_SCRIPTS_SPRITES];

    char *textureNames[MAX_TEXTURES];
    // Storage for the names above. They used to point into the loaded animation, so they died
    // with it; the description they come from is only alive for the duration of the load, so
    // the text has to be copied somewhere that lasts as long as the texture does.
    char textureNamesOwned[MAX_TEXTURES][96];
    // The file currently being loaded, without its extension. LoadAnm finds an entry's sprite
    // table by it, since the table is a separate file rather than a region of the animation.
    //
    // A plain array rather than std::string because the constructor memsets the whole object:
    // a std::string here would have its internals zeroed behind its back, and every use would
    // then read a pointer the class no longer owns.
    char currentAnmName[64];
    i32 loadedSpriteCount;
    AnmRawInstr *scripts[MAX_SCRIPTS_SPRITES];
    i32 spriteIndices[MAX_SCRIPTS_SPRITES];
    AnmEntry anmFiles[MAX_ANM_FILES];
    SDL_Surface *surfaces[MAX_SURFACES];
    SDL_Surface *surfacesBis[MAX_SURFACES];
    GfxTextureHandle surfaceTextures[MAX_SURFACES];
    u32 textureWidths[MAX_TEXTURES];
    u32 textureHeights[MAX_TEXTURES];
    u32 texturePitches[MAX_TEXTURES];
    ZunImageInfo surfaceSourceInfo[MAX_SURFACES];
    ZunColor currentTextureFactor;
    GfxTextureHandle currentTexture;
    u8 currentBlendMode;
    u8 currentColorOp;
    u8 currentVertexShader;
    u8 currentZWriteDisable;
    u8 currentCameraMode;
    // pad 3
    AnmLoadedSprite *currentSprite;
    RenderVertexInfo vertexBufferContents[4];
    u32 spritesToDraw;
    VertexTex1DiffuseXyzrhw spriteVertexBuffer[49152];
    VertexTex1DiffuseXyzrhw *vertexBufferCurPtr;
    VertexTex1DiffuseXyzrhw *vertexBufferStartPtr;
    i32 screenshotTextureId;
    i32 screenshotSrcLeft;
    i32 screenshotSrcTop;
    i32 screenshotSrcWidth;
    i32 screenshotSrcHeight;
    i32 screenshotDstLeft;
    i32 screenshotDstTop;
    i32 screenshotDstWidth;
    i32 screenshotDstHeight;
};

extern AnmManager *g_AnmManager;
