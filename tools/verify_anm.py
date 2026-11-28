"""Compare the instruction stream rebuilt from Lua against the original binary.

This is the check the whole approach exists for. The Lua is a transcription, so if it was
transcribed correctly the two arrays are identical: same instructions, same order, same
opcode, time, flags and argument bytes. Nothing has to be judged by playing the game.

Reports the first difference by name rather than a summary, because "the animations differ"
is not actionable and "entry 1 script 0 instruction 7: want opcode 17 time 60, got 18
time 60" is.

Usage:
    python tools/verify_anm.py title01
"""

from __future__ import annotations

import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
THANM = ROOT / '.tools/thtk-bin-12/thanm.exe'
DATA = ROOT / 'build/run/th07.data'
ASSETS = ROOT / 'assets'

HEADER = re.compile(r'^ENTRY #(\d+), VERSION (\d+)$')
INSTR = re.compile(r'^Instruction #(\d+):\s+(-?\d+)\s+(-?\d+)\s+(-?\d+)\s*(.*)$')

# How each opcode reads its arguments. This mirrors the host's table and is used to decide
# how a printed argument maps to the four bytes stored in the file.
FLOAT_ARGUMENTS = {
    6: (0, 1, 2), 7: (0, 1), 12: (0, 1, 2), 13: (0, 1, 2), 14: (0, 1),
    17: (0, 1, 2), 18: (0, 1, 2), 19: (0, 1, 2), 26: (0,), 27: (0,),
    29: (0, 1), 32: (2, 3, 4), 35: (2, 3, 4), 36: (2, 3),
}
BYTE_ARGUMENTS = {
    15: (0,),        # fade reads args[0].b[0]
    32: (1,),        # interp_pos easing
    34: (1, 2),      # interp_alpha easing then target
    33: (1, 2, 3),   # interp_color
    35: (1,),
    36: (1,),
}


def original_streams(anm: Path):
    """[(entry index, [script, ...])] where a script is a list of (time, flags, opcode, args)."""
    result = subprocess.run([str(THANM), '-l', str(anm)], capture_output=True, text=True,
                            errors='replace')
    if result.returncode != 0:
        return []

    entries = []
    entry = None
    script = None
    for raw in result.stdout.splitlines():
        line = raw.strip()
        m = HEADER.match(line)
        if m:
            entry = []
            entries.append(entry)
            script = None
            continue
        if line.startswith('Script:'):
            if entry is not None:
                script = []
                entry.append(script)
            continue
        m = INSTR.match(line)
        if m and script is not None:
            script.append((int(m.group(2)), int(m.group(3)), int(m.group(4)),
                           m.group(5).split()))
    return entries


def word_for(opcode: int, index: int, token: str) -> int:
    """The four bytes an argument occupies, as an integer for comparison."""
    if index in FLOAT_ARGUMENTS.get(opcode, ()):
        return struct.unpack('<I', struct.pack('<f', float(token.rstrip('f'))))[0]
    if index in BYTE_ARGUMENTS.get(opcode, ()):
        return int(token) & 0xFF
    return int(float(token)) & 0xFFFFFFFF


def compare(name: str) -> int:
    anm = DATA / f'{name}.anm'
    root = ASSETS / 'anm' / name

    if not anm.is_file():
        print(f'no original: {anm}')
        return 1
    if not root.is_dir():
        print(f'no converted output: {root}')
        return 1

    originals = original_streams(anm)
    print(f'{name}: {len(originals)} entries in the original')

    problems = 0
    checked = 0

    for entry_index, scripts in enumerate(originals):
        lua_path = root / f'{entry_index:02d}' / 'animation.lua'
        if not lua_path.is_file():
            print(f'  entry {entry_index}: no animation.lua')
            problems += 1
            continue

        # Read the emitted calls back out of the Lua. The syntax is fixed and line-oriented,
        # so this is a transcription of a transcription rather than a Lua parser.
        emitted: dict[int, list[tuple[int, int, int, list[str]]]] = {}
        call = re.compile(r'^s(\d+):emit\((\d+)(?:, (.*))?\)$')
        for line in lua_path.read_text(encoding='utf-8').splitlines():
            m = call.match(line.strip())
            if not m:
                continue
            script_index = int(m.group(1))
            opcode = int(m.group(2))
            body = m.group(3) or ''

            args: list[str] = []
            time = 0
            flags = 0
            if '{' in body:
                positional, _, options = body.partition('{')
                for item in options.rstrip('}) ').split(','):
                    if '=' in item:
                        key, value = (part.strip() for part in item.split('=', 1))
                        if key == 'time':
                            time = int(value)
                        elif key == 'flags':
                            flags = int(value)
                body = positional.rstrip(', ')
            if body:
                args = [part.strip() for part in body.split(',') if part.strip()]

            emitted.setdefault(script_index, []).append((time, flags, opcode, args))

        if sorted(emitted) != list(range(len(scripts))):
            print(f'  entry {entry_index}: scripts {sorted(emitted)} in Lua, '
                  f'{len(scripts)} in the original')
            problems += 1
            continue

        for script_index, instructions in enumerate(scripts):
            want = instructions
            have = emitted[script_index]
            checked += 1

            if len(want) != len(have):
                print(f'  entry {entry_index} script {script_index}: '
                      f'{len(want)} instructions in the original, {len(have)} in the Lua')
                problems += 1
                continue

            for position, (original, raised) in enumerate(zip(want, have)):
                o_time, o_flags, o_opcode, o_args = original
                h_time, h_flags, h_opcode, h_args = raised

                if o_opcode != h_opcode:
                    print(f'  entry {entry_index} script {script_index} '
                          f'instruction {position}: opcode {o_opcode} vs {h_opcode}')
                    problems += 1
                    break
                if o_time != h_time:
                    print(f'  entry {entry_index} script {script_index} '
                          f'instruction {position}: time {o_time} vs {h_time}')
                    problems += 1
                    break
                if o_flags != h_flags:
                    print(f'  entry {entry_index} script {script_index} '
                          f'instruction {position}: flags {o_flags} vs {h_flags}')
                    problems += 1
                    break
                if len(o_args) != len(h_args):
                    print(f'  entry {entry_index} script {script_index} '
                          f'instruction {position}: {len(o_args)} args vs {len(h_args)}')
                    problems += 1
                    break

                mismatch = None
                for index, (o_token, h_token) in enumerate(zip(o_args, h_args)):
                    if word_for(o_opcode, index, o_token) != word_for(o_opcode, index, h_token):
                        mismatch = (index, o_token, h_token)
                        break
                if mismatch:
                    index, o_token, h_token = mismatch
                    print(f'  entry {entry_index} script {script_index} '
                          f'instruction {position} (opcode {o_opcode}): '
                          f'argument {index} is {o_token} in the original, {h_token} in the Lua')
                    problems += 1
                    break

    print(f'\n{checked} script(s) compared')
    if problems:
        print(f'{problems} problem(s)')
        return 1
    print('every instruction matches')
    return 0


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    return compare(sys.argv[1])


if __name__ == '__main__':
    sys.exit(main())
