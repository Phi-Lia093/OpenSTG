"""Derive each opcode's argument kinds from the interpreter.

The text form cannot be written without knowing, for every opcode, how many arguments it has
and how they are read. Reading that from the source rather than from memory is not a matter of
convenience: the argument types are not uniform, and two cases are easy to get wrong:

  * ANM_INTERP_POS and ANM_INTERP_ROTATE take five arguments, and one of them is read as a
    byte (args[1].b[0]) rather than as a whole word
  * the JUMP_IF_* family compares two values and then reaches a shared `jump:` label that reads
    args[2] and args[3], so its own case shows only half its arguments

Both were wrong in the table I wrote by hand, and both produced a stream that loaded and ran
incorrectly rather than failing.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / 'src/script/AnmVm.hpp'
INTERPRETER = ROOT / 'src/script/AnmManager.cpp'

# Arguments read as a single byte. The interpreter indexes .b[] rather than .i or .f.
BYTE_ARGUMENT = {'ANM_INTERP_POS', 'ANM_INTERP_ROTATE'}

# Opcodes whose case reads nothing because they take nothing.
NO_ARGUMENTS = {'ANM_EXIT_HIDE2', 'ANM_EXIT', 'ANM_STOP', 'ANM_STOP_HIDE', 'ANM_22',
                'ANM_FLIP_X', 'ANM_FLIP_Y'}

# ANM_INTERRUPT_LABEL has no case of its own in the main switch: it is recognised during the
# interrupt scan, where its argument is the label number to compare.
INT_LABEL_ARGUMENT = ['int']

# Cases whose own body does not describe their arguments, with the reason.
SPECIAL = {
    'ANM_POS_TIME_DECEL': 'jumps into POS_TIME_LINEAR by a goto',
    'ANM_POS_TIME_ACCEL': 'jumps into POS_TIME_LINEAR by a goto',
    'ANM_STOP': 'falls through into the interrupt handler',
}

# The types the goto-shared bodies use, from POS_TIME_LINEAR.
SHARED_POSITION_TIME = ['float', 'float', 'float', 'int']

CONDITIONAL_JUMPS = {
    'ANM_JUMP_IF_EQ': ('int', 'int'),
    'ANM_JUMP_IF_EQ_FLOAT': ('float', 'float'),
    'ANM_JUMP_IF_NEQ': ('int', 'int'),
    'ANM_JUMP_IF_NEQ_FLOAT': ('float', 'float'),
    'ANM_JUMP_IF_LT': ('int', 'int'),
    'ANM_JUMP_IF_LT_FLOAT': ('float', 'float'),
    'ANM_JUMP_IF_LEQ': ('int', 'int'),
    'ANM_JUMP_IF_LEQ_FLOAT': ('float', 'float'),
    'ANM_JUMP_IF_GT': ('int', 'int'),
    'ANM_JUMP_IF_GT_FLOAT': ('float', 'float'),
    'ANM_JUMP_IF_GEQ': ('int', 'int'),
    'ANM_JUMP_IF_GEQ_FLOAT': ('float', 'float'),
}


def enumeration() -> dict[int, str]:
    text = HEADER.read_text(encoding='utf-8')
    start = text.find('ANM_EXIT_HIDE = -1')
    end = text.find('};', start)
    names = {int(value): name for name, value in
             re.findall(r'(ANM_\w+)\s*=\s*(-?\d+)', text[start:end])}
    names[0] = 'ANM_UNUSED_0'
    return names


def case_bodies() -> dict[str, str]:
    """The source of each `case ANM_X:` up to the next case label."""
    text = INTERPRETER.read_text(encoding='utf-8')
    bodies = {}
    for match in re.finditer(r'case (ANM_\w+):\s*', text):
        start = match.end()
        following = text.find('case ', start)
        end = following if following > 0 else len(text)
        bodies[match.group(1)] = text[start:end]
    return bodies


def argument_kinds(name: str, body: str, arg_count: int | None) -> list[str] | None:
    if name in CONDITIONAL_JUMPS:
        first, second = CONDITIONAL_JUMPS[name]
        return [first, second, 'int', 'int']
    if name in BYTE_ARGUMENT:
        return ['int', 'byte', 'int', 'int', 'int']
    if name in NO_ARGUMENTS:
        return []
    if name == 'ANM_INTERRUPT_LABEL':
        return list(INT_LABEL_ARGUMENT)
    if name in ('ANM_POS_TIME_DECEL', 'ANM_POS_TIME_ACCEL'):
        return list(SHARED_POSITION_TIME)

    # Both spellings occur: `args[N].i` and `instr->args[N].i`, and the second only became
    # common when the interpreter was restructured. Matching one spelling silently dropped
    # half the table.
    int_positions = set()
    float_positions = set()
    byte_positions = set()
    for position in range(8):
        if re.search(r'args\[' + str(position) + r'\]\.f', body) or \
           re.search(r'GET_FLOAT_(?:VALUE|PTR)\(' + str(position) + r'\)', body):
            float_positions.add(position)
        if re.search(r'args\[' + str(position) + r'\]\.(?:i|us)\b', body) or \
           re.search(r'GET_INT_(?:VALUE|PTR)\(' + str(position) + r'\)', body):
            int_positions.add(position)
        if re.search(r'args\[' + str(position) + r'\]\.b', body):
            byte_positions.add(position)

    if not int_positions and not float_positions and not byte_positions:
        return None

    highest = max(max(int_positions, default=-1), max(float_positions, default=-1),
                  max(byte_positions, default=-1))
    result = []
    for position in range(highest + 1):
        if position in byte_positions:
            result.append('byte')
        elif position in float_positions:
            result.append('float')
        else:
            result.append('int')
    return result


def wide_argument(body: str) -> bool:
    """Whether the body reads an argument as more than one word."""
    return bool(re.search(r'args\[\d+\]\.b\[[1-9]\]', body))


def main() -> int:
    names = enumeration()
    bodies = case_bodies()

    # -1 ends a stream on the opcode alone, so its size is never read; 0 has no case in the
    # switch and falls through to the default, which does nothing.
    table = {-1: ('ANM_EXIT_HIDE', []), 0: ('ANM_UNUSED_0', [])}

    unknown = []
    for opcode in sorted(names):
        if opcode < 1:
            continue
        name = names[opcode]
        body = bodies.get(name, '')
        kinds = argument_kinds(name, body, None)

        if kinds is None:
            unknown.append(name)
            continue
        table[opcode] = (name, kinds)

    if unknown:
        print(f'no argument list could be derived for: {", ".join(unknown)}')
        return 1

    print(f'{len(table)} opcodes\n')
    print('OPCODES = {')
    for opcode in sorted(table):
        name, kinds = table[opcode]
        args = ', '.join(f"'{k}'" for k in kinds)
        note = f'  # {SPECIAL[name]}' if name in SPECIAL else ''
        print(f'    {opcode:3}: ({name!r}, [{args}]),{note}')
    print('}')
    return 0


if __name__ == '__main__':
    sys.exit(main())
