#!/usr/bin/env python3
"""Move every instruction stream out of the ANM files, verbatim.

    python tools/extract_scripts.py

The third of four steps, and the one where a mistake would be hard to see: the streams are what
the game runs, so a single wrong byte shows up as an animation that behaves slightly wrong
rather than a file that fails to load.

The encoding does not change. Each entry's streams are copied to
anm/<name>/scripts_<entry>.bin with nothing altered, and the entry's offset table keeps its
values -- they now index that file instead of the entry. ExecuteScript is untouched, and the
result can be compared byte for byte against the source, which tools/verify_scripts.py does.

What stays in the entry: the header, the offset table and the appended texture name. Those are
addressed by nextOffset and nameOffset, which are relative to where the entry begins.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')
ANM_DIR = DATA / 'anm'

NUM_SPRITES = 0x00
NUM_SCRIPTS = 0x04
NEXT_OFFSET = 0x38
TABLE_START = 0x40


def entry_starts(data: bytes) -> list[int]:
    starts = []
    offset = 0
    while True:
        starts.append(offset)
        next_offset = struct.unpack_from('<i', data, offset + NEXT_OFFSET)[0]
        if next_offset == 0:
            return starts
        offset += next_offset


def convert(name: str) -> int:
    path = DATA / f'{name}.anm'
    original = path.read_bytes()
    data = bytearray(original)
    starts = entry_starts(original)

    written = 0
    for index, start in enumerate(starts):
        num_scripts = struct.unpack_from('<i', original, start + NUM_SCRIPTS)[0]
        if num_scripts == 0:
            continue

        end = starts[index + 1] if index + 1 < len(starts) else len(original)

        table = start + TABLE_START
        pairs = struct.unpack_from(f'<{num_scripts * 2}i', original, table)
        offsets = pairs[1::2]

        # The streams run from the first offset to the end of the entry. Everything between
        # two offsets belongs to the earlier stream; there is no separate length field.
        base = min(offsets)
        region = original[start + base:end]

        target_dir = ANM_DIR / name
        target_dir.mkdir(parents=True, exist_ok=True)
        target_file = target_dir / f'scripts_{index}.bin'
        # Running twice must not rebase the offsets again. The file already exists only when
        # the streams were written, so its presence is the marker.
        if target_file.exists():
            continue
        target_file.write_bytes(region)

        # The offsets keep their values, measured from the start of the region now rather than
        # from the entry, so they are rebased once, here, in the table the loader reads.
        for position, value in enumerate(offsets):
            struct.pack_into('<i', data, table + (position * 2 + 1) * 4, value - base)

        written += 1

    if written:
        path.write_bytes(bytes(data))
    print(f'{name}: {written} script regions externalised')
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--data', type=Path, default=DATA)
    args = parser.parse_args()

    names = sorted(p.stem for p in args.data.glob('*.anm'))
    print(f'{len(names)} animation files\n')

    for name in names:
        if convert(name) != 0:
            return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
