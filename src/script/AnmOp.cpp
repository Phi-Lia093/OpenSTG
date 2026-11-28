#include "script/AnmOp.hpp"

#include <cmath>
#include <cstring>

#include "core/Rng.hpp"
#include "core/utils.hpp"
#include "script/AnmManager.hpp"
#include "script/AnmVm.hpp"

extern AnmManager *g_AnmManager;

namespace AnmOp
{
namespace
{
// The argument readers, spelled the way the interpreter's macros spell them: an argument is a
// register reference when `flags` has its bit set, and a literal otherwise.
//
// They take the instruction and the VM rather than reading them from an enclosing scope, which
// is what lets the same expression appear here and in ExecuteScript without either knowing
// about the other.
const u32 *Args(const AnmRawInstr *instr)
{
    return (const u32 *)instr->args;
}
} // namespace

i32 ArgInt(AnmVm *vm, const AnmRawInstr *instr, i32 index)
{
    if (instr->flags & (1 << index))
    {
        return vm->GetVarValue((i32)Args(instr)[index]);
    }
    return (i32)Args(instr)[index];
}

f32 ArgFloat(AnmVm *vm, const AnmRawInstr *instr, i32 index)
{
    f32 value;
    memcpy(&value, &Args(instr)[index], sizeof(value));
    if (instr->flags & (1 << index))
    {
        return vm->GetFloatVarValue(value);
    }
    return value;
}

i32 *ArgIntPtr(AnmVm *vm, const AnmRawInstr *instr, i32 index)
{
    return vm->GetVar((i32 *)&instr->args[index].i, instr->flags, index);
}

f32 *ArgFloatPtr(AnmVm *vm, const AnmRawInstr *instr, i32 index)
{
    return vm->GetFloatVar((f32 *)&instr->args[index].f, instr->flags, index);
}

OpResult Run(AnmVm *vm, const AnmRawInstr *instr)
{
    switch (instr->opcode)
    {
    case 0:
        // No enumerator and no case in the interpreter either; it falls to the default and
        // does nothing.
        return OpResult::Next;

    case ANM_22:
        // Opcode 22 has no enumerator, so it was named after its number. What it does is set
        // both anchor bits, which centres the sprite: anchor bit 0 skips the horizontal
        // offset and bit 1 the vertical one, and the renderer checks exactly those in eight
        // places. It is not a no-op and is called 262 times in the shipped data.
        vm->anchor = 3;
        return OpResult::Next;

    case ANM_SET_ACTIVE_SPRITE:
        vm->visible = 1;
        g_AnmManager->SetActiveSprite(
            vm, ArgInt(vm, instr, 0) + g_AnmManager->spriteIndices[vm->anmFileIdx]);
        vm->timeOfLastSpriteSet = vm->currentTimeInScript.GetCurrent();
        return OpResult::Next;

    case ANM_SET_TRANSLATION:
    {
        const f32 x = ArgFloat(vm, instr, 0);
        const f32 y = ArgFloat(vm, instr, 1);
        const f32 z = ArgFloat(vm, instr, 2);
        if (!vm->useOffset)
        {
            vm->pos = ZunVec3(x, y, z);
        }
        else
        {
            vm->offset = ZunVec3(x, y, z);
        }
        return OpResult::Next;
    }

    case ANM_SET_ALPHA:
        vm->color.bytes.a = (u8)(instr->args[0].i & 255);
        return OpResult::Next;

    case ANM_SET_COLOR:
        vm->color.color = (vm->color.color & 0xff000000) | (instr->args[0].i & 0xffffff);
        return OpResult::Next;

    case ANM_SET_ZWRITE_DISABLE:
        vm->zWriteDisable = instr->args[0].i;
        return OpResult::Next;

    case ANM_SET_SCROLLVEL_X:
        vm->uvScrollVel.x = ArgFloat(vm, instr, 0);
        return OpResult::Next;

    case ANM_SET_SCROLLVEL_Y:
        vm->uvScrollVel.y = ArgFloat(vm, instr, 0);
        return OpResult::Next;

    case ANM_SET_VISIBILITY:
        vm->visible = instr->args[0].i;
        return OpResult::Next;

    case ANM_SET_AUTO_ROTATE:
        vm->autoRotate = instr->args[0].us[0];
        return OpResult::Next;

    case ANM_FLIP_X:
        vm->flip ^= 1;
        vm->scale.x *= -1.0f;
        vm->updateScale = 1;
        return OpResult::Next;

    case ANM_FLIP_Y:
        vm->flip ^= 2;
        vm->scale.y *= -1.0f;
        vm->updateScale = 1;
        return OpResult::Next;

    case ANM_SET_SCALE:
        vm->scale.x = ArgFloat(vm, instr, 0);
        vm->scale.y = ArgFloat(vm, instr, 1);
        vm->updateScale = 1;
        return OpResult::Next;

    case ANM_SET_ROTATION:
        vm->rotation.x = ArgFloat(vm, instr, 0);
        vm->rotation.y = ArgFloat(vm, instr, 1);
        vm->rotation.z = ArgFloat(vm, instr, 2);
        vm->updateRotation = 1;
        return OpResult::Next;

    case ANM_SET_ANGLE_VEL:
        vm->angleVel.x = ArgFloat(vm, instr, 0);
        vm->angleVel.y = ArgFloat(vm, instr, 1);
        vm->angleVel.z = ArgFloat(vm, instr, 2);
        vm->updateRotation = 1;
        return OpResult::Next;

    case ANM_SET_SCALE_SPEED:
        vm->scaleGrowth.x = ArgFloat(vm, instr, 0);
        vm->scaleGrowth.y = ArgFloat(vm, instr, 1);
        return OpResult::Next;

    case ANM_FADE:
        // Starts an alpha interpolation towards args[0]'s first byte, over args[1] frames,
        // with no easing.
        vm->colorInterpInitialColor.bytes.a = vm->color.bytes.a;
        vm->colorInterpFinalColor.bytes.a = instr->args[0].b[0];
        vm->interpStartTimes[2] = 0;
        vm->interpEndTimes[2] = ArgInt(vm, instr, 1);
        vm->easeModes[2] = 0;
        return OpResult::Next;

    case ANM_SET_SCROLL_POS_X:
        vm->uvScrollPos.x += ArgFloat(vm, instr, 0);
        if (vm->uvScrollPos.x >= 1.0f)
        {
            vm->uvScrollPos.x -= 1.0f;
        }
        else if (vm->uvScrollPos.x < 0.0f)
        {
            vm->uvScrollPos.x += 1.0f;
        }
        return OpResult::Next;

    case ANM_SET_SCROLL_POS_Y:
        vm->uvScrollPos.y += ArgFloat(vm, instr, 0);
        if (vm->uvScrollPos.y >= 1.0f)
        {
            vm->uvScrollPos.y -= 1.0f;
        }
        else if (vm->uvScrollPos.y < 0.0f)
        {
            vm->uvScrollPos.y += 1.0f;
        }
        return OpResult::Next;

    case ANM_SET_BLEND:
        vm->blendMode = instr->args[0].i;
        return OpResult::Next;

    case ANM_SET_USE_OFFSET:
        vm->useOffset = instr->args[0].i;
        return OpResult::Next;

    // The five interpolation starters. Each records where it starts, where it ends and how many
    // frames it takes, and the per-frame update in ExecuteScript does the rest.
    //
    // The easing mode is a byte inside args[1] rather than an argument of its own, which is why
    // these read `.b[0]`; the position and rotation variants then use args[2..4] as floats.
    case ANM_INTERP_POS:
        vm->interpStartTimes[0] = 0;
        vm->interpEndTimes[0] = ArgInt(vm, instr, 0);
        vm->easeModes[0] = (i8)instr->args[1].b[0];
        if (!vm->useOffset)
        {
            vm->posInterpInitial = vm->pos;
        }
        else
        {
            vm->posInterpInitial = vm->offset;
        }
        vm->posInterpFinal.x = ArgFloat(vm, instr, 2);
        vm->posInterpFinal.y = ArgFloat(vm, instr, 3);
        vm->posInterpFinal.z = ArgFloat(vm, instr, 4);
        return OpResult::Next;

    case ANM_INTERP_COLOR:
        vm->interpStartTimes[1] = 0;
        vm->interpEndTimes[1] = ArgInt(vm, instr, 0);
        vm->easeModes[1] = (i8)instr->args[1].b[0];
        vm->colorInterpInitialColor.bytes.r = vm->color.bytes.r;
        vm->colorInterpInitialColor.bytes.g = vm->color.bytes.g;
        vm->colorInterpInitialColor.bytes.b = vm->color.bytes.b;
        vm->colorInterpFinalColor.bytes.r = instr->args[2].b[0];
        vm->colorInterpFinalColor.bytes.g = instr->args[2].b[1];
        vm->colorInterpFinalColor.bytes.b = instr->args[2].b[2];
        return OpResult::Next;

    case ANM_INTERP_ALPHA:
        vm->interpStartTimes[2] = 0;
        vm->interpEndTimes[2] = ArgInt(vm, instr, 0);
        vm->easeModes[2] = (i8)instr->args[1].b[0];
        vm->colorInterpInitialColor.bytes.a = vm->color.bytes.a;
        vm->colorInterpFinalColor.bytes.a = instr->args[2].b[0];
        return OpResult::Next;

    case ANM_INTERP_ROTATE:
        vm->interpStartTimes[3] = 0;
        vm->interpEndTimes[3] = ArgInt(vm, instr, 0);
        vm->easeModes[3] = (i8)instr->args[1].b[0];
        vm->rotateInterpInitial = vm->rotation;
        vm->rotateInterpFinal.x = ArgFloat(vm, instr, 2);
        vm->rotateInterpFinal.y = ArgFloat(vm, instr, 3);
        vm->rotateInterpFinal.z = ArgFloat(vm, instr, 4);
        vm->updateRotation = 1;
        return OpResult::Next;

    case ANM_INTERP_SCALE_2:
        vm->interpStartTimes[4] = 0;
        vm->interpEndTimes[4] = ArgInt(vm, instr, 0);
        vm->easeModes[4] = (i8)instr->args[1].b[0];
        vm->scaleInterpInitial = vm->scale;
        vm->scaleInterpFinal.x = ArgFloat(vm, instr, 2);
        vm->scaleInterpFinal.y = ArgFloat(vm, instr, 3);
        vm->updateScale = 1;
        return OpResult::Next;

    // The arithmetic group. Each writes to the register its first argument names and reads the
    // rest as values, which is the shape the interpreter uses: `*GET_INT_PTR(0) = ...`.
    //
    // The _2 suffix marks the three-operand form (destination = a op b); without it the
    // destination is also the left operand (destination op= value).
    case ANM_MOV:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_MOV_FLOAT:
        *ArgFloatPtr(vm, instr, 0) = ArgFloat(vm, instr, 1);
        return OpResult::Next;

    case ANM_ADD_2:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1) + ArgInt(vm, instr, 2);
        return OpResult::Next;
    case ANM_ADD_FLOAT_2:
        *ArgFloatPtr(vm, instr, 0) = ArgFloat(vm, instr, 1) + ArgFloat(vm, instr, 2);
        return OpResult::Next;
    case ANM_SUB_2:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1) - ArgInt(vm, instr, 2);
        return OpResult::Next;
    case ANM_SUB_FLOAT_2:
        *ArgFloatPtr(vm, instr, 0) = ArgFloat(vm, instr, 1) - ArgFloat(vm, instr, 2);
        return OpResult::Next;
    case ANM_MUL_2:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1) * ArgInt(vm, instr, 2);
        return OpResult::Next;
    case ANM_MUL_FLOAT_2:
        *ArgFloatPtr(vm, instr, 0) = ArgFloat(vm, instr, 1) * ArgFloat(vm, instr, 2);
        return OpResult::Next;
    case ANM_DIV_2:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1) / ArgInt(vm, instr, 2);
        return OpResult::Next;
    case ANM_DIV_FLOAT_2:
        *ArgFloatPtr(vm, instr, 0) = ArgFloat(vm, instr, 1) / ArgFloat(vm, instr, 2);
        return OpResult::Next;
    case ANM_MOD_2:
        *ArgIntPtr(vm, instr, 0) = ArgInt(vm, instr, 1) % ArgInt(vm, instr, 2);
        return OpResult::Next;
    case ANM_MOD_FLOAT_2:
        *ArgFloatPtr(vm, instr, 0) = fmodf(ArgFloat(vm, instr, 1), ArgFloat(vm, instr, 2));
        return OpResult::Next;

    case ANM_ADD:
        *ArgIntPtr(vm, instr, 0) += ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_ADD_FLOAT:
        *ArgFloatPtr(vm, instr, 0) += ArgFloat(vm, instr, 1);
        return OpResult::Next;
    case ANM_SUB:
        *ArgIntPtr(vm, instr, 0) -= ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_SUB_FLOAT:
        *ArgFloatPtr(vm, instr, 0) -= ArgFloat(vm, instr, 1);
        return OpResult::Next;
    case ANM_MUL:
        *ArgIntPtr(vm, instr, 0) *= ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_MUL_FLOAT:
        *ArgFloatPtr(vm, instr, 0) *= ArgFloat(vm, instr, 1);
        return OpResult::Next;
    case ANM_DIV:
        *ArgIntPtr(vm, instr, 0) /= ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_DIV_FLOAT:
        *ArgFloatPtr(vm, instr, 0) /= ArgFloat(vm, instr, 1);
        return OpResult::Next;
    case ANM_MOD:
        *ArgIntPtr(vm, instr, 0) %= ArgInt(vm, instr, 1);
        return OpResult::Next;
    case ANM_MOD_FLOAT:
        // The original reads argument 0 as a value here rather than as the destination, which
        // is the one place in this group that differs from its neighbours.
        *ArgFloatPtr(vm, instr, 0) = fmodf(ArgFloat(vm, instr, 0), ArgFloat(vm, instr, 1));
        return OpResult::Next;

    case ANM_RAND:
        *ArgIntPtr(vm, instr, 0) = g_Rng.GetRandomU32InRange(ArgInt(vm, instr, 1));
        return OpResult::Next;
    case ANM_RAND_FLOAT:
        *ArgFloatPtr(vm, instr, 0) = g_Rng.GetRandomFloatInRange(ArgFloat(vm, instr, 1));
        return OpResult::Next;

    case ANM_SIN:
        *ArgFloatPtr(vm, instr, 0) = sinf(ArgFloat(vm, instr, 1));
        return OpResult::Next;
    case ANM_COS:
        *ArgFloatPtr(vm, instr, 0) = cosf(ArgFloat(vm, instr, 1));
        return OpResult::Next;
    case ANM_TAN:
        *ArgFloatPtr(vm, instr, 0) = tanf(ArgFloat(vm, instr, 1));
        return OpResult::Next;
    case ANM_ACOS:
        *ArgFloatPtr(vm, instr, 0) = acosf(ArgFloat(vm, instr, 1));
        return OpResult::Next;
    case ANM_ATAN:
        *ArgFloatPtr(vm, instr, 0) = atanf(ArgFloat(vm, instr, 1));
        return OpResult::Next;
    case ANM_NORMALIZE_ANGLE:
        *ArgFloatPtr(vm, instr, 0) = utils::AddNormalizeAngle(ArgFloat(vm, instr, 0), 0.0f);
        return OpResult::Next;

    // The jumps. Each sets the instruction pointer from the stream's beginning, which is what
    // Restart tells the loop to do; a conditional jump that does not take its branch is
    // an ordinary Next.
    case ANM_JUMP:
        vm->currentTimeInScript = instr->args[1].i;
        vm->currentInstruction =
            (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[0].i);
        return OpResult::Restart;

    case ANM_DEC_JUMP:
        // Decrement a counter register and jump while it stays positive, so this is a loop's
        // tail: the counter is args[0], the target args[1] and the resulting time args[2].
        (*ArgIntPtr(vm, instr, 0))--;
        if (ArgInt(vm, instr, 0) > 0)
        {
            vm->currentTimeInScript = instr->args[2].i;
            vm->currentInstruction =
                (AnmRawInstr *)((u8 *)vm->beginningOfScript + instr->args[1].i);
            return OpResult::Restart;
        }
        return OpResult::Next;

    // The twelve conditional jumps compare arguments 0 and 1 and, when the comparison holds,
    // take args[2] as the target and args[3] as the time. JumpConditional does the second half
    // for all of them, so each case is only its comparison.
    case ANM_JUMP_IF_EQ:
        if (ArgInt(vm, instr, 0) == ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_EQ_FLOAT:
        if (ArgFloat(vm, instr, 0) == ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_NEQ:
        if (ArgInt(vm, instr, 0) != ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_NEQ_FLOAT:
        if (ArgFloat(vm, instr, 0) != ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_LT:
        if (ArgInt(vm, instr, 0) < ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_LT_FLOAT:
        if (ArgFloat(vm, instr, 0) < ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_LEQ:
        if (ArgInt(vm, instr, 0) <= ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_LEQ_FLOAT:
        if (ArgFloat(vm, instr, 0) <= ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_GT:
        if (ArgInt(vm, instr, 0) > ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_GT_FLOAT:
        if (ArgFloat(vm, instr, 0) > ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_GEQ:
        if (ArgInt(vm, instr, 0) >= ArgInt(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;
    case ANM_JUMP_IF_GEQ_FLOAT:
        if (ArgFloat(vm, instr, 0) >= ArgFloat(vm, instr, 1))
        {
            g_AnmManager->JumpConditional(vm, instr);
            return OpResult::Restart;
        }
        return OpResult::Next;

    // The exits. Visible clears first for the _HIDE forms, which is a fall-through in the
    // original rather than two bodies.
    case ANM_EXIT_HIDE:
    case ANM_EXIT_HIDE2:
        vm->visible = 0;
        vm->currentInstruction = NULL;
        return OpResult::Stop;
    case ANM_EXIT:
        vm->currentInstruction = NULL;
        return OpResult::Stop;

    // The position-time starters share StartPositionInterp, and differ only in the easing they
    // set first. The three read a float triple and a duration.
    case ANM_POS_TIME_LINEAR:
        vm->easeModes[0] = 0;
        g_AnmManager->StartPositionInterp(vm, instr, 0);
        return OpResult::Next;
    case ANM_POS_TIME_DECEL:
        vm->easeModes[0] = 4;
        g_AnmManager->StartPositionInterp(vm, instr, 4);
        return OpResult::Next;
    case ANM_POS_TIME_ACCEL:
        vm->easeModes[0] = 6;
        g_AnmManager->StartPositionInterp(vm, instr, 6);
        return OpResult::Next;

    // The waits and stops, which do not simply fall through to the next instruction.
    //
    // WAIT counts down a timer over frames and only lets the script continue when it reaches
    // zero, so it ends the frame for as long as the timer lasts. STOP parks the script where it
    // is; if an interrupt is pending it handles that instead of stopping.
    case ANM_WAIT:
        if (vm->waitTimer == 0)
        {
            vm->waitTimer = ArgInt(vm, instr, 0);
        }
        else
        {
            vm->waitTimer--;
        }
        if (vm->waitTimer <= 0)
        {
            vm->waitTimer = 0;
            return OpResult::Next;
        }
        // The clock steps back by one so the frame does not count against the wait.
        vm->currentTimeInScript--;
        return OpResult::EndFrame;

    case ANM_STOP_HIDE:
        vm->visible = 0;
        // Falls into STOP, as it did in the original.
        if (!vm->pendingInterrupt)
        {
            vm->isStopped = 1;
            vm->currentTimeInScript--;
            return OpResult::EndFrame;
        }
        return g_AnmManager->HandleInterrupt(vm) ? OpResult::Restart : OpResult::EndFrame;

    case ANM_STOP:
        if (!vm->pendingInterrupt)
        {
            vm->isStopped = 1;
            vm->currentTimeInScript--;
            return OpResult::EndFrame;
        }
        return g_AnmManager->HandleInterrupt(vm) ? OpResult::Restart : OpResult::EndFrame;

    // INTERP_SCALE reads its arguments in a different order from INTERP_SCALE_2: the target
    // scale comes first and the duration third, where the _2 form takes duration, easing,
    // then the scale. Both end up in the same slots, so only the reading differs.
    case ANM_INTERP_SCALE:
        vm->interpStartTimes[4] = 0;
        vm->interpEndTimes[4] = ArgInt(vm, instr, 2);
        vm->easeModes[4] = 0;
        vm->scaleInterpInitial = vm->scale;
        vm->scaleInterpFinal.x = ArgFloat(vm, instr, 0);
        vm->scaleInterpFinal.y = ArgFloat(vm, instr, 1);
        return OpResult::Next;

    case ANM_SET_CAMERA_MODE:
        vm->cameraMode = instr->args[0].i;
        return OpResult::Next;

    default:
        return OpResult::NotHandled;
    }
}

} // namespace AnmOp
