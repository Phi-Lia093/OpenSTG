"""Parse every generated animation.lua with a real Lua interpreter.

Text that looks right is not the same as Lua that compiles. This loads each file in a fresh
state and checks the shape the loader depends on: a table of entries, each with a texture and
the arrays it needs, and each stream a function.

Usage: python tools/check_lua_syntax.py
"""

import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / '.tools/lua-check'
CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")
STAGE = ROOT / '.tools/static'

PROGRAM = r'''
#include <stdio.h>
#include <string.h>

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

static int check(const char *path)
{
    lua_State *L = luaL_newstate();
    /* No standard libraries: the description must not depend on them, and running an
       animation file should not be able to reach the file system. */
    if (luaL_loadfile(L, path) != LUA_OK)
    {
        printf("%s: %s\n", path, lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
    {
        printf("%s: %s\n", path, lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    if (!lua_istable(L, -1))
    {
        printf("%s: not a table\n", path);
        lua_close(L);
        return 1;
    }

    int entries = 0, streams = 0, functions = 0;
    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        if (lua_type(L, -2) == LUA_TNUMBER && lua_istable(L, -1))
        {
            entries++;
            lua_getfield(L, -1, "texture");
            if (!lua_isstring(L, -1))
            {
                printf("%s: entry %d has no texture\n", path, entries);
                lua_close(L);
                return 1;
            }
            lua_pop(L, 1);

            lua_pushnil(L);
            while (lua_next(L, -2) != 0)
            {
                if (lua_type(L, -2) == LUA_TNUMBER && lua_type(L, -1) == LUA_TFUNCTION)
                {
                    functions++;
                }
                else if (lua_type(L, -2) == LUA_TNUMBER && lua_istable(L, -1))
                {
                    lua_getfield(L, -1, "offset");
                    if (lua_isnumber(L, -1))
                    {
                        streams++;
                    }
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
    }

    lua_close(L);
    printf("ok %s: %d entries, %d streams, %d functions\n", path, entries, streams, functions);
    return 0;
}

int main(int argc, char **argv)
{
    int failures = 0;
    for (int i = 1; i < argc; i++)
    {
        failures += check(argv[i]);
    }
    return failures ? 1 : 0;
}
'''


def main() -> int:
    BUILD.mkdir(parents=True, exist_ok=True)
    (BUILD / 'main.cpp').write_text(PROGRAM, encoding='utf-8')

    msvc = CL.parents[3]
    sdk_root = Path(r"C:\Program Files (x86)\Windows Kits\10")
    sdk = None
    for include_dir in sorted((sdk_root / 'Include').iterdir(), reverse=True):
        if (sdk_root / 'Lib' / include_dir.name / 'um/x86/kernel32.lib').is_file():
            sdk = include_dir.name
            break
    if sdk is None:
        print('no Windows SDK with 32-bit libraries')
        return 1

    env = os.environ.copy()
    env['INCLUDE'] = ';'.join([
        str(msvc / 'include'),
        str(sdk_root / 'Include' / sdk / 'ucrt'),
        str(sdk_root / 'Include' / sdk / 'shared'),
        str(sdk_root / 'Include' / sdk / 'um'),
        str(STAGE / 'include'),
    ])
    env['LIB'] = ';'.join([
        str(msvc / 'lib/x86'),
        str(sdk_root / 'Lib' / sdk / 'ucrt/x86'),
        str(sdk_root / 'Lib' / sdk / 'um/x86'),
        str(STAGE / 'lib'),
    ])

    result = subprocess.run([
        str(CL), '/nologo', '/MT', '/EHsc', '/std:c++17',
        str(BUILD / 'main.cpp'),
        '/I' + str(STAGE / 'include'),
        '/Fe:' + str(BUILD / 'lua_check.exe'),
        '/Fo' + str(BUILD) + '\\',
        '/link', 'lua54-static.lib',
    ], env=env, capture_output=True, text=True, errors='replace')
    if result.returncode != 0:
        print(result.stdout[-3000:])
        print(result.stderr[-3000:])
        return 1

    files = sorted(Path(ROOT / 'build/run/th07.data/anm').glob('*/animation.lua'))
    run = subprocess.run([str(BUILD / 'lua_check.exe')] + [str(f) for f in files],
                         capture_output=True, text=True, errors='replace')

    lines = run.stdout.splitlines()
    failures = [line for line in lines if not line.startswith('ok ')]
    if failures:
        print(f'{len(failures)} file(s) failed:')
        for line in failures[:15]:
            print(f'  {line}')
        return 1

    totals = {'entries': 0, 'streams': 0, 'functions': 0}
    for line in lines:
        # "ok <path>: 3 entries, 15 streams, 15 functions"
        for key in totals:
            match = re.search(r'(\d+) ' + key, line)
            if match:
                totals[key] += int(match.group(1))

    print(f'{len(lines)} animation files parse')
    print(f'  {totals["entries"]} entries, {totals["streams"]} script offsets, '
          f'{totals["functions"]} stream functions')
    return 0


if __name__ == '__main__':
    sys.exit(main())
