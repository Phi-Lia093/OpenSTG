"""Report exactly what an ANM file contains, part by part.

The point is to separate the parts that were previously replaced all at once. Each has its
own representation and can be swapped independently:

  texture    pixels, embedded in the file
  sprites    rectangles into that texture
  scripts    the instruction streams
  header     the fields that describe the above
"""

import struct
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')


def main(name: str) -> int:
    path = DATA / f'{name}.anm'
    data = path.read_bytes()

    total_sprites = 0
    total_scripts = 0
    total_instructions = 0
    embedded = 0
    external = 0

    print(f'{name}.anm: {len(data)} bytes\n')
    print(f'{"entry":>5} {"sprites":>7} {"scripts":>7} {"size":>10} {"texture":>9} {"next":>10}')
    print('-' * 56)

    offset = 0
    index = 0
    while True:
        fields = struct.unpack_from('<13i', data, offset)
        num_sprites, num_scripts = fields[0], fields[1]
        width, height = fields[3], fields[4]
        name_offset, texture_offset = fields[7], fields[12]
        has_data = data[offset + 0x34]
        next_offset = struct.unpack_from('<i', data, offset + 0x38)[0]

        name_end = data.find(b'\0', offset + name_offset)
        texture_name = data[offset + name_offset:name_end].decode('ascii', 'replace')

        if has_data:
            embedded += 1
            texture_desc = f'{width}x{height}'
        else:
            external += 1
            texture_desc = 'external'

        print(f'{index:5} {num_sprites:7} {num_scripts:7} '
              f'{texture_offset:10} {texture_desc:>9} {next_offset:10}')
        print(f'      name: {texture_name}')

        total_sprites += num_sprites
        total_scripts += num_scripts

        if next_offset == 0:
            break
        offset += next_offset
        index += 1

    print(f'\n{index + 1} entries: {total_sprites} sprites, {total_scripts} scripts')
    print(f'textures: {embedded} embedded, {external} external')

    # The parts, and what each would become.
    print('\nwhat each part is, and what replacing it alone would mean:')
    print('  header   fixed fields + the offset tables          -> stays binary or TOML')
    print('  texture  pixels in THTX form, or a file name       -> PNG beside the file')
    print('  sprites  (id, offset, size) per sprite             -> TOML rects')
    print('  scripts  instruction streams, variable length      -> binary side file, then text')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1] if len(sys.argv) > 1 else 'title01'))
