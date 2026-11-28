# ANM replacement — plan

Replace the binary animation format with three files per animation: a PNG atlas, a TOML
description of everything that is not a program, and a Lua script that reproduces the
original virtual machine's instructions one to one.

The rule that governs the first step is **no abstraction**. Every original instruction
becomes exactly one Lua call with the same arguments. Nothing is inferred, nothing is
grouped, nothing is renamed. The point is to prove the pipeline end to end and to have a
check that can fail: the instruction stream rebuilt from Lua must equal the one the original
binary produces.

Abstraction comes after, on top of a layer that is already known to be correct.

## Feasibility check — done

Before writing any of this, the one mechanism everything rests on was tested: a Lua script,
run once, calling C++ functions that append to a buffer.

`tools/build_lua_static.py` builds Lua 5.4 from `D:/lua-5.4.7/src` into
`.tools/static/lib/lua54-static.lib`, staging the headers beside SDL's. `lua.c` and `luac.c`
are excluded: they are the standalone interpreter and compiler, each with its own `main`.

`tools/test_lua.py` builds and runs a small program that creates a state, exposes a
`record(opcode, time, ...)` collector, runs a script that calls it through a table method,
and compares what was collected against what was expected. It passes, and the resulting
executable depends on nothing but `KERNEL32.dll`, so the static-link property holds.

Two facts learned from it, both now settled for the real implementation:

- **C linkage is required.** Lua's headers declare their functions with C linkage only when
  compiled as C. A C++ caller that does not wrap them in `extern "C"` gets mangled names and
  fails to link on every symbol.
- **The calls use a colon.** `a:record(...)` passes the script table as argument 1, so the
  caller's arguments start at 2. The generated scripts use that form, so the offset is fixed
  in the host rather than left to chance.

`409.6` survived the Lua-to-C++ trip exactly, which matters because sprite positions go
through this path.

## Why this shape

The previous attempt failed because it tried to reinterpret the data directly into
keyframes, and there was no reference to check the reinterpretation against. Guessing at
what a loop meant, in what order to emit an exit, and where a jump should land produced a
stream that looked plausible and behaved differently.

This plan inverts that. The Lua is a transcription, so the expected output is known exactly —
the original instructions — and a mismatch is a bug in the small translation layer rather
than a question about intent.

## Layout

An ANM file holds one or more entries, each with its own texture and scripts. Each entry
becomes its own directory, because each genuinely has a separate atlas:

```
th07.data/anm/title01/
    00/
        texture.png      the atlas, decoded from the embedded THTX
        uvs.toml         sprite rectangles in texture pixels
        animation.lua    the instruction stream
    01/
        ...
```

Entry directories are numbered, because the original identifies them by position in the
file, not by name. `title01` has ten.

Note `anm/`, not `anim/`. The previous attempt used `anim/` and the distinction is worth
keeping clear from that attempt's output.

The ANM file extension disappears. `AnmManager::LoadAnms` looks for `anm/<name>/` first and
falls back to the binary format, so files can be converted one at a time.

### texture.png

The whole atlas, decoded to RGBA8. No slicing, no splitting by entry: an ANM with ten
entries produces ten directories, each with its own atlas, because each entry genuinely has
its own texture.

### uvs.toml

Everything the original stores outside the instruction stream: the sprite table, the texture
facts the loader needs, and the entry's identity.

```toml
# anm/title01/00/uvs.toml
version = 2
texture = "texture.png"
width = 512              # as declared in the original, used for the cols/rows scaling
height = 512

[[sprite]]
id = 0
rect = [0, 0, 80, 32]    # x, y, width, height in texture pixels
```

The rectangle is in pixels rather than UV coordinates because that is what the original
stores. `AnmManager::LoadSprite` divides by the texture size to get UVs, so keeping pixels
means the conversion has no chance to introduce a rounding difference.

### animation.lua

The instruction stream, one call per instruction.

```lua
-- anm/title01/00/animation.lua
local a = anm.script(0)      -- script index within this entry

a:set_sprite(0, { time = 0 })
a:set_anchor_center({ time = 0 })
a:set_z_write_disable(0, { time = 0 })
a:set_alpha(0, { time = 0 })
a:set_position(432.0, 200.0, 0.0, { time = 0 })
a:interrupt_label(2)
a:fade(255, 16, { time = 0 })
a:move_linear(400.0, 200.0, 0.0, 60, { time = 0 })
```

Two things this spelling buys:

- **The argument types are explicit in the name.** `set_alpha` takes an integer,
  `set_position` takes three floats, `fade` takes a byte and an integer. The original reads
  its `AnyArg` union differently per opcode, and a call that took an untyped number would
  lose that distinction. This is why the calls are not a single `exec(opcode, ...)`.
- **`time` is a named argument.** It is not part of the opcode but of the instruction
  header, and every instruction has one. Making it explicit keeps the transcription literal.

Scripts with several interrupt entry points are several `anm.script(n)` calls, matching the
original's script indices.

## The conversion layer

`tools/convert_anm.py` reads an ANM with thtk's `thanm`, which already reports each
instruction as `time opcode args...`. It writes the three files.

The translation is mechanical: opcode to method name, arguments to arguments. The table is
fixed and small.

| Piece | Source | Notes |
| --- | --- | --- |
| Texture | `thanm -x` | Emits a standard PNG already; no THTX decoder needed |
| Sprites | `thanm -l`, `Sprite:` lines | `w*h+x+y` becomes `rect` |
| Instructions | `thanm -l`, `Instruction:` lines | time, opcode, arguments |

## The runtime

`AnmManager` keeps its interpreter, its `AnmVm` state and its timing. What changes is where
`AnmRawInstr` arrays come from: instead of parsing the binary entry, it runs the Lua script
once at load time and collects the instructions the calls build.

```
LoadAnms("data/title01.anm")
  |
  +-- anm/title01/ exists?
        yes -> for each entry directory:
                 build AnmRawEntry, sprites and texture as today
                 open animation.lua, run it with a context that appends
                 instructions to a buffer
                 hand the buffer to the existing LoadAnm
        no  -> existing binary path, unchanged
```

The Lua host is deliberately thin. Each method appends one `AnmRawInstr` with the opcode,
the time, the flags and the four-byte arguments the original would have had. Nothing is
interpreted; the array is what the VM walks afterwards, exactly as before.

This is the point of the design: the VM never learns that Lua exists.

### The API surface

One method per opcode, generated from the interpreter's switch statement rather than typed
by hand, so a missing or mistyped method is a build-time and test-time problem rather than a
runtime surprise.

Each method's signature is decided by how the interpreter reads its arguments:

- integer opcodes take integers
- float opcodes take floats
- `fade` takes a byte then an integer, because the original reads `args[0].b[0]`
- arguments that name a VM variable carry the variable numbers the original uses
  (10000..10009), not a separate mechanism

## Verification

The check that makes this safe, and the reason for the whole approach:

1. **Reconstruct and compare.** For every converted animation, run the Lua through the host
   to build the instruction array, and compare it against the array produced by loading the
   original binary. Every instruction: opcode, time, flags, argument bytes. This is an exact
   comparison with no tolerance, and it either passes or names the first difference.
2. **Run the game.** With the comparison passing, the game's behaviour follows by
   construction, because it consumes the same array. Playing the title screen and the menus
   is then a sanity check rather than the primary test.
3. **Coverage.** Report every file converted and every one still binary.

Step 1 removes the guessing that sank the previous attempt. A failure names the animation,
the entry, the script, the instruction index and the mismatching field.

## Order of work

1. Get Lua building as a static library, alongside SDL, so it stays a single self-contained
   executable.
2. Write the host: create a script, expose the methods, collect the instructions.
3. Write a generator that produces the method table from the interpreter's switch, with a
   test that every defined opcode has a method.
4. Hand-convert one small sprite set with no loops and no interrupts, and make step 1 above
   pass for it.
5. Convert `title01`, and make the comparison pass for all 123 of its scripts.
6. Play the title screen.
7. Convert the rest, in groups, comparing after each group.

Steps 1 to 5 are the feasibility question. If the comparison passes for a file with loops,
interrupts and sub-tweens, the approach works and the remaining files are repetition.

## What this does not do yet

- **No abstraction.** No keyframes, no scenes, no easing names. The Lua reads like the
  byte-code it came from.
- **No replacement of the VM.** The interpreter stays; it is what makes the comparison
  meaningful.
- **No re-authoring.** Nothing is reshaped for readability.

That ordering is deliberate: a layer that transcribes correctly can be built on, and a
layer that reinterprets cannot be trusted.

## Open questions

- **Lua version and source.** The project has no Lua. A static build of Lua 5.4 is the
  assumption; it needs the source placing beside the SDL sources.
- **Where the method table is generated from.** The interpreter is one large switch with
  fallthrough labels (`interp_pos` is shared by four opcodes). The generator needs to handle
  those shared cases rather than one case per opcode.
- **How a script that continues past its last instruction is handled.** The original relies
  on an exit instruction at the end; the host should reject a script that has none, so that a
  missing exit is caught at load rather than by reading past the buffer.
