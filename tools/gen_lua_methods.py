"""Generate the Lua method table from the opcode enumeration.

The table maps a method name to the opcode number, which the instruction closure needs so that
AnmOp::Run can see the same number the binary form carried. Writing it by hand was wrong twice:
ADD_2 is 49 rather than 58, MOD_FLOAT_2 is 58 rather than 84, and the math group is shifted by
ten. Reading the enumeration removes the chance.

The two opcodes without enumerators -- 0 and 22 -- get names of their own: 0 does nothing and
22 sets the anchor bits.
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / 'src/script/AnmVm.hpp'


def main() -> int:
    text = HEADER.read_text(encoding='utf-8')
    start = text.find('ANM_EXIT_HIDE = -1')
    end = text.find('};', start)
    pairs = [(int(v), n) for n, v in re.findall(r'(ANM_\w+)\s*=\s*(-?\d+)', text[start:end])]
    pairs.sort()

    lines = []
    for value, name in pairs:
        method = name[4:] if name.startswith('ANM_') else name
        if method == '22':
            method = 'NOP22'
        lines.append(f'        {{"{method}", {value}}},')

    # Opcodes the enumeration has no entry for.
    lines.insert(0, '        {"NOP", 0},')

    print(f'{len(lines)} methods')
    print('\n'.join(lines))
    del sys
    return 0


if __name__ == '__main__':
    sys.exit(main())
