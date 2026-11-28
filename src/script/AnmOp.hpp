#pragma once

#include <string>

#include "core/inttypes.hpp"

struct AnmVm;
struct AnmRawInstr;

// What an instruction did, as far as the loop in ExecuteScript is concerned.
//
// The switch said this with its own control flow: a jump changed the instruction pointer and
// started again, an exit returned 1, everything else fell through to the next instruction.
// Naming the three makes the loop's decisions explicit, which they have to be before any body
// can move elsewhere -- and the Lua runtime wants the same answer from an instruction.
enum class OpResult
{
    Next,          // advance to the next instruction
    Restart,       // the instruction pointer moved; start again from it
    Stop,          // the script ended
    EndFrame,      // nothing more happens this frame; run the per-frame update
    NotHandled,    // this instruction is not one of the lifted ones
};

// The instructions that have been lifted out of the interpreter.
//
// Each one is a self-contained assignment to the VM, so it can run without the switch around it:
// that is the condition for being lifted at all. Instructions that share control flow with the
// loop -- STOP, WAIT, the interrupt scan -- are not here and still go through the switch.
//
// `Run` returns NotHandled for anything not lifted, and the caller falls back to the switch.
// That is what makes the lift incremental: the set grows one instruction at a time, and the
// game behaves the same before and after each addition.
namespace AnmOp
{
OpResult Run(AnmVm *vm, const AnmRawInstr *instr);

// The instruction's arguments, read the way the interpreter reads them: as a register reference
// when the flags say so, as a value otherwise.
i32 ArgInt(AnmVm *vm, const AnmRawInstr *instr, i32 index);
f32 ArgFloat(AnmVm *vm, const AnmRawInstr *instr, i32 index);
i32 *ArgIntPtr(AnmVm *vm, const AnmRawInstr *instr, i32 index);
f32 *ArgFloatPtr(AnmVm *vm, const AnmRawInstr *instr, i32 index);
} // namespace AnmOp
