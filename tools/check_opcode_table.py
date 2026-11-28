"""Check the derived opcode table against the instruction sizes in the shipped data.

Every instruction records its own size, and that size must be 8 + 4 * (argument count). A table
whose argument count is wrong therefore disagrees with the data, which makes this comparison a
direct test of the table rather than a restatement of it.

Zero-size instructions are skipped: opcode -1 ends a stream on the opcode alone and never has
its size read.
"""

import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ANM = ROOT / 'build/run/th07.data/anm'
TABLE = ROOT / '.tools/opcodes.txt'
OFFSET = re.compile(r'^offset = (\d+)$', re.M)
ROW = re.compile(r"(-?\d+): \('(\w+)', \[([^\]]*)\]\)")


def load_table() -> dict[int, tuple[str, int]]:
    text = TABLE.read_text(encoding='utf-8')
    table = {}
    for match in ROW.finditer(text):
        opcode = int(match.group(1))
        name = match.group(2)
        kinds = [part.strip().strip("'") for part in match.group(3).split(',') if part.strip()]
        table[opcode] = (name, len(kinds))
    return table


def main() -> int:
    table = load_table()
    print(f'{len(table)} opcodes in the table')

    mismatches: dict[tuple[int, str, int, int], int] = {}
    unknown: dict[int, int] = {}
    checked = 0

    for directory in sorted(p for p in ANM.iterdir() if p.is_dir()):
        for script_file in sorted(directory.glob('*.bin')):
            data = script_file.read_bytes()
            entry = script_file.stem.replace('scripts_', '')
            description = directory / f'{entry}.toml'
            offsets = [int(m) for m in OFFSET.findall(description.read_text(encoding='utf-8'))]

            for offset in sorted(offsets):
                position = offset
                while position + 8 <= len(data):
                    opcode, size, time, flags = struct.unpack_from('<hHhH', data, position)
                    checked += 1

                    if opcode not in table:
                        unknown[opcode] = unknown.get(opcode, 0) + 1
                    else:
                        name, count = table[opcode]
                        expected = 8 + 4 * count
                        if size != 0 and size != expected:
                            key = (opcode, name, size, expected)
                            mismatches[key] = mismatches.get(key, 0) + 1

                    if opcode in (-1, 1, 2, 23) or size < 8:
                        break
                    position += size

    print(f'{checked} instructions checked')

    if unknown:
        print('\nopcodes not in the table:')
        for opcode, count in sorted(unknown.items()):
            print(f'  {opcode}: {count}')
        return 1

    if mismatches:
        print('\nsize disagreements:')
        for (opcode, name, size, expected), count in sorted(mismatches.items())[:15]:
            print(f'  {name} ({opcode}): data says {size}, table implies {expected} (x{count})')
        return 1

    print('\nevery instruction agrees with the table')
    return 0


if __name__ == '__main__':
    sys.exit(main())
