#!/usr/bin/env python3
"""Check that the generated Lua describes exactly the instructions in the binary.

    python tools/verify_scripts_lua.py

The Lua was generated from the binary, so the test is whether reading it back yields the same
instructions: same opcode sequence and the same time for each. A stream is code the player
watches rather than a number on a screen, so a transcription error has to be caught here.

Shape read back:

    -- stream N
    [N] = function(s)
        s:at(t)                  sets the time
        ::LNNN::                 a jump target
        s:NAME(args)             an instruction
        goto LNNN                a JUMP whose only argument is its target
    end,

Only the shapes the generator emits are accepted; anything else is reported.
"""

from __future__ import annotations

import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ANM = ROOT / 'build/run/th07.data/anm'
HEADER = ROOT / 'src/script/AnmVm.hpp'

# The instructions that end a stream: EXIT_HIDE, EXIT_HIDE2 and EXIT. STOP_HIDE is not among
# them -- it hides the sprite and parks the script, and the stream continues after it, often at a
# label an interrupt names. Reading it as an end truncated every stream at its first STOP_HIDE,
# which is why blocks after it were never described and never ran.
EXITS = {-1, 1, 2}
JUMP_OPCODE = 4

CALL = re.compile(r'^s:(\w+)\((.*)\)$')
AT = re.compile(r'^s:at\((-?\d+)\)$')
LABEL = re.compile(r'^::L(m?\d+)::$')
GOTO = re.compile(r'^goto L(\d+)$')

# One entry's block: from `[N] = {` at two-space indent to the matching `  },`.
ENTRY_BLOCK = re.compile(r'^  \[(\d+)\] = \{\n(.*?)\n  \},$', re.M | re.S)
STREAM = re.compile(r'-- stream (\d+)\n\s+\[(\d+)\] = function\(s\)\n(.*?)\n\s+end,', re.S)


def opcode_numbers() -> dict[str, int]:
    text = HEADER.read_text(encoding='utf-8')
    start = text.find('ANM_EXIT_HIDE = -1')
    end = text.find('};', start)
    numbers = {n[4:]: int(v) for n, v in re.findall(r'(ANM_\w+)\s*=\s*(-?\d+)', text[start:end])}
    numbers['UNUSED_0'] = 0
    # The generator renames the two opcodes with no enumerator.
    numbers['NOP22'] = 22
    numbers['NOP'] = 0
    # The generator writes an interrupt label as s:LABEL(n), because the label is where a script
    # waits rather than an instruction it executes.
    numbers['LABEL'] = 21
    return numbers


def binary_stream(data: bytes, start: int) -> list[tuple[int, int]]:
    out = []
    position = start
    while position + 8 <= len(data):
        opcode, size, time, flags = struct.unpack_from('<hHhH', data, position)
        del flags
        out.append((time, opcode))
        if opcode in EXITS or size < 8:
            break
        position += size
    return out


def lua_stream(body: str, numbers: dict[str, int]) -> tuple[list[tuple[int, int]], list[str]]:
    out = []
    problems = []
    time = 0
    for raw in body.splitlines():
        line = raw.strip()
        if not line:
            continue

        # The entry dispatch at the top of every stream is not an instruction. It reads the
        # entry point and jumps to a label, which is how an interrupt resumes a stream; the
        # jumps it takes are not JUMP instructions and must not be counted as any.
        if line.startswith('local at = ') or line.startswith('if at == '):
            continue

        match = AT.match(line)
        if match:
            time = int(match.group(1))
            continue
        if LABEL.match(line):
            # The Lua label is where the entry dispatch jumps to. The s:LABEL call beside it is
            # what running the stream reports, so that is counted and this is not.
            continue
        if GOTO.match(line):
            out.append((time, JUMP_OPCODE))
            continue
        match = CALL.match(line)
        if not match:
            problems.append(f'cannot read: {line}')
            continue
        opcode = numbers.get(match.group(1))
        if opcode is None:
            problems.append(f'unknown opcode name: {match.group(1)}')
            continue
        out.append((time, opcode))
    return out, problems


def main() -> int:
    numbers = opcode_numbers()
    problems = []
    compared = 0

    for directory in sorted(p for p in ANM.iterdir() if p.is_dir()):
        animation = directory / 'animation.lua'
        if not animation.is_file():
            continue

        text = animation.read_text(encoding='utf-8')

        for entry_match in ENTRY_BLOCK.finditer(text):
            entry = int(entry_match.group(1))
            block = entry_match.group(2)

            offsets = [int(v) for v in re.findall(r'offset = (\d+)', block)]
            script_file = directory / f'scripts_{entry}.bin'
            if not script_file.is_file():
                if offsets:
                    problems.append(f'{directory.name} entry {entry}: no binary')
                continue

            data = script_file.read_bytes()
            ordered = sorted(offsets)

            for stream_match in STREAM.finditer(block):
                index = int(stream_match.group(1))
                body = stream_match.group(3)

                if index >= len(ordered):
                    problems.append(f'{directory.name} entry {entry} stream {index}: '
                                    f'no offset')
                    continue

                expected = binary_stream(data, ordered[index])
                produced, unreadable = lua_stream(body, numbers)
                problems.extend(f'{directory.name} entry {entry} stream {index}: {p}'
                                for p in unreadable)

                if len(produced) != len(expected):
                    problems.append(f'{directory.name} entry {entry} stream {index}: '
                                    f'{len(expected)} expected, {len(produced)} written')
                    continue

                if produced != expected:
                    difference = next(i for i, (a, b) in enumerate(zip(expected, produced))
                                      if a != b)
                    problems.append(f'{directory.name} entry {entry} stream {index} '
                                    f'instruction {difference}: {expected[difference]} vs '
                                    f'{produced[difference]}')
                    continue

                compared += 1

    print(f'{compared} streams compared')
    if problems:
        print(f'{len(problems)} problem(s):')
        for problem in problems[:15]:
            print(f'  {problem}')
        return 1

    print('every stream describes the same instructions as the binary')
    return 0


if __name__ == '__main__':
    sys.exit(main())
