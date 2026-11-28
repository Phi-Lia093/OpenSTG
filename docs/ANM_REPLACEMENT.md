# ANM replacement

The plan for replacing the ANM system: the binary animation format becomes PNG images plus
TOML descriptions, and the JPEG artwork becomes PNG. This document records what the current
format holds, what the conversion has to preserve, and the decisions that follow from it.

Nothing here is implemented yet.

## What ANM is

`th07.data/` holds **63 ANM files**. Each is one self-contained container:

| Part | Contents |
| --- | --- |
| Header | Format version, texture metadata |
| Texture | One image, embedded in THTX format, or a reference by file name |
| Sprites | A list of rectangles into that texture |
| Scripts | Runs of instructions that drive a sprite over time |

An ANM is therefore an atlas, the rectangles cut from it, and the animation that picks among
them. Everything a sprite needs lives in one file, which is why the format survived: it is
compact and self-contained.

The parts that make it awkward are the same parts: the image is a custom packed format that
only this game reads, the rectangles are raw pixel offsets with no names, and the scripts are
byte-code whose meaning lives in the interpreter's switch statement.

## The pieces, measured

### Textures

Textures come in three of the game's five THTX formats:

| THTX format | Meaning | Files |
| --- | --- | --- |
| 1 | ARGB8888 | 49 |
| 3 | RGB565 | 10 |
| 5 | ARGB4444 | 39 |

Formats 2 (ARGB1555) and 4 (RGB888) are not used. All 63 textures are embedded; none of them
references an external file.

Outside the ANMs, `th07.data/` also holds **31 JPEGs**: the ending artwork and the title
image.

### Sprites

A sprite is a rectangle in the texture plus a grid:

```
width*height + x + y
```

with the width and height divided by a `cols`/`rows` count, so one record can describe a
sheet of evenly spaced frames. The rectangle is in texture pixels; `AnmManager::LoadSprite`
divides it by the texture size to get the UVs the renderer uses, so the pixel form is the
authoritative one.

Sprites are identified by index. The scripts select them by writing an index into the VM.

### Instructions

The interpreter recognises **82 opcodes** (numbered -1 to 81). The shipped data uses **47**
of them, across **9,833 instructions** in total.

| Opcode | Name | Uses | Arguments |
| --- | --- | --- | --- |
| 3 | `ANM_SET_ACTIVE_SPRITE` | 2038 | 1 |
| 21 | `ANM_INTERRUPT_LABEL` | 1159 | 1 |
| 8 | `ANM_SET_ALPHA` | 530 | 1 |
| 15 | `ANM_FADE` | 465 | 2 |
| 34 | `ANM_INTERP_ALPHA` | 468 | 3 |
| 23 | `ANM_STOP_HIDE` | 431 | 0 |
| 6 | `ANM_SET_TRANSLATION` | 417 | 3 |
| 1 | `ANM_EXIT_HIDE2` | 401 | 0 |
| 20 | `ANM_STOP` | 397 | 0 |
| 4 | `ANM_JUMP` | 357 | 2 |
| … | | | |

The full table is at the end of this document.

Each instruction starts with a **time**, and the VM advances through those whose time has
arrived. Time is a timeline position, not a delay, which is why a jump can move it backwards
and restart an animation.

Arguments are raw slots with no type tag. Whether a slot is an integer or a float is decided
by the opcode, and by the argument's position: `ANM_INTERP_POS` takes floats, `ANM_SET_ALPHA`
takes an integer. The `AnyArg` union in the reconstruction reflects this.

### Control flow

Only two instructions change the instruction pointer:

- **`ANM_JUMP`** (4, 357 uses) — unconditional, to a byte offset, setting a new time.
- **`ANM_DEC_JUMP`** (5, 36 uses) — decrement a VM integer, jump if still positive.

That is the entire control flow. There are no calls, no computed jumps, no indirect target.
Everything else only changes VM state.

This matters for the design: **the animation language is not Turing-complete in any way the
data uses.** It is a flat timeline with labels, a `goto`, and a counted loop.

## Where the data sits

| Item | Count |
| --- | --- |
| ANM files | 63 |
| Embedded textures | 63 |
| External JPEGs | 31 |
| Instructions | 9,833 |
| Distinct opcodes used | 47 (of 82 defined) |
| Largest file | `title01.anm`, 2,189 instructions |

## Proposed replacement

### Layout

```
th07.data/
  anim/
    title01/
      texture.png
      anim.toml
    ascii/
      texture.png
      anim.toml
    …
  image/
    end00.png
    end01.png
    …
```

One directory per ANM, holding the decoded texture and its description. The ANM file
extension disappears entirely; the loader looks for `anim/<name>/` instead of `<name>.anm`.

### What the TOML holds

```toml
# anim/ascii/anim.toml
version = 2

[texture]
# Written by the exporter; the loader only needs the path.
image = "texture.png"

[[sprite]]
name = "blank"
rect = [0, 0, 8, 8]

[[sprite]]
name = "glyph_0"
rect = [0, 0, 8, 8]
cols = 1
rows = 1

[[script]]
name = "default"

[[script.frame]]
time = 0
sprite = "glyph_0"
alpha = 255

[[script.frame]]
time = 30
scale = [1.5, 1.5]
```

Two decisions are open and both are in the questions below: whether scripts become a list of
frames or a list of instructions, and whether the names are generated or derived.

### Images

Every texture is decoded to **PNG**. The conversion is mechanical: THTX formats to RGBA8,
then PNG.

**Alpha.** The source formats have binary or 4-bit alpha. Where an image is fully opaque —
`RGB565`, or `ARGB4444` whose alpha is all ones — the exporter writes an opaque PNG, with no
alpha channel at all if that keeps it smaller. Where it has real transparency, the values are
preserved exactly, `0` and `15` mapping to `0` and `255`.

The **31 JPEGs** become PNG with alpha forced to fully opaque. JPEG has no alpha, so nothing
is lost.

### Names

The current format addresses sprites by index and scripts by index. Both become names in the
TOML. Two options:

- **Generated**: `sprite_0`, `script_0`, in order. Mechanical and safe, but no more readable
  than the numbers.
- **Derived**: from the context that uses them. `AnmIdx.hpp` already names the scripts the
  game refers to (`ANM_SCRIPT_PLAYER_MOVING_LEFT` and so on), so those names can be carried
  over; sprites could be named after the script that first selects them.

A mixed approach seems right: name what the game already names, number the rest.

## Questions to settle before implementing

1. **Scripts as frames or as instructions?**
   The data uses 47 opcodes, and the two jumps would become `loop`/`goto` in the TOML. That
   keeps every behaviour expressible without a second language, at the cost of a document
   that still reads like byte-code with names. The alternative — a frame list with implicit
   time and no jumps — is far more readable but cannot express the 393 jump uses without
   restructuring the animations, which risks changing timing subtly.

2. **Does anything need a script language?**
   Given that the only control flow is one `goto` and one counted loop, TOML can express the
   whole instruction set without exception. Lua would add a dependency and a runtime to solve
   a problem the data does not have.

3. **Interrupts.**
   `ANM_INTERRUPT_LABEL` is used 1,159 times, the second most common instruction. The game
   calls `SetInterrupt` to jump to a labelled point. These labels need to survive as names.

4. **How much verification?**
   The exact-match property of the reconstruction means the current behaviour is known.
   A frame-by-frame comparison of old and new — same VM, same inputs — would catch a
   conversion error, but it needs the old format readable until the new one is proven.

## Suggested order

1. Write the exporter and the PNG conversion, and check that every texture decodes to the
   same pixels the loader currently produces.
2. Define the TOML schema and write the reader, with the existing interpreter still present
   for comparison.
3. Convert one small ANM, compare behaviour, then a whole category (menus), then everything.
4. Remove the ANM reader and the THTX decoder.
5. Convert the JPEGs.

Steps 3 and 4 are the risky ones: until the last file is converted, both loaders exist, and
that is deliberate.

---

## Appendix: opcodes used by the shipped data

| Opcode | Uses | Args |
| --- | ---: | ---: |
| 0 | 95 | 0 |
| 1 | 401 | 0 |
| 2 | 248 | 0 |
| 3 | 2038 | 1 |
| 4 | 357 | 2 |
| 5 | 36 | 3 |
| 6 | 417 | 3 |
| 7 | 188 | 2 |
| 8 | 530 | 1 |
| 9 | 82 | 1 |
| 10 | 70 | 0 |
| 12 | 158 | 3 |
| 13 | 193 | 3 |
| 14 | 49 | 2 |
| 15 | 465 | 2 |
| 16 | 105 | 1 |
| 17 | 220 | 4 |
| 18 | 174 | 4 |
| 19 | 36 | 4 |
| 20 | 397 | 0 |
| 21 | 1159 | 1 |
| 22 | 262 | 0 |
| 23 | 431 | 0 |
| 24 | 119 | 1 |
| 25 | 55 | 1 |
| 26 | 1 | 1 |
| 27 | 28 | 1 |
| 28 | 2 | 1 |
| 29 | 4 | 3 |
| 30 | 233 | 1 |
| 31 | 109 | 1 |
| 32 | 277 | 5 |
| 33 | 84 | 3 |
| 34 | 468 | 3 |
| 35 | 73 | 5 |
| 36 | 108 | 4 |
| 37 | 26 | 2 |
| 38 | 2 | 2 |
| 42 | 1 | 2 |
| 50 | 38 | 3 |
| 52 | 27 | 3 |
| 55 | 12 | 3 |
| 59 | 2 | 2 |
| 60 | 28 | 2 |
| 69 | 5 | 4 |
| 79 | 19 | 1 |
| 80 | 1 | 1 |

Opcodes defined but never used (36): -1, 11, 39–41, 43–49, 51, 53, 54, 56–58, 61–68, 70–78,
81.
