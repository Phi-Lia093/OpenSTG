"""Check that the Lua description holds the same numbers as the TOML it replaces.

The description is now generated from TOML rather than being the source of truth, so the two
can be compared directly: every texture, size, format, rectangle and script offset must appear
in both. A conversion that dropped a key or transposed a number would otherwise only show up as
an animation drawing the wrong part of its texture.

Runs the Lua through a real interpreter rather than reading it as text, so a syntax error is
caught here rather than at load time.
"""

from __future__ import annotations

import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / 'build/run/th07.data'
ANM = DATA / 'anm'

SPRITE = re.compile(r'^\[\[sprite\]\]$', re.M)
SCRIPT = re.compile(r'^\[\[script\]\]$', re.M)


def parse_toml(text: str) -> dict:
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
            numbers = [int(p) for p in re.findall(r'-?\d+', value)]
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


def main() -> int:
    names = sorted(p.name for p in ANM.iterdir() if p.is_dir())
    print(f'comparing {len(names)} animations\n')

    problems = []
    checked = 0

    for name in names:
        directory = ANM / name
        lua_file = directory / 'animation.lua'
        if not lua_file.is_file():
            problems.append(f'{name}: no animation.lua')
            continue

        count = 0
        while (directory / f'{count}.toml').is_file():
            count += 1
        if count == 0:
            continue

        # Run the Lua and dump it back out in a comparable shape.
        with tempfile.NamedTemporaryFile('w', suffix='.lua', delete=False,
                                         encoding='utf-8') as handle:
            handle.write(f'''
local entries = dofile([[{lua_file.as_posix()}]])
for i = 0, {count - 1} do
  local e = entries[i]
  if not e then print("MISSING " .. i) 
  else
    print(string.format("entry %d %s %d %d %d", i, e.texture, e.width, e.height, e.format))
    for _, s in ipairs(e.sprites) do
      print(string.format("  sprite %d %d %d %d %d", s.id, s.rect[1], s.rect[2],
                          s.rect[3], s.rect[4]))
    end
    for _, s in ipairs(e.scripts) do
      print(string.format("  script %d %d", s.index, s.offset))
    end
  end
end
''')
            temp = handle.name

        try:
            run = subprocess.run(['lua', temp], capture_output=True, text=True,
                                 encoding='utf-8', errors='replace')
        except FileNotFoundError:
            print('no lua interpreter on PATH; install one or point LUA_BIN at it')
            return 1
        finally:
            Path(temp).unlink(missing_ok=True)

        if run.returncode != 0:
            problems.append(f'{name}: {run.stderr.strip()[:120]}')
            continue

        produced = [line for line in run.stdout.splitlines() if line.strip()]

        expected = []
        for index in range(count):
            entry = parse_toml((directory / f'{index}.toml').read_text(encoding='utf-8'))
            expected.append(f"entry {index} {entry.get('texture', '')} "
                            f"{entry.get('width', 0)} {entry.get('height', 0)} "
                            f"{entry.get('format', 0)}")
            for sprite in entry['spriteList']:
                rect = sprite['rect']
                expected.append(f"  sprite {sprite['id']} {rect[0]} {rect[1]} "
                                f"{rect[2]} {rect[3]}")
            for script in entry['scriptList']:
                expected.append(f"  script {script['index']} {script['offset']}")

        if produced != expected:
            difference = next((i for i, (a, b) in enumerate(zip(expected, produced)) if a != b),
                              min(len(expected), len(produced)))
            problems.append(f'{name}: {len(expected)} lines expected, {len(produced)} produced; '
                            f'first difference at {difference}')
            continue

        checked += 1

    if problems:
        print(f'{len(problems)} problem(s):')
        for problem in problems[:15]:
            print(f'  {problem}')
        return 1

    print(f'{checked} animations describe the same content in Lua as in TOML')
    return 0


if __name__ == '__main__':
    sys.exit(main())
