# assets

Modernised content, committed to the repository, merged into `th07.data/` at build time.

`tools/extract_assets.py` copies this directory into the build output **first**, then
extracts the original archive over it, skipping anything already present. So `assets/` always
wins, and a file moves from "derived from the archive" to "committed here" simply by
appearing here. No build change is needed, and the archive dependency shrinks on its own as
more of the game is modernised.

## What belongs here

Only content the project owns or has finished reworking:

- Images the project has converted itself, from formats the project controls.
- Configuration and description files the project has authored, such as the TOML that will
  replace the ANM format.
- Anything else rewritten into a form the project defines.

## What does not

Content still taken verbatim from the original game data. That stays derived from the
archive on each build, so the repository carries one copy rather than two:

| Asset | Reason |
| --- | --- |
| `*.anm` | Not yet converted: becomes PNG and TOML |
| `*.ecl` | Not yet converted: becomes Lua |
| `*.dat`, `*.rpy` | Score and replay data, reworked last |
| `*.jpg`, `*.wav`, `*.mid` and similar | Original game data, not distributed here |

## Current state

Empty. The first content arrives with the ANM replacement — converted atlases and their
TOML descriptions — which is documented in
[ANM_DESIGN.md](../docs/ANM_DESIGN.md).
