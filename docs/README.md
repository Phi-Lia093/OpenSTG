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
- The original `th07.dat` and `thbgm.dat` in `.tools/`, plus thtk under `.tools/thtk-bin-*/`

SDL is linked statically, so the executable needs no SDL DLLs beside it — only Windows
system DLLs. The static archives are built from source once:

```bash
python tools/build_sdl_static.py   # SDL3, SDL3_image, SDL3_ttf into .tools/static/
python tools/build_portable.py     # configure, build, assemble build/run/
```

`build_portable.py` configures CMake, builds Release, and assembles `build/run/` with the
executable and the game data. `build_sdl_static.py` reads the SDL repositories from
`D:/SDL`, `D:/SDL_image` and `D:/SDL_ttf`.

The executable carries its own icon, a version resource, and the text font, so it needs no
files beside it except the game data below.

## Assets

The original game data is not distributed here. `build/run/` gets:

| Path | Contents |
| --- | --- |
| `th07.data/` | The game's data files, one per file, extracted from `th07.dat` |
| `thbgm.dat` | The music stream, copied unchanged |

`tools/extract_assets.py` produces `th07.data/` with thdat from thtk; `build_portable.py`
runs it automatically when the directory is missing. Pass `--force` to re-extract.

The text font, ＭＳ ゴシック (`msgothic.ttc`), is embedded in the executable's resource
section at build time. It is read from `.tools/` or from the system font directory; a copy
beside the executable is used as a fallback at runtime if the build had none.

## Repository layout

| Path | Contents |
| --- | --- |
| `src/` | Game code: the engine, the managers for each resource type, and the graphics abstraction |
| `src/AssetStore.cpp` | Reads the game's data files from `th07.data/` |
| `src/ConfigFile.cpp` | Reads and writes `th07.toml` |
| `src/Toml.cpp` | A reader for the subset of TOML the settings file uses |
| `src/EmbeddedAssets.cpp` | Reads resources embedded in the executable; currently the text font |
| `src/graphics/` | The renderer: `ZunGraphics` interface, its OpenGL implementation, and the Windows GL loader |
| `src/compress/` | LZSS, used by replay and savedata files |
| `src/sj2utf8/` | Shift-JIS to UTF-8 conversion for the game's own text tables |
| `tools/` | Build scripts, the resource template and the icon |
| `docs/` | This documentation |

## Licensing

The reconstruction is by some100 and its contributors; its license is preserved in
[LICENSE](../LICENSE). The original game is the work of Team Shanghai Alice. No original
game data is included.
