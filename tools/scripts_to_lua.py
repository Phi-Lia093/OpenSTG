#!/usr/bin/env python3
"""Write each animation as one Lua file: its entries, their rectangles and their streams.

    python tools/scripts_to_lua.py

Generates the whole file rather than editing one, so the result does not depend on what was
written before. The binary streams remain the source of truth until the Lua is shown to
describe them exactly, which tools/verify_scripts_lua.py checks.

The file is a returned table:

    return {
      [0] = {
        texture = "texture_0.png",
        width = 512, height = 256, format = 5,
        sprites = { { id = 0, rect = { 0, 0, 80, 32 } } },
        scripts = { { index = 0, offset = 0 } },
        [0] = function(s) ... end,   -- stream 0
      },
    }

A stream is a function of the script object, one call per instruction:

    [0] = function(s)
      s:SET_ACTIVE_SPRITE(0)
      s:at(256)
      ::L116::
      s:INTERP_POS_DECEL(409.6, 200, 0, 96)
      s:at(768)
      goto L116
    end

`at(t)` is where the original's time comparison lives: it yields while the clock is behind t
and returns immediately once it is not, so a frame runs as many instructions as the original
would. Jumps become `goto` and their targets labels, placed only where something jumps to them.
A register argument, marked in the flags field, is written `r(N)`.
"""

from __future__ import annotations

import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DATA = ROOT / 'build/run/th07.data'
ANM_DIR = DATA / 'anm'
HEADER = ROOT / 'src/script/AnmVm.hpp'
INTERPRETER = ROOT / 'src/script/AnmManager.cpp'

JUMP = 4
DEC_JUMP = 5
CONDITIONAL = set(range(67, 79))
EXITS = {-1, 1, 2, 23}

# Arguments read as a byte rather than a whole word, by the interpreter's own indexing.
BYTE_ARGUMENT = {'ANM_INTERP_POS', 'ANM_INTERP_ROTATE'}

ENTRY = re.compile(r'^\s*\[(\d+)\] = \{', re.M)


def opcode_names() -> dict[int, str]:
    text = HEADER.read_text(encoding='utf-8')
    start = text.find('ANM_EXIT_HIDE = -1')
    end = text.find('};', start)
    names = {int(v): n for n, v in re.findall(r'(ANM_\w+)\s*=\s*(-?\d+)', text[start:end])}
    names[0] = 'ANM_UNUSED_0'
    return names


def float_positions() -> dict[str, set[int]]:
    """Which argument positions each opcode reads as a float."""
    text = INTERPRETER.read_text(encoding='utf-8')
    bodies = {}
    for match in re.finditer(r'case (ANM_\w+):\s*', text):
        following = text.find('case ', match.end())
        bodies[match.group(1)] = text[match.end():following if following > 0 else len(text)]

    out = {}
    for name, body in bodies.items():
        positions = set()
        for position in range(8):
            if re.search(r'args\[' + str(position) + r'\]\.f', body) or \
               re.search(r'GET_FLOAT_(?:VALUE|PTR)\(' + str(position) + r'\)', body):
                positions.add(position)
        if positions:
            out[name] = positions

    # These two reach ANM_POS_TIME_LINEAR's body through a goto, so their own case reads
    # nothing; the shared body reads three floats and an int.
    for name in ('ANM_POS_TIME_DECEL', 'ANM_POS_TIME_ACCEL'):
        out[name] = {0, 1, 2}
    return out


def method_name(name: str) -> str:
    """A Lua method name for an opcode.

    Names come from the enumeration, but a few have none: opcode 22 and opcode 0 have no
    enumerator, and `s:22()` is not Lua. Those get a readable name instead.
    """
    if name.startswith('ANM_'):
        name = name[4:]
    if name == '22':
        return 'NOP22'
    if name == 'UNUSED_0':
        return 'NOP'
    return name


def format_number(value: int, is_float: bool) -> str:
    if is_float:
        as_float = struct.unpack('<f', struct.pack('<I', value & 0xFFFFFFFF))[0]
        if as_float != as_float or as_float in (float('inf'), float('-inf')):
            return '0/0' if as_float != as_float else ('math.huge' if as_float > 0
                                                       else '-math.huge')
        if as_float == int(as_float) and abs(as_float) < 1e15:
            return f'{int(as_float)}.0'
        return repr(as_float)
    signed = value if value < 0x80000000 else value - 0x100000000
    return str(signed)


def stream_body(data: bytes, start: int, names: dict[int, str],
                floats: dict[str, set[int]]) -> list[str]:
    instructions = []
    position = start
    while position + 8 <= len(data):
        opcode, size, time, flags = struct.unpack_from('<hHhH', data, position)
        count = max(0, size // 4 - 2)
        if position + 8 + count * 4 > len(data):
            break
        args = list(struct.unpack_from(f'<{count}I', data, position + 8))
        instructions.append((position - start, opcode, size, time, flags, args))
        if opcode in EXITS or size < 8:
            break
        position += size

    targets = set()
    for _, opcode, _, _, _, args in instructions:
        if opcode == JUMP and len(args) >= 1:
            targets.add(args[0])
        elif opcode == DEC_JUMP and len(args) >= 2:
            targets.add(args[1])
        elif opcode in CONDITIONAL and len(args) >= 3:
            targets.add(args[2])

    lines = []
    current_time = None

    for relative, opcode, size, time, flags, args in instructions:
        del size

        if relative in targets:
            lines.append(f'        ::L{relative}::')

        if time != current_time:
            lines.append(f'        s:at({time})')
            current_time = time

        name = names.get(opcode, f'OP{opcode}')

        if opcode in EXITS:
            lines.append(f'        s:{method_name(name)}()')
            continue

        rendered = []
        float_args = floats.get(name, set())
        for index, raw in enumerate(args):
            if flags & (1 << index):
                rendered.append(f'r({raw})')
            elif name in BYTE_ARGUMENT and index == 1:
                rendered.append(str(raw & 0xFF))
            else:
                rendered.append(format_number(raw, index in float_args))

        call = f'        s:{method_name(name)}('
        call += ', '.join(rendered)
        call += ')'

        # A jump whose only argument is the target is clearer as a jump than as a call that
        # has to be decoded to see where it goes.
        if opcode == JUMP and len(args) == 2 and not (flags & 0b11):
            lines.append(f'        goto L{args[0]}')
            continue

        lines.append(call)

    if not lines:
        lines.append('        s:EXIT_HIDE()')
    return lines


def read_description(directory: Path) -> dict[int, dict]:
    """The entries as they were written, taken from the file's own structure.

    The description is a Lua table, so rather than matching text this reads the entry lines and
    the arrays of rectangles and script offsets that follow them. Each `[N] = {` at the entries
    table's indentation starts an entry, and every later `[N] = {` at that indentation ends an
    entry -- including the animation table's own closing, which is not an entry.
    """
    animation = directory / 'animation.lua'
    lines = animation.read_text(encoding='utf-8').splitlines()

    entries: dict[int, dict] = {}
    current = None

    for line in lines:
        stripped = line.strip()

        match = re.match(r'^\[(\d+)\] = \{$', stripped)
        if match and line.startswith('  ') and not line.startswith('    '):
            current = int(match.group(1))
            entries[current] = {'text': [line], 'offsets': []}
            continue

        if current is None:
            continue

        entries[current]['text'].append(line)
        offset = re.search(r'offset = (\d+)', line)
        if offset:
            entries[current]['offsets'].append(int(offset.group(1)))

    out = {}
    for number, entry in entries.items():
        body = entry['text']
        while body and not body[-1].strip():
            body.pop()

        # The block runs from the entry's opening line to the end of the file or the next
        # entry, so its last line may be the entries table closing or a later entry's
        # opening. Trim from the end until the block ends with the entry's own `    },`.
        while body and body[-1].strip() not in ('},',):
            body.pop()
        if body:
            body.pop()  # the `    },` itself

        out[number] = {'text': '\n'.join(body[1:]), 'offsets': entry['offsets']}
    return out


def main() -> int:
    names = opcode_names()
    floats = float_positions()

    if not ANM_DIR.is_dir():
        print(f'not found: {ANM_DIR}')
        return 1

    streams = 0
    for directory in sorted(p for p in ANM_DIR.iterdir() if p.is_dir()):
        animation = directory / 'animation.lua'
        if not animation.is_file():
            continue

        entries = read_description(directory)
        if not entries:
            print(f'{directory.name}: no entries found')
            continue

        lines = [
            f'-- {directory.name}: how the animation is put together and what it does.',
            '--',
            '-- One table per entry. Each names its texture, gives the rectangles into it, and',
            '-- carries its instruction streams as functions of the script object.',
            '',
            'return {',
        ]

        for number in sorted(entries):
            block = entries[number]['text']
            offsets = entries[number]['offsets']

            script_file = directory / f'scripts_{number}.bin'
            data = script_file.read_bytes() if script_file.is_file() else b''

            # `block` holds the entry's own lines, without its opening `[N] = {` and its
            # closing `    },`; the streams go before that closing brace.
            body = block.rstrip().splitlines() if block.strip() else []
            lines.append(f'  [{number}] = {{')
            lines.extend(body)

            for index, start in enumerate(sorted(offsets)):
                streams += 1
                lines.append(f'')
                lines.append(f'    -- stream {index}')
                lines.append(f'    [{index}] = function(s)')
                lines.extend(stream_body(data, start, names, floats))
                lines.append('    end,')

            lines.append('  },')

        lines.append('}')
        animation.write_text('\n'.join(lines) + '\n', encoding='utf-8')

    print(f'wrote {streams} streams into '
          f'{len(list(ANM_DIR.glob("*/animation.lua")))} files')
    return 0


if __name__ == '__main__':
    sys.exit(main())
