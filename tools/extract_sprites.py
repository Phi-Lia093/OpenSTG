#!/usr/bin/env python3
"""Move every animation's sprite table out of the ANM file into TOML.

    python tools/extract_sprites.py

The second of four steps. Only the sprite table moves: textures are already external, and the
scripts and header stay binary, so a fault can only come from this change.

For each entry:

  * every sprite's (id, x, y, width, height) is written to <anm>_<entry>.spr.toml, one
    [[sprite]] element each
  * the entry's offset table loses the sprite offsets and keeps only the script pairs
  * the sprite payload is left in place rather than cut out: the entries keep their positions
    and their internal offsets stay valid, which is what makes the change reviewable

The sprite bytes remaining in the file are unreachable once the offset table stops pointing at
them. Removing them is a separate step, once this one is known good.
"""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')

NUM_SPRITES = 0x00
NUM_SCRIPTS = 0x04
NEXT_OFFSET = 0x38
TABLE_START = 0x40

# AnmRawSprite: id, offset.x, offset.y, size.x, size.y
SPRITE_SIZE = 20


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
    data = bytearray(path.read_bytes())
    starts = entry_starts(data)

    written = 0
    for index, start in enumerate(starts):
        num_sprites = struct.unpack_from('<i', data, start + NUM_SPRITES)[0]
        num_scripts = struct.unpack_from('<i', data, start + NUM_SCRIPTS)[0]

        table = start + TABLE_START
        sprite_offsets = list(struct.unpack_from(f'<{num_sprites}i', data, table))
        script_pairs = list(struct.unpack_from(f'<{num_scripts * 2}i', data,
                                               table + num_sprites * 4))

        # Write the table as TOML.
        lines = [
            '# Sprite rectangles in texture pixels: x, y, width, height.',
            '# Positions the original stored as floats; every value in the shipped data is',
            '# whole, so they are written as integers and read back exactly.',
            '',
        ]
        for sprite_offset in sprite_offsets:
            position = start + sprite_offset
            sprite_id, ox, oy, sx, sy = struct.unpack_from('<i4f', data, position)
            lines.append('[[sprite]]')
            lines.append(f'id = {sprite_id}')
            lines.append(f'rect = [{int(ox)}, {int(oy)}, {int(sx)}, {int(sy)}]')
            lines.append('')

        target = DATA / f'{name}_{index}.spr.toml'
        target.write_text('\n'.join(lines).rstrip() + '\n', encoding='utf-8')
        written += 1

        # The offset table now holds only the script pairs, so they move to the front.
        for offset_in_table, value in enumerate(script_pairs):
            struct.pack_into('<i', data, table + offset_in_table * 4, value)

    path.write_bytes(bytes(data))
    print(f'{name}: {written} sprite tables externalised')
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
