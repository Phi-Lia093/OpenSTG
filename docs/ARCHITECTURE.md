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
        GlesGraphics                  OpenGL 3.3 implementation
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
primitives. `GlesGraphics` implements it on OpenGL 3.3 core.

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

On Windows, `opengl32.dll` exports only OpenGL 1.1, so `GLWin32.hpp` declares the 3.3
entry points as function pointers and resolves them through `SDL_GL_GetProcAddress` once
the context exists.

The GL context itself is created explicitly in `GameWindow.cpp`. In SDL3 the
`SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN` property only marks a window as GL-capable; it
does not attach a context.

## Data formats

These are the original formats and the reason for much of the planned work.

### PBG4 archives

`th07.dat` and `thbgm.dat` are PBG4 archives: a header, an index of entries, and each
entry compressed with LZSS. `src/pbg4/` reads them. Every file the game opens goes through
`FileSystem::OpenFile`, which is the single point where archive access happens.

`thbgm.dat` is a different case: it holds audio streams addressed by offset, not
individually compressed files.

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

`th07.cfg` is a packed binary struct: controller mapping, shot type, difficulty, volume
settings and option flags. `Supervisor::LoadConfig` validates it field by field and falls
back to defaults if anything is out of range.

### Text

The game's own text is Shift-JIS, converted to UTF-8 for rendering by `src/sj2utf8/`.
Dialogue and menu strings live inside the data archives.

## What this means for the planned work

The formats above are self-contained behind narrow interfaces, which is what makes the
planned replacements tractable:

- All file access goes through `FileSystem::OpenFile`, so changing the container format
  touches one place.
- Graphics is behind `ZunGraphics`, so resolution handling changes in one implementation.
- Audio is behind `SoundPlayer` and `MidiOutput`.
- The scripting VM is behind `EclManager`, and its instruction set is already enumerated
  in one file.

The parts that are *not* isolated are the two that carry the most risk: the ECL VM, whose
state is embedded in structs shared with gameplay code, and text, which is spread across
every menu and dialogue path.
