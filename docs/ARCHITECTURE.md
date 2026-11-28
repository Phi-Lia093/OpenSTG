# Architecture

This describes the code as it stands today: a faithful reconstruction of the original
game, ported to SDL3 and OpenGL, with its original data formats intact. The work planned
on top of it is in [ROADMAP.md](ROADMAP.md).

Two things are worth keeping in mind throughout:

- The engine is a **byte-for-byte reconstruction** of the 2003 binary. Function bodies,
  struct layouts and call sequences deliberately mirror the original, which is why some
  names are terse (`vm1`, `idk`, `unused_04`) and why odd-looking code is often correct.
- The port replaced only the platform boundary. Everything above it — the game logic,
  the resource managers, the scripting — is still the original design.

## Layers

```
                    main.cpp
                       |
                 Supervisor          window, config, per-frame driver
                       |
              +--------+--------+
              |                 |
        GameManager          Chain        update and draw callback lists
              |
   +----------+-----------+-------------+
   |          |           |             |
 Stage    Player     Enemy/Bullet    Gui/Ascii
   |          |           |
   +----------+-----------+
              |
         AnmManager                   sprite and animation manager
              |
         ZunGraphics                  renderer interface
              |
        GLGraphics                    OpenGL 3.3 implementation
```

### Driver

`main.cpp` opens the window and runs the loop. Each frame it walks the update chain and
the draw chain through `Chain`, whose callbacks are registered by whichever objects are
alive. `Supervisor` owns the window, the game configuration and the scene state that
decides which managers exist.

The update chain and the draw chain are separate, so logic and rendering are stepped at
the same rate but through different code paths.

### Resource managers

Each resource type has one manager, and they are global singletons:

| Manager | Owns |
| --- | --- |
| `AnmManager` | Sprites, animations and the ANM scripts that drive them |
| `EclManager` | The enemy scripting VM and its instruction set |
| `Stage` | Stage background geometry and camera |
| `Player`, `EnemyManager`, `BulletManager`, `ItemManager`, `EffectManager` | Gameplay entities |
| `Gui`, `AsciiManager` | HUD and text |
| `SoundPlayer`, `MidiOutput` | Audio |
| `FileSystem` | Every file read |

### Graphics

`ZunGraphics` is an interface that models the Direct3D 8 device the original was written
against: transform matrices, texture stage state, render state, clear, viewport, draw
primitives. `GLGraphics` implements it on OpenGL 3.3 core. Both live in `src/render/`.

The D3D model shows up directly in the shader, which reproduces the fixed-function
pipeline rather than replacing it:

- `u_ColorOpRgb` / `u_ColorOpAlpha` — `D3DTSS_COLOROP` and `D3DTSS_ALPHAOP`
- `u_TexArg` — `D3DTA_DIFFUSE`, `D3DTA_TEXTURE`, `D3DTA_TFACTOR`
- `u_TextureMatrix` — `D3DTS_TEXTURE0`, applied to texture coordinates
- `u_AlphaTest` / `u_AlphaRef`, `u_Fog*` — alpha test and fog
- `u_ScreenSpace` — whether a primitive is in screen pixels or in world space

Two vertex layouts come out of this: screen-space quads for sprites and HUD, and
world-space triangles for the stage background, which go through model, view and
projection matrices.

On Windows, `opengl32.dll` exports only OpenGL 1.1, so `render/GL.hpp` declares the 3.3
entry points as function pointers and `GL.cpp` resolves them through
`SDL_GL_GetProcAddress` once the context exists. The declarations and the OpenGL tokens the
1.1 header lacks are in the same header, since nothing outside the renderer uses them.

The GL context itself is created explicitly in `GameWindow.cpp`. In SDL3 the
`SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN` property only marks a window as GL-capable; it
does not attach a context.

## Assets

There are two ways the game gets data, split by whether the data ever changes.

### Loose files: `th07.data/`

The game reads its data from `th07.data/`, a directory of loose files next to the
executable, through `AssetStore`. Files are addressed by bare name — the game asks for
`data/ascii.anm` and the directory holds `ascii.anm` — which is how the original archive
was laid out.

That directory is assembled from two sources at build time. `assets/` — modernised content
committed to the repository — is copied first. The original archive is then extracted over
it, skipping paths `assets/` already provides. So a file moves from "derived from the
archive" to "committed in `assets/`" by appearing there, with no change to the build, and
the archive dependency shrinks as the modernisation progresses.

`assets/` holds only what the project owns or has finished reworking. Original game data
that is still taken verbatim stays out, so the repository does not carry a second copy of
what the archive already defines.

This replaced the original PBG4 archive. `AssetStore` keeps a container switch so a ZIP can
be read later without callers changing.

`thbgm.dat` is not part of this. It is a single PCM stream addressed by offset, with a
separate `.fmt` describing the track table, and `SoundPlayer` reads it directly.

### The 31 JPEGs must stay JPEG

> **Do not convert `data/end/*.jpg`, `title00.jpg`, `select00.jpg`, `phantasm.jpg`,
> `music.jpg`, `result.jpg`, `staff00.jpg` or `th07logo.jpg` to PNG.**

This was tried and reverted. Re-encoding them changes how the game behaves: the original
relies on details of how these images are decoded and sampled that a PNG does not reproduce,
and the result is a visible fault. The exact mechanism has not been pinned down, which is a
reason for caution rather than for trying again.

If these images are ever to change format, it has to come after the engine's graphics layer
is abstracted — at that point the sampling behaviour is ours to define, rather than something
inherited from the original's assumptions. Until then the JPEGs stay exactly as extracted.

The conversion is also not free in size: re-encoding these 3.8 MB of JPEG as PNG produced
12.2 MB.

### Resources in the executable

Things that never change and would be awkward as loose files are compiled into the
executable's resource section by the generated `th07.rc`: the window icon, a version
record, and the text font. `EmbeddedAssets` locates them with `FindResourceW` and, for the
font, hands the bytes to SDL_ttf through a read-only memory stream, so the 8.8 MB never
touches the file system.

The font has a fallback: if the build embedded none, it is opened from `msgothic.ttc`
beside the executable.

## Data formats

Below the asset layer, these are the original formats and the reason for much of the
planned work.

### ANM

Sprites and animation are described by ANM files. An ANM has an embedded texture and a
list of scripts, each a run of instructions that set sprite, position, scale, rotation,
colours and the like. `AnmManager` interprets these at runtime.

Sprites reference a texture by index and a rectangle within it, so a texture and the
sprites that use it are loaded together.

### ECL

Enemy behaviour is written in ECL, a byte-code language run by `EclManager`. The
instruction set is implemented in `EnemyEclInstr.cpp`, with the interpreter and its
arithmetic in `EclManager.cpp`. Stage scripts and boss scripts are both ECL.

This is the largest single subsystem and the one where the original design is most
visible: instructions are fixed-size records, and the VM state lives in structs whose
layout matches the original binary.

### Configuration

`th07.toml` holds the settings, read and written by `ConfigFile`. The original stored them
as a 56-byte binary struct and cast the file straight into it, so any wrong byte replaced
every setting with a default.

Now each field is named, values that name a choice use words (`default_difficulty =
"normal"`) rather than numbers, and an out-of-range value is reported and clamped instead of
discarding the file. A field that is absent keeps its default, so a partial file is valid
and deleting the file resets everything.

The in-memory `GameConfiguration` is unchanged: only the serialization layer differs, which
keeps the menus and every `cfg.*` reader untouched.

`Toml.cpp` is a small reader for the subset of TOML the file uses — tables, comments,
strings, integers, floats, booleans and flat arrays. Arrays of tables, inline tables,
datetimes and multi-line strings are reported as errors rather than misread. The full TOML
specification is far larger than a settings file needs, and a complete implementation would
be a dependency for a fraction of its features.

`tools/test_config.py` builds `Toml.cpp` and `ConfigFile.cpp` without SDL and checks
parsing, error reporting, round-tripping and clamping.

### Text

The game's own text is Shift-JIS, converted to UTF-8 for rendering by `src/support/sj2utf8/`.
Dialogue and menu strings live in the data files under `th07.data/`.

## What this means for the planned work

The formats above are self-contained behind narrow interfaces, which is what makes the
planned replacements tractable:

- All file access goes through `AssetStore`, so changing the container touches one place.
  This has already been exercised: the original archive was replaced by a directory
  without any caller changing.
- Graphics is behind `ZunGraphics`, so resolution handling changes in one implementation.
- Audio is behind `SoundPlayer` and `MidiOutput`.
- The scripting VM is behind `EclManager`, and its instruction set is already enumerated
  in one file.

The parts that are *not* isolated are the two that carry the most risk: the ECL VM, whose
state is embedded in structs shared with gameplay code, and text, which is spread across
every menu and dialogue path.
