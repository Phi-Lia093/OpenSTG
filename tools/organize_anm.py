#!/usr/bin/env python3
"""Move the derived animation files into one directory per animation.

    python tools/organize_anm.py

The texture and sprite files are currently flat in the data directory, named
<anm>_<entry>.png and <anm>_<entry>.spr.toml. With 63 animations that is 199 files in among
the archive's own, which makes the directory hard to read and hard to tell apart from data
that has not been touched.

After this:

    th07.data/anm/title01/texture_0.png
    th07.data/anm/title01/sprites_0.toml
    ...

The references are updated to match: the name inside each ANM points at the new texture path,
and the loader looks for the sprite file beside it.
"""

from __future__ import annotations

import re
import shutil
import struct
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')
ANM_DIR = DATA / 'anm'

NAME_OFFSET = 0x1C
NEXT_OFFSET = 0x38
APPEND_MARK = b'th07.data/'

PATTERN = re.compile(r'^(?P<name>[\w]+)_(?P<entry>\d+)\.(?P<kind>png|spr\.toml)$')


def main() -> int:
    if not DATA.is_dir():
        print(f'data directory not found: {DATA}')
        return 1

    if ANM_DIR.exists():
        # A previous run left something behind; start clean rather than merging.
        shutil.rmtree(ANM_DIR)

    moved = 0
    for path in sorted(DATA.iterdir()):
        if not path.is_file():
            continue
        match = PATTERN.match(path.name)
        if not match:
            continue

        name = match.group('name')
        entry = match.group('entry')
        kind = match.group('kind')

        target_dir = ANM_DIR / name
        target_dir.mkdir(parents=True, exist_ok=True)

        if kind == 'png':
            target = target_dir / f'texture_{entry}.png'
        else:
            target = target_dir / f'sprites_{entry}.toml'

        path.replace(target)
        moved += 1

    print(f'moved {moved} files into {ANM_DIR}')

    # Point each ANM at its texture's new path. The name is appended at the end of the file,
    # so the entry's other offsets are unaffected; only nameOffset changes.
    updated = 0
    for anm in sorted(DATA.glob('*.anm')):
        data = bytearray(anm.read_bytes())
        changed = False

        offset = 0
        index = 0
        appended = bytearray()
        while True:
            name_offset = struct.unpack_from('<i', data, offset + NAME_OFFSET)[0]
            end = data.find(b'\0', offset + name_offset)
            current = data[offset + name_offset:end]

            if current.startswith(APPEND_MARK):
                new_name = (f'th07.data/anm/{anm.stem}/texture_{index}.png').encode('ascii')
                new_name += b'\0'
                absolute = len(data) + len(appended)
                # nameOffset is relative to the entry, not the file.
                struct.pack_into('<i', data, offset + NAME_OFFSET, absolute - offset)
                appended += new_name
                changed = True

            next_offset = struct.unpack_from('<i', data, offset + NEXT_OFFSET)[0]
            if next_offset == 0:
                break
            offset += next_offset
            index += 1

        if changed:
            anm.write_bytes(bytes(data) + bytes(appended))
            updated += 1

    print(f'updated the texture paths in {updated} animation files')
    return 0


if __name__ == '__main__':
    sys.exit(main())
