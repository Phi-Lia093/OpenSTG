#!/usr/bin/env python3
"""Describe each animation in Lua instead of TOML.

    python tools/toml_to_lua.py

First of three steps that replace the remaining files with Lua. This one moves only the
description -- texture name, size, format, rectangles -- and leaves the instruction streams in
their binary files, so a fault here can only be this change.

An entry becomes a Lua table:

    return {
      [0] = {
        texture = "texture_0.png",
        width = 512, height = 256, format = 5,
        sprites = {
          { id = 0, rect = { 0, 0, 80, 32 } },
        },
        scripts = {
          { index = 0, offset = 0 },
        },
      },
    }

Why Lua rather than a smaller TOML: the streams are next to become Lua, and an animation
described in one language is easier to follow than one split across two. Lua also expresses the
rectangles without a second syntax for the same thing.

The file is a returned table rather than a sequence of calls so it can be loaded with lua_pcall
and read through the table interface, with no Lua code running that could touch anything else.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')
ANM_DIR = DATA / 'anm'

SPRITE_KEY = re.compile(r'^\[\[sprite\]\]$', re.M)
SCRIPT_KEY = re.compile(r'^\[\[script\]\]$', re.M)


def parse_entry(text: str) -> dict:
    """A small reader for the subset the converter wrote.

    Reading it directly is simpler than depending on a TOML library for a file whose shape is
    known: a header of scalars, then repeated [[sprite]] and [[script]] blocks.

    `sprites` and `scripts` each appear twice with different meanings -- the count in the header
    and the array built from the [[sprite]] and [[script]] blocks -- so the arrays are kept
    under their own keys.
    """
    entry: dict = {'spriteList': [], 'scriptList': []}
    block = None

    for raw in text.splitlines():
        line = raw.strip()
        if not line or line.startswith('#'):
            continue

        if line == '[[sprite]]':
            block = {'id': 0, 'rect': []}
            entry['spriteList'].append(block)
            continue
        if line == '[[script]]':
            block = {'index': 0, 'offset': 0}
            entry['scriptList'].append(block)
            continue

        if '=' not in line:
            continue
        key, _, value = line.partition('=')
        key = key.strip()
        value = value.strip()

        if value.startswith('['):
            numbers = [int(part) for part in re.findall(r'-?\d+', value)]
            if block is None:
                entry[key] = numbers
            else:
                block[key] = numbers
        elif value.startswith('"'):
            entry[key] = value.strip('"')
        else:
            number = int(value)
            if block is None:
                entry[key] = number
            else:
                block[key] = number

    return entry


def entry_count(directory: Path) -> int:
    count = 0
    while (directory / f'{count}.toml').is_file():
        count += 1
    return count


def convert(name: str) -> int:
    directory = ANM_DIR / name
    count = entry_count(directory)
    if count == 0:
        print(f'{name}: no entries')
        return 1

    lines = [
        f'-- {name}: how the animation is put together.',
        '--',
        '-- One table per entry, in the order the game loads them. Each names its texture and',
        '-- gives the rectangles into it; the instruction streams are still separate files for',
        '-- now and are listed with where each begins.',
        '',
        'return {',
    ]

    for index in range(count):
        entry = parse_entry((directory / f'{index}.toml').read_text(encoding='utf-8'))
        lines.append(f'  [{index}] = {{')

        texture = entry.get('texture', '')
        lines.append(f'    texture = "{texture}",')
        lines.append(f"    width = {entry.get('width', 0)}, "
                     f"height = {entry.get('height', 0)}, "
                     f"format = {entry.get('format', 0)},")

        sprites = entry.get('spriteList', [])
        if sprites:
            lines.append('    sprites = {')
            for sprite in sprites:
                rect = ', '.join(str(v) for v in sprite['rect'])
                lines.append(f"      {{ id = {sprite['id']}, rect = {{ {rect} }} }},")
            lines.append('    },')
        else:
            lines.append('    sprites = {},')

        scripts = entry.get('scriptList', [])
        if scripts:
            lines.append('    scripts = {')
            for script in scripts:
                lines.append(f"      {{ index = {script['index']}, "
                             f"offset = {script['offset']} }},")
            lines.append('    },')
        else:
            lines.append('    scripts = {},')

        lines.append('  },')

    lines.append('}')

    (directory / 'animation.lua').write_text('\n'.join(lines) + '\n', encoding='utf-8')
    return count


def main() -> int:
    if not ANM_DIR.is_dir():
        print(f'not found: {ANM_DIR}')
        return 1

    total = 0
    for directory in sorted(p for p in ANM_DIR.iterdir() if p.is_dir()):
        total += convert(directory.name)

    print(f'wrote animation.lua for {total} entries')
    return 0


if __name__ == '__main__':
    sys.exit(main())
