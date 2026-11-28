#!/usr/bin/env python3
"""Fold the sprite table into the entry description, so an entry is one TOML file.

    python tools/merge_entry_toml.py

An entry was described by two files, <n>.toml and sprites_<n>.toml, opened one after the other
during a load. They describe the same entry and are always read together, so keeping them apart
buys nothing and costs a file handle per entry on every animation load -- 101 of them across the
shipped data.

After this an entry is:

    anm/<name>/<n>.toml        texture, size, format, counts, [[sprite]] and [[script]]
    anm/<name>/scripts_<n>.bin
    anm/<name>/texture_<n>.png

The [[sprite]] elements move into the description unchanged. Nothing about their content
changes, and the loader reads them from the same document it already parses for the rest.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')
ANM_DIR = DATA / 'anm'


def strip_comments(text: str) -> str:
    """The body of a sprite file without its leading explanation."""
    lines = text.splitlines()
    while lines and (lines[0].startswith('#') or not lines[0].strip()):
        lines.pop(0)
    return '\n'.join(lines).strip()


def main() -> int:
    merged = 0

    for directory in sorted(p for p in ANM_DIR.iterdir() if p.is_dir()):
        for sprite_file in sorted(directory.glob('sprites_*.toml')):
            entry = re.match(r'sprites_(\d+)\.toml$', sprite_file.name)
            if not entry:
                continue
            index = entry.group(1)

            description = directory / f'{index}.toml'
            if not description.is_file():
                print(f'{directory.name}: {description.name} missing, skipping')
                continue

            body = strip_comments(sprite_file.read_text(encoding='utf-8'))
            if not body:
                sprite_file.unlink()
                continue

            text = description.read_text(encoding='utf-8').rstrip() + '\n'
            text += '\n# The rectangles this entry draws, in texture pixels.\n\n'
            text += body + '\n'
            description.write_text(text, encoding='utf-8')

            sprite_file.unlink()
            merged += 1

    print(f'merged {merged} sprite tables into their descriptions')
    return 0


if __name__ == '__main__':
    sys.exit(main())
