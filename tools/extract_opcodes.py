"""Extract, for every opcode, how the interpreter reads its arguments.

The Lua host must reproduce the original byte layout exactly, and that depends on which
member of the AnyArg union each argument is read through: args[n].i for an integer,
.f for a float, .b[k] for one byte of one. Guessing here would produce a stream that loads
without complaint and animates wrongly.

This reads the interpreter's switch and reports, per opcode, the accessor used for each
argument, so the host's signatures can be written from evidence rather than memory.
"""

import re
from pathlib import Path

SOURCE = Path('src/script/AnmManager.cpp')

# Accessors the interpreter defines, in order of specificity.
PATTERNS = [
    (re.compile(r'GET_INT_VALUE\((\d+)\)'), 'int'),
    (re.compile(r'GET_FLOAT_VALUE\((\d+)\)'), 'float'),
    (re.compile(r'GET_INT_PTR\((\d+)\)'), 'intptr'),
    (re.compile(r'GET_FLOAT_PTR\((\d+)\)'), 'floatptr'),
    (re.compile(r'args\[(\d+)\]\.i'), 'int'),
    (re.compile(r'args\[(\d+)\]\.f'), 'float'),
    (re.compile(r'args\[(\d+)\]\.b\[(\d+)\]'), 'byte'),
    (re.compile(r'args\[(\d+)\]\.u'), 'uint'),
]


def cases(text: str):
    """Yield (opcode name, body) for each case in the instruction switch."""
    start = text.find('switch (instr->opcode)')
    depth = 0
    end = start
    while end < len(text):
        if text[end] == '{':
            depth += 1
        elif text[end] == '}':
            depth -= 1
            if depth == 0:
                break
        end += 1
    block = text[start:end]

    # Split on `case X:` markers; a body runs until the next case or the block ends.
    positions = [(m.start(), m.group(1)) for m in re.finditer(r'case (ANM_\w+):', block)]
    for index, (offset, name) in enumerate(positions):
        stop = positions[index + 1][0] if index + 1 < len(positions) else len(block)
        yield name, block[offset:stop]


def main() -> int:
    text = SOURCE.read_text(encoding='utf-8')

    rows = []
    for name, body in cases(text):
        found = {}
        for pattern, kind in PATTERNS:
            for match in pattern.finditer(body):
                index = int(match.group(1))
                found.setdefault(index, kind)
        if found:
            signature = ', '.join(f'{found[i]}:{i}' for i in sorted(found))
        else:
            signature = '(none)'
        rows.append((name, signature))

    print(f'{len(rows)} opcode cases\n')
    print(f'{"opcode":32} {"arguments"}')
    print('-' * 70)
    for name, signature in rows:
        print(f'{name:32} {signature}')

    # Group by signature so the host can be written from a handful of shapes rather than
    # 81 individual cases.
    from collections import defaultdict
    grouped = defaultdict(list)
    for name, signature in rows:
        grouped[signature].append(name)

    print(f'\n{len(grouped)} distinct argument shapes:')
    for signature, names in sorted(grouped.items(), key=lambda kv: -len(kv[1])):
        print(f'  {signature:28} x{len(names):2}  {", ".join(n.replace("ANM_", "") for n in names[:6])}'
              + (' ...' if len(names) > 6 else ''))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
