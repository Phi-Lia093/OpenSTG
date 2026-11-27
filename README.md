# 東方妖々夢 ～ Perfect Cherry Blossom — Windows port

A Windows build of the cross-platform Touhou 7 port, based on the
[`reallyportable`](https://github.com/some100/th07/tree/reallyportable) branch of the
some100 decompilation. The game runs on SDL3 with desktop OpenGL, and needs none of
Direct3D 8, DirectInput or DirectSound.

## Status

Playable. The title screen, all menus including Options, stage play, and the result
screen work; the game was verified by playing several stages.

## What differs from upstream

Upstream targets Android, iOS, macOS and Emscripten as well as desktop, and gets SDL from
vendored submodules that are built as part of the tree. This repository covers Windows
only:

- `CMakeLists.txt` covers the Windows configuration alone: SDL3, SDL3_image and SDL3_ttf
  are taken from `.tools/` through `find_package`, desktop OpenGL 3.3 replaces OpenGL ES,
  and the assets are read from the working directory.
- `src/graphics/GLWin32.hpp` adds Windows support. `opengl32.dll` only exports OpenGL
  1.1, so the 3.3 entry points the renderer uses are resolved through
  `SDL_GL_GetProcAddress` once the context exists.
- `src/GameWindow.cpp` creates and makes current a GL context explicitly. In SDL3 the
  `SDL_PROP_WINDOW_CREATE_OPENGL_BOOLEAN` property only marks a window as GL-capable; it
  does not attach a context.
- Android, iOS, the vendored SDL submodules and the non-Windows CMake paths are removed.

Everything else is upstream code. The `#ifdef` branches for other platforms are left in
place so future upstream changes stay easy to merge.

## Building

Requirements:

- Visual Studio 2022 with the C++ desktop workload, or any toolchain CMake drives
- CMake 3.20+
- The SDL3, SDL3_image and SDL3_ttf MSVC development packages, 32-bit, in `.tools/`:
  - `.tools/sdl3/SDL3-3.4.20`
  - `.tools/sdlimage/SDL3_image-3.4.8`
  - `.tools/sdlttf/SDL3_ttf-3.2.2`

```bash
python tools/build_portable.py
```

The script configures CMake, builds Release, and assembles `build/run/` with the
executable, the SDL DLLs and the game data. To build by hand:

```bash
cmake -S . -B build -A Win32 \
      -DSDL3_DIR=.tools/sdl3/SDL3-3.4.20/cmake \
      -DSDL3_image_DIR=.tools/sdlimage/SDL3_image-3.4.8/cmake \
      -DSDL3_ttf_DIR=.tools/sdlttf/SDL3_ttf-3.2.2/cmake
cmake --build build --config Release
```

## Assets

The original game data is not distributed here. `build/run/` needs:

| File | Source |
| --- | --- |
| `th07.dat` | Original game data archive (PBG4) |
| `thbgm.dat` | Original game music archive (ZWAV) |
| `msgothic.ttc` | ＭＳ ゴシック, used for text rendering |

`tools/build_portable.py` copies them from `.tools/` and from the system font directory.
The game reads them from the working directory.

## Credits

- [some100/th07](https://github.com/some100/th07) — the source reconstruction this is
  built on, and its `reallyportable` branch
- Team Shanghai Alice — the original game

The upstream license is preserved in [LICENSE](LICENSE).
