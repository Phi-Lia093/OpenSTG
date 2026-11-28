# Roadmap

The order the planned work is done in, with what each stage needs and how it can be
checked. Each stage is meant to leave the game playable.

The sequence is chosen so that each stage depends only on things already done, and so the
riskiest work — the ECL VM — happens once the surrounding systems are modern and easier to
test against.

## Order at a glance

| Stage | Work | Risk | Status |
| --- | --- | --- | --- |
| 1 | Data container: PBG4 → loose files | Low | **Done** |
| 2 | i18n: text extracted, UTF-8 throughout | Medium | Planned |
| 3 | Resolution independence | Medium | Planned |
| 4 | Audio: new subsystem for SE and BGM | Medium | Planned |
| 5 | Configuration: binary cfg → TOML | Low | **Done** |
| 6 | ECL: byte-code → Lua | High | Planned |

Stages 1, 2 and 3 are ordered first because the later work is easier when assets are
plain files, strings are externalized, and the coordinate system is not fixed at 640×480.

---

## Stage 1 — Data container — Done

**Goal.** Read the game's assets from something ordinary instead of PBG4.

**Result.** Assets are loose files in `th07.data/` next to the executable. `AssetStore`
replaces the archive: it probes for `th07.data/` and reads files from it by bare name, which
is how the archive addressed them. The PBG4 reader, its file abstraction and the LZSS path
that unpacked the archive are gone; LZSS itself remains under `src/support/` because replay
and savedata files still use it.

`tools/extract_assets.py` produces the directory from the original archive with thdat, and
`tools/build_portable.py` runs it as part of assembling `build/run/`.

**Still open.** `th07.zip` is not read yet. `AssetStore` has the container switch in place,
so adding a ZIP reader is a new case rather than a change to callers.

Alongside the container change, the text font moved out of the install directory and into
the executable's resource section, together with the icon and a version record. Everything
the game needs beside it is now the data: `th07.data/` and `thbgm.dat`.

---

## Stage 2 — Internationalization

**Goal.** All user-visible text comes from external, translatable resources.

**Why here.** Text handling is spread through every menu and dialogue path, so it is
easier to do while the code is otherwise unchanged and the game is known to be playable —
a text regression is obvious.

**What exists.** Strings are Shift-JIS inside the data archives, converted by
`src/sj2utf8/`. Some are in ECL dialogue instructions; some are in menu code.

**Approach.**

1. Inventory every user-visible string and where it comes from. The dialogue and menu
   text embedded in data files is the bulk of it; hard-coded strings in the source are the
   tail.
2. Extract them to a keyed table — a message id per string, with the Japanese as the base
   entry.
3. Replace the Shift-JIS conversion with a lookup through that table, keyed by message id.
4. Let the table carry additional languages, selected at runtime.

**Risks.**

- Text that lives in ECL byte-code has to be handled by whatever the ECL stage produces,
  so some of this work may need to wait for stage 6. The parts in data files and menus can
  be done now.
- Japanese line breaking and spacing are not the same as Latin. The existing renderer
  measures text per character; that assumption needs checking against other scripts.
- The font is currently `msgothic.ttc`, chosen for Japanese coverage. Other languages need
  either a font with wider coverage or per-language font selection.

**Done when** no user-visible string is read from a Shift-JIS source, and a second
language can be added by editing a data file alone.

---

## Stage 3 — Resolution independence

**Goal.** The game renders correctly at any window size, including widescreen.

**Why here.** It touches the renderer and the coordinate assumptions in gameplay code, and
having it done makes later visual work (new UI layouts, translations with different text
widths) much easier.

**What exists.** The design assumes 640×480 throughout. `AsciiManager` sets the viewport
to those dimensions, the shader maps screen pixels into clip space using the viewport, and
various gameplay constants are in 640×480 units.

**Approach.**

- Separate the *logical* coordinate space from the *window* size. The playfield keeps its
  384×448 proportion; the surrounding area scales.
- Check every place that hard-codes 640 or 480 and decide whether it means a logical unit
  or a window pixel.
- Decide what happens outside the original 4:3 area in a widescreen window: letterbox,
  extended background, or see-through.

**Risks.** The faithful-reconstruction code is full of literals that are correct at
640×480. Distinguishing "this is a screen coordinate" from "this is a design constant" is
the real work, and getting it wrong shows up as subtle misalignment rather than obvious
breakage.

**Done when** the game runs correctly at several window sizes, including a widescreen one,
with the playfield proportion preserved.

---

## Stage 4 — Audio

**Goal.** Replace the audio subsystem with a modern one that is not tied to a vendored
single-header library.

**Why here.** It is independent of the stages before it, so it can be done any time; it
sits here because the BGM archive work is naturally tied to it.

**What exists.** `SoundPlayer` handles sound effects and BGM through `miniaudio`;
`MidiOutput` handles the original's MIDI output.

**Scope.**

- Sound effects: short samples, several playing at once, simple triggering. Straightforward.
- BGM: the original plays from a stream archive with a track table. This is where the
  `thbgm.dat` work lands.
- MIDI: the original can drive an external MIDI device. This is a Windows-era feature; the
  decision is whether to keep it, replace it with rendered audio, or drop it.

**Approach.** Define the audio interface the game actually needs — trigger a sound, play a
track, adjust volumes — and implement it on SDL3's audio API, which is already a
dependency. SDL3 audio is a plain pull/push stream with format conversion, which is enough
for both sample playback and streaming a track.

**Done when** sound effects and BGM both work through the new path, with no `miniaudio`
dependency.

---

## Stage 5 — Configuration — Done

**Goal.** Replace the packed binary configuration with a readable file.

**Result.** `th07.toml` replaces `th07.cfg`. Every field is named and grouped into
`[input]`, `[game]`, `[graphics]`, `[audio]` and `[display]`; choices use words rather than
numbers.

Three behaviours the old format lacked: an out-of-range value is reported and clamped
instead of replacing every setting with defaults, a field absent from the file keeps its
default so a partial file is valid, and a parse error names the line.

The in-memory `GameConfiguration` is untouched, so no reader or menu code changed.

`Toml.cpp` is a reader for the subset of TOML the file uses, since a full implementation is
a large dependency for a settings file. `tools/test_config.py` covers parsing, error
reporting, round-tripping and clamping without needing SDL.

This was done before stage 2, out of order: the configuration is self-contained and does not
depend on the text work.

---

## Stage 6 — ECL to Lua

**Goal.** Replace the ECL byte-code VM with Lua scripts.

**Why last.** It is the largest and riskiest change. The VM's state is embedded in structs
shared with gameplay code, its instruction set is large, and the game data *is* ECL — there
is no ECL source to convert, only the compiled byte-code inside the archives. Doing it
after the surrounding systems are modern means the comparison is easier to make and the
fallback is clearer.

**What exists.** `EclManager` interprets fixed-size instruction records;
`EnemyEclInstr.cpp` implements the instruction set. Both together are the VM.

**Approach.**

1. Define a Lua host API that exposes the same operations the instructions perform —
   move, shoot, set variables, spawn effects, wait. This is a one-to-one mapping of the
   existing instruction implementations, not a redesign.
2. Write a decompiler that reads the compiled ECL in the archives and emits Lua using that
   API. The decompiler can be validated by running both the original VM and the generated
   Lua on the same stage and comparing observable state frame by frame.
3. Keep the original VM available behind a flag until the Lua path is proven, so a
   regression can be diagnosed by switching back.

**Risks.** This is where a mistake is both likely and expensive: subtle differences in
timing, rounding or evaluation order change how a boss behaves, and that is hard to see
without playing. The frame-by-frame comparison in step 2 is the main safeguard, and it
needs the original VM left intact to compare against.

**Done when** every stage runs from Lua with behaviour indistinguishable from the original
byte-code path.

---

## Hard constraints

- **The JPEGs stay JPEG.** Converting the 31 images in `data/end/` and the title art to PNG
  was tried and reverted: it changes behaviour in game, because the original depends on how
  those images are decoded and sampled. It belongs after the graphics layer is abstracted,
  and is not part of the ANM work. See
  [ARCHITECTURE.md](ARCHITECTURE.md#the-31-jpegs-must-stay-jpeg).

## Removed: mobile targets

Done before the stages above, because dead platform code is a hazard for everything that
follows.

The upstream port reached Android, iOS, macOS and the web, and carried the machinery for
each: an OpenGL ES path alongside desktop OpenGL, a touch-screen control scheme, and
per-platform file locations. This build is Windows only, so that code was removed rather
than left to rot:

- `Touch.cpp` and `Touch.hpp`, the touch control scheme, along with the finger events in
  `main.cpp` and the drag-movement handling in `Player`. Movement now comes from the
  keyboard or a gamepad only.
- The iOS and Emscripten branches in the renderer, the window setup and `CheckVSync`.
- The OpenGL ES shader header and the `glClearDepthf` alternates: the shaders are GLSL
  3.30 core unconditionally.

Choosing between targets at compile time is what let these paths stay half-tested. With one
target there is one path.

---

## Sources reorganised

The sources were flat in `src/`, 72 files in one directory, which made it impossible to see
where a new file belonged. They are now grouped by role — `core`, `game`, `script`, `ui`,
`audio`, `data`, `render`, `support` — with each header next to its implementation.

Two renames came out of the same pass:

- `dxutil.hpp` was a copy of a Microsoft DirectX SDK header, but nothing in it had ever
  touched DirectX: it held four pointer-cleanup macros. It is `core/memory.hpp` now.
- `Gles.cpp` and `GLWin32.hpp` were three files for one renderer, split because the port
  once supported OpenGL ES. They are `render/GL.hpp` and `render/GL.cpp`, and the class is
  `GLGraphics`.

Includes are written as paths from `src/`, so one include directory in the build still
covers everything.

---

## Working notes

- **Keep the game playable.** Each stage should end with a build that runs. The value of
  the current baseline is that it works; losing that makes every later change harder to
  judge.
- **Keep the original alongside.** For the VM, the audio path and the texture pipeline,
  the old implementation is the reference. Removing it before the replacement is proven
  removes the ability to compare.
- **The reconstruction is not a normal codebase.** Much of it looks strange because it
  matches a 2003 binary. Before "cleaning up" a piece of code, check whether its shape is
  deliberate.
