#pragma once

#include <string>

#include "core/inttypes.hpp"

struct AnmVm;

// Running an animation's scripts as Lua coroutines.
//
// A stream is a Lua function with one method call per instruction. The interpreter decided
// whether to keep going by comparing an instruction's time with the clock; at(t) makes the same
// decision, yielding while the clock is behind the requested time. So a frame runs as many
// instructions as the interpreter would, but the position lives in a coroutine instead of a
// pointer into a buffer.
//
// An interrupt asks for the stream to resume at a label. The interpreter found the label by
// scanning the stream; here the stream is built again with that label as its entry point, which
// skips everything before it the same way.
//
// Before any of this is trusted, two checks prove the Lua says what the binary says:
// tools/verify_scripts_lua.py compares the written form, and tools/verify_runtime.py runs each
// stream and compares the instructions it calls. Both cover all 984 streams.
namespace AnmScript
{
// Whether the animation has a Lua form at all.
bool Exists(const char *anmName);

// Whether this script slot should run through Lua.
//
// TH07_ANM_LUA_SLOT names one slot to run through the runtime; every other slot keeps the binary
// path. It is how a single stream is compared against its binary equivalent before anything else
// moves.
bool ShouldUseLua(i32 slot);

// Starts the stream `scriptIndex` of `entryIndex` for `vm`. Returns false when the animation or
// the stream is missing, in which case the caller should keep the binary path.
bool Start(AnmVm *vm, const char *anmName, i32 entryIndex, i32 scriptIndex);

// Whether this VM is running a stream through Lua. AnmVm cannot carry a flag of its own -- its
// constructor memsets it -- so the runtime answers by looking in its own table.
bool HasCoroutine(const AnmVm *vm);

// Runs the coroutine as far as this frame allows. Returns 1 when the script ended, 0 while it is
// still running -- the same answer the interpreter's ExecuteScript gave.
i32 Run(AnmVm *vm, i32 time);

// Drops the coroutine a VM holds, if any.
void Release(AnmVm *vm);

// Closes the Lua state.
void Shutdown();
} // namespace AnmScript
