#!/usr/bin/env python3
"""Rebuild assets/anm/ from the original animations.

    python tools/build_anm_data.py

Reads the *.anm files in build/run/th07.data/ and writes assets/anm/: one directory per
animation holding animation.lua and its textures. Run it after the game's data has been extracted
(build_portable.py does that), and not otherwise -- it overwrites assets/anm/ completely.

The steps are in a fixed order because they write over each other:

    extract_textures   root/*.png           textures out of each entry's THTX block
    extract_sprites    root/*.spr.toml      the sprite table, as a file of its own
    organize_anm       anm/<name>/*         moves the two above into a directory per animation
    extract_scripts    anm/<name>/*.bin     the streams, with their offsets rebased to the file
    write_entry_toml   anm/<name>/N.toml    the entry's remaining fields
    merge_entry_toml   anm/<name>/N.toml    folds the sprite table into it
    toml_to_lua        anm/<name>/*.lua     the description as Lua
    scripts_to_lua     anm/<name>/*.lua     the streams as Lua functions

organize_anm clears anm/ before it starts, so everything that belongs there has to be written
after it, not before. That is why extract_scripts and write_entry_toml come third and fourth
rather than first: run in the other order, their files are deleted by organize_anm and the result
is an animation with no scripts at all, which looks like a conversion that worked.

write_entry_toml is also why extract_scripts cannot be re-run later: it records the offsets it
finds, and those offsets are only right after extract_scripts has rebased them. Running
extract_scripts again rebases them a second time and every stream starts part way into the buffer.

The intermediate .toml and .bin files are removed at the end. They are what the steps pass to each
other, not something the game reads.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / 'build/run/th07.data'
ANM = DATA / 'anm'
ASSETS = ROOT / 'assets/anm'

STEPS = [
    'extract_textures.py',
    'extract_sprites.py',
    'organize_anm.py',
    'extract_scripts.py',
    'write_entry_toml.py',
    'merge_entry_toml.py',
    'toml_to_lua.py',
    'scripts_to_lua.py',
]


def main() -> int:
    if not any(DATA.glob('*.anm')):
        print(f'no *.anm files in {DATA}; run build_portable.py first')
        return 1

    # A previous run leaves converted files in the data directory, and the steps read whatever
    # they find there. Starting from the archive only is what makes the result reproducible.
    for stale in list(DATA.glob('*.png')) + list(DATA.glob('*.spr.toml')):
        stale.unlink()
    if ANM.is_dir():
        import shutil
        shutil.rmtree(ANM)

    for step in STEPS:
        script = ROOT / 'tools' / step
        if not script.is_file():
            print(f'missing: {script}')
            return 1
        print(f'--- {step}')
        result = subprocess.run([sys.executable, str(script)], cwd=str(ROOT))
        if result.returncode != 0:
            print(f'{step} failed')
            return result.returncode

    # The hand-off files go; the game reads the Lua and the textures only.
    removed = 0
    for pattern in ('*.toml', 'scripts_*.bin'):
        for path in ANM.rglob(pattern):
            path.unlink()
            removed += 1
    print(f'removed {removed} intermediate files')

    if ASSETS.is_dir():
        import shutil
        shutil.rmtree(ASSETS)
    import shutil
    shutil.copytree(ANM, ASSETS)

    animations = len(list(ASSETS.glob('*/animation.lua')))
    textures = len(list(ASSETS.glob('*/*.png')))
    streams = sum(len(__import__('re').findall(r'function\(s\)',
                                               p.read_text(encoding='utf-8')))
                  for p in ASSETS.glob('*/animation.lua'))
    print(f'\nassets/anm: {animations} animations, {streams} streams, {textures} textures')
    return 0


if __name__ == '__main__':
    sys.exit(main())
