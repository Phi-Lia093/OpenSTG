#!/usr/bin/env python3
"""Convert an ANM file into a directory of PNG, TOML and Lua.

    python tools/convert_anm.py title01
    python tools/convert_anm.py title01 --out assets

Layout, one directory per entry:

    anm/title01/00/texture.png
    anm/title01/00/uvs.toml
    anm/title01/00/animation.lua

The Lua is a transcription, not a translation: every instruction becomes one `emit` call
with the same opcode, the same arguments and the same time. Nothing is grouped, renamed or
inferred. That is what makes the result verifiable -- the instruction array rebuilt from Lua
can be compared against the one the binary produces, and either matches or names the first
difference.

thtk's thanm does the reading and the texture extraction; this script only transcribes.
"""

from __future__ import annotations

import argparse
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
THANM = ROOT / '.tools/thtk-bin-12/thanm.exe'
DATA = ROOT / 'build/run/th07.data'
ASSETS = ROOT / 'assets'

# Opcode number to the name the Lua host knows. Taken from src/script/AnmVm.hpp; only the
# ones appearing in converted files are needed, and a missing entry is reported rather than
# guessed at.
OPCODE_NAMES = {
    -1: 'ANM_EXIT_HIDE',
    1: 'ANM_EXIT_HIDE2',
    2: 'ANM_EXIT',
    3: 'ANM_SET_ACTIVE_SPRITE',
    4: 'ANM_JUMP',
    5: 'ANM_DEC_JUMP',
    6: 'ANM_SET_TRANSLATION',
    7: 'ANM_SET_SCALE',
    8: 'ANM_SET_ALPHA',
    9: 'ANM_SET_COLOR',
    10: 'ANM_FLIP_X',
    11: 'ANM_FLIP_Y',
    12: 'ANM_SET_ROTATION',
    13: 'ANM_SET_ANGLE_VEL',
    14: 'ANM_SET_SCALE_SPEED',
    15: 'ANM_FADE',
    16: 'ANM_SET_BLEND',
    17: 'ANM_POS_TIME_LINEAR',
    18: 'ANM_POS_TIME_DECEL',
    19: 'ANM_POS_TIME_ACCEL',
    20: 'ANM_STOP',
    21: 'ANM_INTERRUPT_LABEL',
    22: 'ANM_22',
    23: 'ANM_STOP_HIDE',
    24: 'ANM_SET_USE_OFFSET',
    25: 'ANM_SET_AUTO_ROTATE',
    26: 'ANM_SET_SCROLL_POS_X',
    27: 'ANM_SET_SCROLL_POS_Y',
    28: 'ANM_SET_VISIBILITY',
    29: 'ANM_INTERP_SCALE',
    30: 'ANM_SET_ZWRITE_DISABLE',
    31: 'ANM_SET_CAMERA_MODE',
    32: 'ANM_INTERP_POS',
    33: 'ANM_INTERP_COLOR',
    34: 'ANM_INTERP_ALPHA',
    35: 'ANM_INTERP_ROTATE',
    36: 'ANM_INTERP_SCALE_2',
}

# How each opcode reads its arguments, which decides whether a value is written as a float
# literal or an integer one. A float literal is needed where the VM reads the word through
# .f; writing an integer there would produce a completely different value.
FLOAT_ARGUMENTS = {
    6: (0, 1, 2),
    7: (0, 1),
    12: (0, 1, 2),
    13: (0, 1, 2),
    14: (0, 1),
    17: (0, 1, 2),
    18: (0, 1, 2),
    19: (0, 1, 2),
    26: (0,),
    27: (0,),
    29: (0, 1),
    32: (2, 3, 4),
    35: (2, 3, 4),
    36: (2, 3),
}

HEADER = re.compile(r'^ENTRY #(\d+), VERSION (\d+)$')
SPRITE = re.compile(r'^Sprite: (\d+) (\d+)\*(\d+)\+(\d+)\+(\d+)$')
INSTR = re.compile(r'^Instruction #(\d+):\s+(-?\d+)\s+(-?\d+)\s+(-?\d+)\s*(.*)$')
KV = re.compile(r'^([A-Za-z][\w-]*):\s*(.*)$')


class Entry:
    def __init__(self, index: int, version: int):
        self.index = index
        self.version = version
        self.name = ''
        self.width = 0
        self.height = 0
        self.sprites: list[tuple[int, int, int, int, int]] = []
        self.scripts: list[list[tuple[int, int, int, list[str]]]] = []


def parse_listing(text: str) -> list[Entry]:
    entries: list[Entry] = []
    entry: Entry | None = None
    script: list[tuple[int, int, int, list[str]]] | None = None

    for raw in text.splitlines():
        line = raw.strip()
        if not line:
            continue

        m = HEADER.match(line)
        if m:
            entry = Entry(int(m.group(1)), int(m.group(2)))
            entries.append(entry)
            script = None
            continue

        m = SPRITE.match(line)
        if m and entry is not None:
            entry.sprites.append((int(m.group(1)), int(m.group(2)), int(m.group(3)),
                                  int(m.group(4)), int(m.group(5))))
            continue

        if line.startswith('Script:'):
            if entry is not None:
                script = []
                entry.scripts.append(script)
            continue

        m = INSTR.match(line)
        if m and script is not None:
            # time, flags, opcode, then the arguments as written.
            script.append((int(m.group(2)), int(m.group(3)), int(m.group(4)),
                           m.group(5).split()))
            continue

        m = KV.match(line)
        if m and entry is not None:
            key, value = m.group(1), m.group(2).strip()
            if key == 'Name':
                entry.name = value
            elif key == 'Width':
                entry.width = int(value)
            elif key == 'Height':
                entry.height = int(value)

    return entries


def list_anm(path: Path) -> list[Entry]:
    result = subprocess.run([str(THANM), '-l', str(path)], capture_output=True, text=True,
                            errors='replace')
    if result.returncode != 0:
        print(f'  thanm failed: {result.stderr[-300:]}')
        return []
    return parse_listing(result.stdout)


def extract_texture(anm: Path, texture_name: str, destination: Path) -> bool:
    scratch = destination.parent / '.extract'
    if scratch.exists():
        shutil.rmtree(scratch)
    scratch.mkdir(parents=True)

    result = subprocess.run([str(THANM), '-x', str(anm), texture_name], cwd=str(scratch),
                            capture_output=True, text=True, errors='replace')
    produced = scratch / texture_name
    if not produced.is_file():
        print(f'  {texture_name} not extracted: {result.stderr[-200:]}')
        shutil.rmtree(scratch, ignore_errors=True)
        return False

    destination.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(produced, destination)
    shutil.rmtree(scratch, ignore_errors=True)
    return True


def format_number(token: str, is_float: bool) -> str:
    """Render an argument so the Lua host reads back the same value.

    thanm prints a float with an f suffix and an integer without. The suffix is dropped, and
    a float that happens to be whole keeps a decimal point so Lua does not deliver it as an
    integer; the host reads floats and integers through the same number, but the types are
    what decide the byte layout, and being explicit costs nothing.
    """
    text = token.rstrip('f')

    if is_float:
        value = float(text)
        if value == int(value) and abs(value) < 1e15:
            return f'{int(value)}.0'
        return repr(value)

    return str(int(float(text)))


def write_uvs(entry: Entry, texture_name: str, path: Path) -> None:
    lines = [
        '# Sprite rectangles in texture pixels, as the original stores them.',
        f'version = {entry.version}',
        f'texture = "{texture_name}"',
        f'width = {entry.width}',
        f'height = {entry.height}',
        '',
    ]
    for index, width, height, x, y in entry.sprites:
        lines.append('[[sprite]]')
        lines.append(f'id = {index}')
        lines.append(f'rect = [{x}, {y}, {width}, {height}]')
        lines.append('')
    path.write_text('\n'.join(lines).rstrip() + '\n', encoding='utf-8')


def read_script_indices(anm: Path) -> list[list[int]]:
    """Read each entry's script indices straight from the binary.

    thanm numbers its scripts 0, 1, 2 in listing order, but the file stores the number the
    loader will actually use, and that is not the position: title01's entries 2 to 6 hold
    negative values that place their scripts immediately after the first two entries, while
    entries 7 to 9 hold zeros that place theirs at the entry's own slot. The layout matters
    because the game addresses scripts by that number, so it is read from the data rather
    than inferred.
    """
    data = anm.read_bytes()
    result = []
    offset = 0

    while True:
        num_sprites, num_scripts = struct.unpack_from('<2i', data, offset)
        next_offset = struct.unpack_from('<i', data, offset + 0x38)[0]

        table = offset + 0x40
        values = struct.unpack_from(f'<{num_sprites + num_scripts * 2}i', data, table)
        # After the sprite offsets come (script index, script offset) pairs.
        indices = list(values[num_sprites::2])
        result.append(indices)

        if next_offset == 0 or len(result) > 256:
            break
        offset += next_offset

    return result


def write_lua(entry: Entry, indices: list[int], path: Path) -> int:
    """Transcribe every instruction, one emit call each. Returns the count."""
    lines = [
        '-- Transcribed from the original instruction stream.',
        '--',
        '-- One call per instruction, in the original order, with the original arguments.',
        '-- `time` and `flags` are instruction header fields, so they travel in the table.',
        '--',
        '-- This file is machine-written. It is a record of what the binary contained, not a',
        '-- hand-authored animation; the abstraction over it comes later, on top.',
        '',
    ]

    unknown = set()
    count = 0

    for script_position, instructions in enumerate(entry.scripts):
        # The name is only local; the number in the call is the one the loader uses, taken
        # from the binary rather than from the loop position.
        script_index = indices[script_position] if script_position < len(indices) \
            else script_position
        lines.append(f'local s{script_position} = anm.script({script_index})')
        lines.append('')

        for time, flags, opcode, args in instructions:
            name = OPCODE_NAMES.get(opcode)
            if name is None:
                unknown.add(opcode)
                continue

            float_positions = FLOAT_ARGUMENTS.get(opcode, ())
            rendered = [
                format_number(token, index in float_positions)
                for index, token in enumerate(args)
            ]

            options = []
            if time != 0:
                options.append(f'time = {time}')
            if flags != 0:
                options.append(f'flags = {flags}')
            if options:
                rendered.append('{ ' + ', '.join(options) + ' }')

            if rendered:
                lines.append(f's{script_position}:emit({opcode}, {", ".join(rendered)})')
            else:
                lines.append(f's{script_position}:emit({opcode})')
            count += 1

        lines.append('')

    path.write_text('\n'.join(lines), encoding='utf-8')

    if unknown:
        print(f'  unsupported opcode(s): {sorted(unknown)}')
    return count


def convert(name: str, out_root: Path) -> int:
    anm = DATA / f'{name}.anm'
    if not anm.is_file():
        print(f'no such ANM: {anm}')
        return 1

    target = out_root / 'anm' / name
    if target.exists():
        shutil.rmtree(target)
    target.mkdir(parents=True)

    entries = list_anm(anm)
    if not entries:
        print(f'{name}: could not be read')
        return 1

    # The script numbers the loader uses, read from the binary because the listing does not
    # carry them.
    script_indices = read_script_indices(anm)

    total_instructions = 0
    for entry in entries:
        entry_dir = target / f'{entry.index:02d}'
        entry_dir.mkdir(parents=True)

        texture_name = Path(entry.name).name
        if not extract_texture(anm, entry.name, entry_dir / texture_name):
            return 1
        if texture_name != 'texture.png':
            (entry_dir / texture_name).rename(entry_dir / 'texture.png')

        write_uvs(entry, 'texture.png', entry_dir / 'uvs.toml')
        total_instructions += write_lua(entry, script_indices[entry.index]
                                        if entry.index < len(script_indices) else [],
                                        entry_dir / 'animation.lua')

    print(f'{name}: {len(entries)} entries, '
          f'{sum(len(e.sprites) for e in entries)} sprites, '
          f'{sum(len(e.scripts) for e in entries)} scripts, '
          f'{total_instructions} instructions')
    return 0


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('names', nargs='*')
    parser.add_argument('--out', type=Path, default=ASSETS)
    parser.add_argument('--all', action='store_true')
    args = parser.parse_args()

    if not THANM.is_file():
        print(f'thanm not found: {THANM}')
        return 1

    names = args.names
    if args.all:
        names = sorted(p.stem for p in DATA.glob('*.anm'))
    if not names:
        parser.error('name at least one ANM, or pass --all')

    for name in names:
        if convert(name, args.out) != 0:
            return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
