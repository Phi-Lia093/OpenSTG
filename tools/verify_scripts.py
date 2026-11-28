#!/usr/bin/env python3
"""Check that the external script files are exactly the bytes that were in the ANM files.

    python tools/verify_scripts.py --source .tools/data_backup

The streams are what the game runs. A byte out of place shows up as an animation that behaves
slightly wrong, which is easy to miss and hard to trace, so the check is a direct comparison
against the file the regions came from rather than a run of the game.

The source must be a copy of the data directory from before step 3. For each entry it rebuilds
the region the converter would have cut -- from the first stream offset to the end of the entry
-- and compares that with the file on disk.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

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


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('--source', type=Path, required=True,
                        help='the data directory from before the streams were moved')
    parser.add_argument('--data', type=Path, default=ROOT / 'build/run/th07.data')
    args = parser.parse_args()

    source_dir = args.source
    data_dir = args.data
    if not source_dir.is_dir():
        print(f'source not found: {source_dir}')
        return 1

    names = sorted(p.stem for p in source_dir.glob('*.anm'))
    print(f'comparing {len(names)} animations against {source_dir}\n')

    checked = 0
    problems = []

    for name in names:
        original = (source_dir / f'{name}.anm').read_bytes()
        starts = entry_starts(original)

        for index, start in enumerate(starts):
            num_scripts = struct.unpack_from('<i', original, start + NUM_SCRIPTS)[0]
            if num_scripts == 0:
                continue

            end = starts[index + 1] if index + 1 < len(starts) else len(original)
            table = start + TABLE_START
            pairs = struct.unpack_from(f'<{num_scripts * 2}i', original, table)
            base = min(pairs[1::2])
            expected = original[start + base:end]

            external = data_dir / 'anm' / name / f'scripts_{index}.bin'
            if not external.is_file():
                problems.append(f'{name} entry {index}: missing')
                continue

            have = external.read_bytes()

            # The file ends with terminators the converter adds, so the comparison is against
            # the bytes the region came from, and the tail is checked separately.
            if len(have) < len(expected) or have[:len(expected)] != expected:
                where = next((i for i, (a, b) in enumerate(zip(expected, have)) if a != b),
                             None)
                problems.append(f'{name} entry {index}: {len(expected)} bytes expected, '
                                f'{len(have)} present'
                                + (f', first difference at {where}' if where is not None else ''))
                continue

            tail = have[len(expected):]
            terminator = struct.pack('<hHhH', -1, 8, 0, 0)
            if tail != terminator * (len(tail) // len(terminator)):
                problems.append(f'{name} entry {index}: the terminator tail is malformed')
                continue

            checked += 1

    if problems:
        print(f'{len(problems)} problem(s):')
        for problem in problems[:20]:
            print(f'  {problem}')
        return 1

    print(f'{checked} script regions match the originals byte for byte')
    return 0


if __name__ == '__main__':
    sys.exit(main())
