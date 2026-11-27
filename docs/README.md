# OpenSTG

A modern, cross-platform rebuild of 東方妖々夢 ～ Perfect Cherry Blossom (TH07).

## Origin and current state

The rendering port comes from
[some100/th07](https://github.com/some100/th07)'s `reallyportable` branch, which already
targets SDL3 and OpenGL. That port is complete and verified: the title screen, every
menu, stage play and the result screen all work, with the correct Japanese text and
without the startup flash the earlier hand-written port suffered from.

The project therefore starts from a foundation that is known good, and the work ahead is
about replacing the game's original data formats and subsystems with modern equivalents,
not about making the game run.

The build targets Windows and uses desktop OpenGL 3.3. See
[ARCHITECTURE.md](ARCHITECTURE.md) for how the code is laid out today and
[ROADMAP.md](ROADMAP.md) for the order the work is planned in.

## Building

Requirements:

- Visual Studio 2022 with the C++ desktop workload
- CMake 3.20+
- SDL3, SDL3_image and SDL3_ttf MSVC development packages, 32-bit, under `.tools/`

```bash
python tools/build_portable.py
```

This configures CMake, builds Release, and assembles `build/run/` with the executable,
the SDL DLLs and the game data.

## Assets

The original game data is not distributed here. `build/run/` needs `th07.dat`,
`thbgm.dat` and `msgothic.ttc`; the build script copies them from `.tools/` and from the
system font directory.

## Repository layout

| Path | Contents |
| --- | --- |
| `src/` | Game code: the engine, the managers for each resource type, and the graphics abstraction |
| `src/graphics/` | The renderer: `ZunGraphics` interface, its OpenGL implementation, and the Windows GL loader |
| `src/pbg4/` | The original archive reader and its LZSS decompressor |
| `src/sj2utf8/` | Shift-JIS to UTF-8 conversion for the game's own text tables |
| `tools/` | Build scripts |
| `docs/` | This documentation |

## Licensing

The reconstruction is by some100 and its contributors; its license is preserved in
[LICENSE](../LICENSE). The original game is the work of Team Shanghai Alice. No original
game data is included.
