#!/usr/bin/env python3
"""Run the four conversion steps in the order they depend on each other.

    python tools/build_anm_data.py

The steps are not interchangeable. Each one reads what the previous one left, and running them
out of order produces data that looks plausible and is wrong:

    extract_textures   embedded THTX -> PNG, entry name rewritten
    extract_sprites    sprite table -> TOML, offset table reduced to script pairs
    extract_scripts    streams -> .bin, script offsets rebased to that file
    write_entry_toml   the entry's remaining fields -> TOML

The third step rewrites the offsets it finds, and the fourth copies them into the description.
Doing the fourth before the third records the offsets as they were relative to the entry rather
than to the stream file, so every script starts part way into the buffer and the game steps
through the middle of an instruction: it reads a zero size field and never advances.

That is exactly what happened, and it presented as the title screen hanging with the process
still using CPU -- no crash, no log, nothing to indicate which file was wrong.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

STEPS = [
    'extract_textures.py',
    'extract_sprites.py',
    'extract_scripts.py',
    'write_entry_toml.py',
]


def main() -> int:
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
    return 0


if __name__ == '__main__':
    sys.exit(main())
