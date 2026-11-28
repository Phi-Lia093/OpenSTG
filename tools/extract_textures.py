#!/usr/bin/env python3
"""Move every animation's texture out of the ANM file and beside it as a PNG.

    python tools/extract_textures.py

The first of four steps that pull the format apart. Only the texture moves: the sprite table,
the scripts and the header stay exactly where they are, so a fault can only come from this
change.

For each entry that carries an embedded THTX image:

  * the image is decoded to <anm>_<entry>.png beside the file
  * hasData is cleared, which makes the loader open that file instead of reading pixels
  * the entry's name is rewritten to point at it

The entries keep their positions. Every offset inside an entry -- sprite table, script
streams, the offset table itself -- is relative to the entry, so leaving the entries in place
means none of them need correcting. The new name is appended at the end of the file and
nameOffset is pointed at it, which is the only change to an entry's bytes.
"""

from __future__ import annotations

import argparse
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
THANM = ROOT / '.tools/thtk-bin-12/thanm.exe'
DATA = ROOT / 'build/run/th07.data'

NAME_OFFSET = 0x1C
TEXTURE_OFFSET = 0x30
HAS_DATA = 0x34
NEXT_OFFSET = 0x38


def entry_starts(data: bytes) -> list[int]:
    starts = []
    offset = 0
    while True:
        starts.append(offset)
        next_offset = struct.unpack_from('<i', data, offset + NEXT_OFFSET)[0]
        if next_offset == 0:
            return starts
        offset += next_offset


def entry_name(data: bytes, start: int) -> str:
    name_offset = struct.unpack_from('<i', data, start + NAME_OFFSET)[0]
    end = data.find(b'\0', start + name_offset)
    return data[start + name_offset:end].decode('ascii', 'replace')


def extract(name: str, texture_name: str, destination: Path) -> bool:
    scratch = destination.parent / '.tex'
    if scratch.exists():
        shutil.rmtree(scratch)
    scratch.mkdir(parents=True)

    result = subprocess.run([str(THANM), '-x', str(DATA / f'{name}.anm'), texture_name],
                            cwd=str(scratch), capture_output=True, text=True,
                            errors='replace')
    produced = scratch / texture_name
    if not produced.is_file():
        print(f'  {name}: {texture_name} not extracted ({result.stderr[-120:]})')
        shutil.rmtree(scratch, ignore_errors=True)
        return False

    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(produced, destination)
    shutil.rmtree(scratch, ignore_errors=True)
    return True


def convert(name: str) -> int:
    path = DATA / f'{name}.anm'
    data = bytearray(path.read_bytes())
    starts = entry_starts(data)

    changed = 0
    # Names are appended, so the original bytes stay addressable while iterating.
    appended = bytearray()

    for index, start in enumerate(starts):
        if not data[start + HAS_DATA]:
            continue

        original_name = entry_name(data, start)
        target = f'{name}_{index}.png'
        if not extract(name, original_name, DATA / target):
            return 1

        # The loader opens the file from disk, so the name is a path it can open.
        new_name = (f'th07.data/{target}').encode('ascii') + b'\0'
        # nameOffset is relative to the entry, not the file. The first entry starts at zero,
        # so an absolute offset looks right there and points into the wrong entry for every
        # one after it -- which is why entry 0 loading correctly said nothing about entry 1.
        absolute = len(data) + len(appended)
        struct.pack_into('<i', data, start + NAME_OFFSET, absolute - start)
        data[start + HAS_DATA] = 0
        appended += new_name
        changed += 1

    if changed == 0:
        print(f'{name}: already external')
        return 0

    path.write_bytes(bytes(data) + bytes(appended))
    print(f'{name}: {changed} of {len(starts)} textures externalised')
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--data', type=Path, default=DATA)
    args = parser.parse_args()

    if not THANM.is_file():
        print(f'thanm not found: {THANM}')
        return 1

    names = sorted(p.stem for p in args.data.glob('*.anm'))
    print(f'{len(names)} animation files\n')

    for name in names:
        if convert(name) != 0:
            return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
