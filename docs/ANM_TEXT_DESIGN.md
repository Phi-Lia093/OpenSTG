"""Design for the text form of the instruction streams.

Facts, measured from the shipped data:

    984 streams, 7602 instructions, 47 distinct opcodes
    instruction sizes 8, 12, 16, 20, 24, 28 bytes
    opcode -1 appears with size 0 (230 times): the interpreter returns on the opcode alone
    opcodes 32 and 35 take 28 bytes: five arguments

The current binary form:

    i16 opcode; u16 size; i16 time; u16 flags; u32 args[]
    size = 8 + 4 * len(args)

and `flags` bit N says argument N is a reference to a variable register rather than a value.
Arguments are read as int, float or bytes depending on the opcode -- `ANM_INTERP_POS` reads
args[1].b[0], a single byte, which is what makes the type decision opcode-specific rather than
global.

------------------------------------------------------------------------------------------------
What the text has to preserve

Every instruction keeps four things: opcode, time, flags and its arguments with their exact
bit patterns. Size is derived, so it is not written. A round trip must reproduce the binary
byte for byte, which is the test.

------------------------------------------------------------------------------------------------
Proposed form

One Lua file per animation, not per entry:

    anm/title01/animation.lua

    return {
      entries = {
        [0] = {
          texture = "texture_0.png",
          width = 512, height = 256, format = 5,
          sprites = {
            { id = 0, rect = { 0, 0, 80, 32 } },
          },
          scripts = {
            [0] = {
              { "SET_ACTIVE_SPRITE", 0, 4 },                       -- no arguments
              { "SET_SCALE", 0, 1.0, 1.0 },                        -- floats
              { "JUMP", 0, 116 },                                  -- target, time
              { "INTERP_POS", 30, 0x00, 0, 0, 0 },                 -- five arguments
              { v = { "JUMP", 116, 4 } },                          -- a register reference
            },
          },
        },
      },
    }

Why one file per animation rather than one per entry:

  * an animation is the unit the game loads, and its entries only ever load together
  * 101 files instead of 101, with one open per animation rather than one per entry
  * the entry's number is its key, so the loader's walk over entries is a loop over keys

Why names rather than numbers: the number is an implementation detail of the original binary,
and a name is what makes the file readable. The mapping is fixed and lives in the converter and
the loader from one table.

Why positional arguments: an instruction is a list, and the argument positions are what the
interpreter indexes. Naming them would add a second, redundant description of each opcode.

Why a separate marker for registers: `flags` is a bitmask over argument positions, and a
register reference is not a value. Writing `v = { ... }` for the whole instruction keeps the
common case -- no registers -- as a plain list, and makes the rare one visible.

------------------------------------------------------------------------------------------------
What this costs and what it buys

Cost: a Lua runtime in the game, and parsing at load time. Measured against the binary form,
the text is smaller (no padding, no size field) but slower to read.

Buy: the instruction streams become visible and editable, which the binary form never was.
An animation's behaviour can be read, diffed and changed without a hex editor.

------------------------------------------------------------------------------------------------
Order of work

  1. the opcode table, complete, with each opcode's argument kinds -- including the byte ones
  2. the converter: binary -> Lua, and a checker that reads the Lua back and compares it with
     the binary field by field. Nothing runs the Lua yet.
  3. the loader: a Lua runtime that builds the same instruction array the binary would give,
     verified by comparing the two arrays in memory at load time.
  4. only then is the .bin dropped

Step 3 is where the previous attempt failed. It failed because all of the above was done at
once and the fault could have been in any part of it; doing 1 and 2 first, each checked on its
own, means that by the time the game runs Lua the translation is already known correct and only
the runtime remains.
"""

print(__doc__)
