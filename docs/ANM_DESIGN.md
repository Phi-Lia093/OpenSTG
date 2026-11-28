# ANM replacement — design

The binary animation format becomes PNG images plus a TOML description written in terms of
scenes, tracks and keyframes rather than byte-code. This document is the design; the
investigation that led to it is [ANM_REPLACEMENT.md](ANM_REPLACEMENT.md).

The goal is an engine that a person can author content for, not only a rebuild of one game.
The format has to read like an animation, not like an assembler listing.

## What the data turned out to be

Every script in the shipped data was classified by the shape of its instruction stream:

| Shape | Scripts | Share | Structure |
| --- | ---: | ---: | --- |
| Linear | 660 | 67% | Keyframes on a timeline, no jumps |
| Sprite cycle | 214 | 22% | Equally spaced frames, one jump back to the start |
| Goto | 92 | 9% | A linear animation, then a jump over a second one |
| Counted loop | 18 | 2% | A counter, a per-frame effect, a loop back |

**984 scripts, and every one of them is one of four shapes.** That is the finding that makes
a declarative format possible: the byte-code is not used as a general program. It is a
timeline with three embellishments.

The three embellishments, concretely:

```asm
; sprite cycle (player00.anm script 0)
SET_SPRITE 0    time 0
SET_SPRITE 1    time 8
SET_SPRITE 2    time 16
SET_SPRITE 3    time 24
JUMP         -> 0, time 0        ; restart

; counted loop (face_mr00.anm script 4)
MOV         v10008, 100          ; counter
SCROLL_Y    -0.008333334         ; per-frame drift
DEC_JUMP    v10008, back, 0      ; decrement, repeat while positive
INTERP_ALPHA 50, 0, 0            ; fade out afterwards

; goto (ascii.anm script 5)
INTERP_SCALE2 30, 4, 1.2, 1.2
INTERP_SCALE2 30, 1, 0.8, 0.8
JUMP          -> end, time 0     ; skip a second animation
```

## The format

### Files

```
th07.data/
  anim/
    player00/
      texture.png
      anim.toml
```

One directory per animation, holding the decoded atlas and its description. The ANM
extension disappears.

### Structure of `anim.toml`

An animation file has a texture, a set of sprites, and a set of scripts. A script is a
timeline of keyframes, optionally containing bounded repeats.

```toml
version = 1

[texture]
image = "texture.png"
width = 256              # written by the exporter, informational
height = 256

# Sprites are named regions of the texture, in texture pixels.
[[sprite]]
name = "idle_0"
rect = [0, 0, 32, 48]

[[sprite]]
name = "idle_1"
rect = [32, 0, 32, 48]

# A sprite may also be a grid slice, which several atlases use.
[[sprite]]
name = "glyph_A"
rect = [0, 0, 64, 64]
cols = 8
rows = 8

# ---------------------------------------------------------------------------

[[script]]
name = "idle"
loop = true                      # the sprite cycle: restart at the end

[[script.key]]
frame = 0
sprite = "idle_0"

[[script.key]]
frame = 8
sprite = "idle_1"
```

The field that carries most of the meaning is `frame`: a timeline position, not a delay.
That is what the original `time` field is, and keeping it means a long animation reads as a
list of positions rather than a list of waits.

### Keyframes

A keyframe sets as many properties as it needs. The exporter writes only what the original
instruction wrote, so a round trip is exact.

```toml
[[script.key]]
frame = 60
sprite = "blink_2"
alpha = 255
color = "#ff8040"
flip = "x"
blend = "add"

# A movement along a path, taking a number of frames.
[[script.key]]
frame = 60
to = [144, 96]
duration = 150
easing = "ease_out"        # linear | ease_in | ease_out | ease_in_out

# Interpolation of a scalar or a vector over a span.
[[script.key]]
frame = 0
alpha = { from = 0, to = 255, duration = 120 }

[[script.key]]
frame = 0
scale = { from = [1.0, 1.0], to = [1.2, 1.2], duration = 30 }
```

`to`, `from/to/duration` and `easing` map onto `ANM_POS_TIME_LINEAR`, `ANM_POS_TIME_DECEL`,
`ANM_POS_TIME_ACCEL`, `ANM_INTERP_POS`, `ANM_INTERP_ALPHA`, `ANM_INTERP_SCALE` and their
relatives. The exporter chooses the spelling from the opcode; the runtime picks the opcode
back from the spelling.

### Repeats

The counted loop becomes a block:

```toml
[[script.repeat]]
count = 100
duration = 1               # frames per iteration, from the DEC_JUMP time

[[script.repeat.key]]
scroll_y = -0.008333334
```

The per-frame effect in the original is "run the loop body once per frame", which is why the
body's instructions all carry the same time. A `repeat` with `duration = 1` says the same
thing.

A repeat can also carry a whole sequence of keys, for the case where the body is more than
one instruction.

### Sections

92 scripts play one animation and jump over a second. In the original these are two runs of
instructions in one script, where the second is only reached by an interrupt.

```toml
# The script that runs by default.
[[script]]
name = "appear"
[[script.key]]
frame = 0
sprite = "appear_0"

# Reached by SetInterrupt(1); the default path ends before it.
[[script]]
name = "appear_alt"
interrupt = 1
```

This removes the jump-over entirely: an alternate path becomes a named script addressed by
the interrupt number the game already uses. `AnmIdx.hpp` names many of these
(`ANM_SCRIPT_PLAYER_MOVING_LEFT` and so on), and those names carry over.

### Interrupt labels inside a script

`ANM_INTERRUPT_LABEL` appears 1,159 times, second only to setting a sprite. In the data it
marks a restart point: the game calls `SetInterrupt(n)` and the VM jumps there.

A named keyframe is the natural equivalent:

```toml
[[script.key]]
name = "loop_start"
frame = 0
sprite = "spin_0"
```

and a `loop` on the script restarts at that key rather than at the beginning.

## The escape hatch

There is one, and the design's success is measured by how little of it is needed:

```toml
[[script.raw]]
frame = 60
opcode = "set_auto_rotate"
args = [1]
```

The converter emits it only when a script's shape matches none of the four patterns. The
target is **zero uses across all 63 files**. If the conversion produces any, the reason is
recorded per occurrence, and either the pattern table grows or the escape is judged necessary.

This is the check that the abstraction is honest: a format that needs the escape everywhere
has not abstracted anything.

## Converter

`tools/convert_anm.py`, driven by thtk's `thanm` for reading the originals:

1. **Extract** the texture with `thanm -x`, decode THTX to RGBA8, write PNG.
2. **Read** the sprite table and the scripts through `thanm -l`.
3. **Classify** each script by the four shapes above.
4. **Emit** keyframes, repeats and named scripts; fall back to `script.raw` when a script
   matches nothing.
5. **Record** per-file statistics so the escape-hatch count is visible immediately.

The converter is not part of the build. It runs once, its output is committed, and the
original ANMs are no longer needed.

## Runtime

`AnmManager` keeps its VM, its timers and its interrupt handling. What changes is where the
data comes from:

| Today | After |
| --- | --- |
| `AnmRawInstr` array, walked by opcode | Keyframe list, walked by `frame` |
| Sprite index | Named sprite, resolved to an index at load |
| Script index | Named script, resolved at load |
| `SetInterrupt(n)` jumps to a byte offset | Jumps to the keyframe carrying that number |

The VM's *state* — position, scale, alpha, timers, easing — is unchanged, because the
keyframes set the same fields the instructions set. The interpreter becomes a reader of a
timeline rather than a dispatcher over 82 opcodes.

Two things must stay identical or the game changes:

- **Timing.** A keyframe at `frame = 8` must fire when `time == 8` exactly as an instruction
  with `time = 8` does, including when a jump rewinds the clock.
- **Order within a frame.** When several instructions share a time, they run in order. A
  keyframe that sets three properties must apply them in the same order.

## Verification

The reconstruction matches the original byte for byte, so the current behaviour is the
reference. To prove the conversion preserves it:

1. **Texture comparison** — decode each PNG back to RGBA8 and compare against what the THTX
   loader produces. This should be exact and needs no judgement.
2. **Script comparison** — for each script, load the old and new forms into the same VM, step
   both through the same number of frames with the same interrupts, and compare every field
   of the VM state each frame. This catches timing and ordering mistakes, which are the ones
   that matter.
3. **Coverage report** — the count of `script.raw` uses, which must be zero.

Both loaders exist until step 3 passes for every file. That duplication is deliberate.

## Unresolved

- **Naming the sprites.** The exporter has to name them, and names generated from an index
  (`sprite_0`) are honest but no better than numbers. Naming a sprite after the script that
  first selects it is a heuristic that will sometimes be wrong. The proposal is to carry over
  the names `AnmIdx.hpp` already gives the scripts, and number sprites within a script's
  scope.
- **Whether `loop = true` covers every cycle.** The 214 sprite cycles all end with a jump to
  the first instruction, but whether some of them also rewind the clock in a different way
  needs checking per file.
- **JPEG conversion: not to be attempted.** The 31 JPEGs stay as they are. Converting them
  was tried and reverted: re-encoding changes behaviour in a way that is visible in game,
  because the original depends on how those images are decoded and sampled. See
  [ARCHITECTURE.md](ARCHITECTURE.md#the-31-jpegs-must-stay-jpeg). It is out of scope for this
  work and belongs after the graphics layer is abstracted.
