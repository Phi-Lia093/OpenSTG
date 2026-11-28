#include "script/AnmVmDebug.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <SDL3/SDL_stdinc.h>

#include "script/AnmVm.hpp"

namespace AnmVmDebug
{
namespace
{
// The file grows by a few thousand lines a second when the game is busy, so it stops after
// enough to read. A few frames of every VM is the useful part; minutes of it is not.
constexpr int kLimit = 20000;

int g_written = 0;

bool ReadFlag()
{
    if (SDL_getenv("TH07_ANM_DEBUG"))
    {
        return true;
    }
    const char *setting = SDL_getenv("TH07_ANM_DUMP");
    return setting != nullptr && setting[0] == '1';
}
} // namespace

bool Enabled()
{
    // Read once: getenv on every VM every frame is a needless cost even when it returns the
    // same answer.
    static const bool enabled = ReadFlag();
    return enabled;
}

void Dump(AnmVm *vm)
{
    if (!Enabled() || g_written >= kLimit)
    {
        return;
    }
    g_written++;

    FILE *file = fopen("anmvm.txt", "a");
    if (!file)
    {
        return;
    }

    // One line, every field that decides whether and where the sprite draws. `instr` is the
    // slot rather than an address, because the instructions are Lua and the pointer is a
    // packed entry-and-stream number.
    fprintf(file,
            "%p slot=%-5d vis=%d act=%d a=%-3u spr=%-6d tex=%-3d "
            "pos=%.1f,%.1f sc=%.3f,%.3f t=%-5u intr=%d stop=%d instr=%p\n",
            (const void *)vm, (int)vm->anmFileIdx, (int)vm->visible, (int)vm->active,
            (unsigned)vm->color.bytes.a, (int)vm->activeSpriteIdx,
            (vm->sprite && vm->sprite->sourceFileIndex >= 0) ? (int)vm->sprite->sourceFileIndex
                                                             : -1,
            (double)vm->pos.x, (double)vm->pos.y, (double)vm->scale.x, (double)vm->scale.y,
            (unsigned)vm->currentTimeInScript.GetCurrent(), (int)vm->pendingInterrupt,
            (int)vm->isStopped, (const void *)vm->currentInstruction);
    fclose(file);
}

void Note(const char *format, ...)
{
    if (!Enabled())
    {
        return;
    }

    FILE *file = fopen("anmvm.txt", "a");
    if (!file)
    {
        return;
    }
    fprintf(file, "-- ");
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
    fprintf(file, "\n");
    fclose(file);
}
} // namespace AnmVmDebug
