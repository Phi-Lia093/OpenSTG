#pragma once

#include "core/inttypes.hpp"

struct AnmVm;

// Dumping VM state for a window of frames, so a runtime problem can be read rather than inferred.
//
// The fields are the ones that decide what a VM draws: whether it is visible, which sprite it
// resolved, where it is, how faded it is, and where its script has got to. A frame of these for
// every VM says what the whole animation system thinks it is doing.
namespace AnmVmDebug
{
// Whether the dump is on. Set once from the environment.
bool Enabled();

// Writes one line for this VM if the bound has not been reached.
void Dump(AnmVm *vm);

// Writes a line of the caller's own, for events that are not a VM state.
void Note(const char *format, ...);
} // namespace AnmVmDebug
