"""Write the entry descriptions, then read them back to be sure nothing is lost.

Layout of what is produced:

    anm/<name>/<entry>.toml      one entry: its texture, size, format and script offsets
    anm/<name>/scripts_<entry>.bin   the streams, already external from step 3
    anm/<name>/texture_<entry>.png   the texture, already external from step 1
    anm/<name>/sprites_<entry>.toml  the rectangles, already external from step 2

Fields written: texture, width, height, format, sprites, scripts, and the script offsets.
Fields dropped, with the reason:

    textureIdx        always 0 in the shipped data; the loader assigns the slot
    spriteIdxOffset   computed by the loader as entries are placed
    mipmapNameOffset  always 0; no entry has a mipmap
    version           always 2; rejected if anything else appeared
    hasData           always 0 since step 1
    ownsMemory        a lifetime flag, not content
    priority          read by nothing; ChainElem has its own
    nameOffset        the binary name is a file name, kept as a string here
    nextOffset        the entry list is the order of files in the directory
    textureOffset     an embedded-image offset, unused since step 1
    dataOffsets       the offset table, replaced by the script list below

The script offsets must be kept: streams differ in length, so where each one starts is not
derivable from anything else.
"""

import struct
import sys
from pathlib import Path

DATA = Path(r'D:\OpenSTG\build\run\th07.data')
ANM_DIR = DATA / 'anm'

NUM_SPRITES = 0x00
NUM_SCRIPTS = 0x04
WIDTH = 0x0C
HEIGHT = 0x10
FORMAT = 0x14
NAME_OFFSET = 0x1C
VERSION = 0x28
NEXT_OFFSET = 0x38
TABLE_START = 0x40

DROPPED = {
    'numSprites': 'sprites', 'numScripts': 'scripts',
}


def main() -> int:
    names = sorted(p.stem for p in DATA.glob('*.anm'))
    print(f'{len(names)} animation files\n')

    written = 0
    for name in names:
        data = (DATA / f'{name}.anm').read_bytes()

        starts = []
        offset = 0
        while True:
            starts.append(offset)
            next_offset = struct.unpack_from('<i', data, offset + NEXT_OFFSET)[0]
            if next_offset == 0:
                break
            offset += next_offset

        for index, start in enumerate(starts):
            num_sprites = struct.unpack_from('<i', data, start + NUM_SPRITES)[0]
            num_scripts = struct.unpack_from('<i', data, start + NUM_SCRIPTS)[0]
            width = struct.unpack_from('<i', data, start + WIDTH)[0]
            height = struct.unpack_from('<i', data, start + HEIGHT)[0]
            fmt = struct.unpack_from('<i', data, start + FORMAT)[0]
            version = struct.unpack_from('<i', data, start + VERSION)[0]

            # An entry with no image of its own keeps the "@" marker: the loader then creates
            # an empty texture of the stated size instead of opening a file. Writing a file
            # name for these asked for a PNG that was never produced.
            name_offset = struct.unpack_from('<i', data, start + NAME_OFFSET)[0]
            name_end = data.find(b'\0', start + name_offset)
            texture_name = data[start + name_offset:name_end]
            if texture_name == b'@':
                texture_value = '@'
            else:
                texture_value = f'texture_{index}.png'

            if version != 2:
                print(f'{name} entry {index}: version {version}, not 2')
                return 1

            lines = [
                '# One entry of an animation.',
                '#',
                '# The texture is a PNG beside this file and the streams are in',
                '# scripts_N.bin, so what is left here is the description: how big the texture',
                '# is, how the loader should read it, and where each stream begins.',
                f'texture = "{texture_value}"',
                f'width = {width}',
                f'height = {height}',
                f'format = {fmt}',
                f'sprites = {num_sprites}',
                f'scripts = {num_scripts}',
            ]

            if num_scripts:
                table = start + TABLE_START
                pairs = struct.unpack_from(f'<{num_scripts * 2}i', data, table)
                lines.append('')
                lines.append('# Where each stream starts inside scripts_N.bin, and the index the')
                lines.append('# game uses to run it.')
                for script_index, script_offset in zip(pairs[0::2], pairs[1::2]):
                    lines.append('')
                    lines.append('[[script]]')
                    lines.append(f'index = {script_index}')
                    lines.append(f'offset = {script_offset}')

            target = ANM_DIR / name / f'{index}.toml'
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text('\n'.join(lines) + '\n', encoding='utf-8')
            written += 1

    print(f'wrote {written} entry descriptions')
    return 0


if __name__ == '__main__':
    sys.exit(main())
