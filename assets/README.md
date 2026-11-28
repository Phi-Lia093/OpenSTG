# assets

Content the project owns, committed to the repository and read in preference to the game's
original data.

The asset store tries `assets/` before `th07.data/` for every name, and falls back when it is not
there. So a file moves from "taken from the archive" to "committed here" simply by appearing here:
no build step merges the two, and the archive dependency shrinks on its own as more of the game is
modernised.

## What belongs here

Content the project has converted into a form it defines:

| Path | What it is |
| --- | --- |
| `anm/` | Every animation: the Lua description, the scripts as Lua functions, and the textures as PNG |

## What does not

Content still taken from the original game data, which stays in `th07.data/`:

| Asset | Reason |
| --- | --- |
| `*.ecl` | Not yet converted |
| `*.std`, `*.sht` | Not yet converted |
| `*.dat`, `*.rpy` | Score and replay data |
| `*.jpg`, `*.wav`, `*.mid` and similar | Original game data, not distributed here |

## anm/

63 animations, 101 entries, 984 scripts, 98 textures.

Each animation is a directory:

    anm/<name>/
        animation.lua     description, sprites and every script
        texture_0.png     the texture for entry 0
        texture_1.png     the texture for entry 1
        ...

`animation.lua` is source: it can be read, edited and reasoned about without anything converting
it first. The format is documented in [ANM_TEXT_DESIGN.md](../docs/ANM_TEXT_DESIGN.md).

There is no binary form beside it. `tools/build_anm_data.py` converts the original `*.anm` files
into this directory once, when the game's data is first extracted; after that the Lua is the
source and the binary is not read.
