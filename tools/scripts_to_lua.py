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
would. Every instruction, jumps included, is a call: a jump carries the clock it sets as well as
where it goes, and writing it as `goto` would lose that. A register argument, marked in the
flags field, is written `{r = N}` -- a value that says it names a register.
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
INTERRUPT_LABEL = 21
DEC_JUMP = 5
CONDITIONAL = set(range(67, 79))
# The instructions that end a stream: EXIT_HIDE, EXIT_HIDE2 and EXIT. STOP_HIDE is not among
# them -- it hides the sprite and parks the script, and the stream continues after it, often at a
# label an interrupt names. Reading it as an end truncated every stream at its first STOP_HIDE,
# which is why blocks after it were never described and never ran.
EXITS = {-1, 1, 2}

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


def label_name(label: int) -> str:
    """The Lua label for an interrupt number. -1 becomes m1, which is a valid name."""
    return f'Lm1' if label < 0 else f'L{label}'


def jump_name(offset: int) -> str:
    """The Lua label for a jump target, named by the byte offset it lands on."""
    return f'J{offset}'


def jump_label(offset: int) -> str:
    """The Lua label a jump to `offset` goes to, or an empty string if nothing is emitted there.

    Only offsets some instruction starts at get a label; a jump to the middle of an instruction
    cannot happen in well-formed data, and emitting a label at a point nothing reaches would hide
    that rather than show it.
    """
    return jump_name(offset)


def stream_labels(data: bytes, start: int) -> list[int]:
    """The interrupt labels a stream can be entered at, in the order they appear.

    A label of -1 is the fallback the interpreter used when an interrupt named a label the stream
    did not have, so it is listed like any other.
    """
    labels = []
    position = start
    while position + 8 <= len(data):
        opcode, size, time, flags = struct.unpack_from('<hHhH', data, position)
        del time, flags
        count = max(0, size // 4 - 2)
        if position + 8 + count * 4 > len(data):
            break
        if opcode == INTERRUPT_LABEL and count:
            value = struct.unpack_from('<i', data, position + 8)[0]
            if value not in labels:
                labels.append(value)
        if opcode in EXITS or size < 8:
            break
        position += size
    return labels


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

    # Every interrupt number this stream can be entered at. The entry dispatch at the top of the
    # function needs all of them, and they have to be known before the body is written.
    labels = []
    for _, opcode, _, _, _, args in instructions:
        if opcode == INTERRUPT_LABEL and args:
            value = struct.unpack('<i', struct.pack('<I', args[0]))[0]
            if value not in labels:
                labels.append(value)

    # The time each label starts at: the time of the instruction after it. This is what the
    # interpreter set the clock to when an interrupt jumped to the label.
    label_times = {}
    for position in range(len(instructions)):
        opcode, args = instructions[position][1], instructions[position][5]
        if opcode != INTERRUPT_LABEL or not args:
            continue
        value = struct.unpack('<i', struct.pack('<I', args[0]))[0]
        following = instructions[position + 1][3] if position + 1 < len(instructions) else 0
        label_times[value] = following

    lines = []

    if labels:
        # The entry point, then a jump for each label. `local at` is the only local in the
        # function, and it is declared before every label, so no goto crosses a declaration.
        lines.append('        local at = s:entry()')
        for value in labels:
            lines.append(f'        if at == {value} then goto {label_name(value)} end')

    current_time = None

    # Every offset a jump targets gets a label, so the jump has somewhere to go.
    jump_targets = set()
    for instruction in instructions:
        if instruction[1] == JUMP and instruction[5]:
            jump_targets.add(struct.unpack('<i', struct.pack('<I', instruction[5][0]))[0])

    for relative, opcode, size, time, flags, args in instructions:
        del size

        # The place this instruction starts, which is what a jump's offset names.
        if relative in jump_targets:
            lines.append(f'        ::{jump_name(relative)}::')

        if time != current_time:
            lines.append(f'        s:at({time})')
            current_time = time

        name = names.get(opcode, f'OP{opcode}')

        if opcode in EXITS:
            lines.append(f'        s:{method_name(name)}()')
            continue

        # An interrupt label marks a place the stream can be resumed from. The Lua label is what
        # the entry dispatch jumps to; the call beside it is what running the stream records, so
        # that a run of the stream reports the same instructions the binary holds.
        if opcode == INTERRUPT_LABEL:
            value = struct.unpack('<i', struct.pack('<I', args[0]))[0] if args else 0
            # The time the label starts at, which is what the interpreter set the clock to when an
            # interrupt jumped here. Without it a resumed stream kept the clock it had, so its
            # next at(t) was already satisfied and the stream ran past the STOP it should have
            # waited at -- which is how a menu item ended up hidden on every frame.
            lines.append(f'        ::{label_name(value)}::')
            lines.append(f'        s:LABEL({value}, {label_times.get(value, 0)})')
            continue

        rendered = []
        float_args = floats.get(name, set())
        for index, raw in enumerate(args):
            if flags & (1 << index):
                # A register reference. The instruction's flags bit says this argument names a
                # register rather than carrying a value, and a plain number cannot say that --
                # r(10008) would be an undefined global, which is what an earlier version
                # generated and what made every stream using a register fail at run time.
                rendered.append(f'{{r = {raw}}}')
            elif name in BYTE_ARGUMENT and index == 1:
                rendered.append(str(raw & 0xFF))
            else:
                rendered.append(format_number(raw, index in float_args))

        call = f'        s:{method_name(name)}('
        call += ', '.join(rendered)
        call += ')'

        # A jump goes somewhere and sets the clock. Leaving it as a plain call loses the going:
        # the runtime's JUMP records where to go and reports a restart, but a coroutine cannot be
        # moved, so the stream carried on to the next instruction instead. A looping animation
        # jumped back to its first frame and, in Lua, fell through to the EXIT_HIDE after the
        # jump -- which is why a player standing still turned invisible.
        #
        # The clock is set first, then a Lua jump to the offset's label. Both statements are
        # emitted together so they cannot be separated.
        if opcode == JUMP and args:
            offset = struct.unpack('<i', struct.pack('<I', args[0]))[0]
            target = jump_label(offset)
            if target:
                clock = struct.unpack('<i', struct.pack('<I', args[1]))[0] if len(args) > 1 else 0
                # Both halves of the jump on one line: the clock it sets and where it goes. The
                # runtime uses the clock; the check reads both, so a jump is compared as the
                # single instruction the binary holds rather than as two unrelated statements.
                lines.append(f'        s:SET_CLOCK({clock}, {offset})')
                lines.append(f'        goto {target}')
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

            # Each stream's interrupt labels, so the runtime can tell whether an interrupt names
            # one before building a coroutine for it. The interpreter answered the same question
            # by scanning the stream; here it is written down, which is what makes falling back
            # to -1 possible. Without it, an interrupt naming 2 built a coroutine entered at 2 for
            # a stream that only has -1 -- the dispatch matched nothing, the stream ran from the
            # top and reached its STOP_HIDE again, and the sprite was never visible.
            all_labels = [stream_labels(data, start) for start in sorted(offsets)]
            lines.append('    labels = {')
            for index, labels in enumerate(all_labels):
                listed = ', '.join(str(value) for value in labels)
                lines.append(f'      [{index}] = {{ {listed} }},')
            lines.append('    },')

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
