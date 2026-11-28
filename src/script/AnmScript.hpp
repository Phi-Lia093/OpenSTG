#pragma once

#include <string>

#include "core/inttypes.hpp"

struct AnmVm;

// Running an animation's scripts as Lua coroutines.
//
// A stream is a Lua function that calls one method per instruction and advances time with
// at(t). Where the original decided whether to keep going by comparing `instr->time` with the
// clock, at(t) does the same thing by yielding when the requested time has not arrived yet and
// returning immediately when it has. The VM therefore stops in the same place for the same
// reason, but the position lives in the coroutine rather than in a pointer into a buffer.
//
// One coroutine per VM, created when a script is started and discarded when it ends or is
// replaced. Nothing is shared between VMs, so a stream cannot see another's state.
namespace AnmScript
{
// Whether the animation has a Lua form at all.
bool Exists(const char *anmName);

// Starts the stream at `scriptIndex` for `vm`, replacing any it was running. Returns false when
// the animation, the entry or the stream is missing.
bool Start(AnmVm *vm, const char *anmName, i32 entryIndex, i32 scriptIndex);

// Runs the coroutine until it yields or finishes.
//
// `time` is the VM's script clock. The return value is what the original ExecuteScript returns:
// 1 when the stream ended, 0 when it is still running. `stopped` is set when the coroutine asked
// to stop for this frame, which the caller uses to leave the VM where it is.
i32 Run(AnmVm *vm, i32 time, bool *stopped);

// Drops the coroutine a VM is holding, if any. Called when the animation that owns it is
// released, before the Lua state is closed.
void Release(AnmVm *vm);

// Closes the Lua state. Called at shutdown.
void Shutdown();
} // namespace AnmScript
