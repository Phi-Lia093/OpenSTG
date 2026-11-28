"""Check that the TOML reader handles arrays of tables.

The sprite table needs [[sprite]] elements, which the reader did not support: it read flat
tables for the configuration file and reported anything else as an error. Extending it is
small but easy to get wrong -- a key collected with the wrong prefix, or an element whose
values land in the parent document, both look fine until a reader asks for them.

Usage: python tools/test_toml.py
"""

import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / '.tools/toml-test'
CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")
STAGE = ROOT / '.tools/static'

PROGRAM = r'''
#include <cstdio>
#include <cstring>

#include "data/Toml.hpp"

static int g_failures = 0;

static void check(bool condition, const char *what)
{
    if (!condition)
    {
        printf("FAIL: %s\n", what);
        g_failures++;
    }
}

int main()
{
    const std::string text =
        "version = 2\n"
        "width = 256\n"
        "\n"
        "[[sprite]]\n"
        "id = 0\n"
        "rect = [0, 0, 8, 8]\n"
        "\n"
        "[[sprite]]\n"
        "id = 1\n"
        "rect = [8, 0, 8, 8]\n"
        "\n"
        "[texture]\n"
        "name = \"a.png\"\n";

    Toml::Document document;
    std::string error;
    check(Toml::Parse(text, document, error), "the document parses");
    if (g_failures)
    {
        printf("  error: %s\n", error.c_str());
        return 1;
    }

    // Scalars at the top level.
    const Toml::Value *version = document.Get("version");
    check(version && version->integer == 2, "top-level integer");

    // The array of tables.
    const std::vector<Toml::Document> *sprites = document.Tables("sprite");
    check(sprites != nullptr, "sprite is an array of tables");
    if (!sprites)
    {
        printf("%d failure(s)\n", g_failures);
        return 1;
    }
    check(sprites->size() == 2, "two sprite elements");

    // Values inside an element are flat: asking for "rect" must find it, not "sprite.rect".
    const Toml::Value *rect = (*sprites)[0].Get("rect");
    check(rect && rect->integers.size() == 4, "first element has a four-value rect");
    if (rect && rect->integers.size() == 4)
    {
        check(rect->integers[0] == 0 && rect->integers[2] == 8, "rect values");
    }

    const Toml::Value *second = (*sprites)[1].Get("id");
    check(second && second->integer == 1, "second element has its own id");

    // A plain table still works, with its keys prefixed as before.
    const Toml::Value *name = document.Get("texture.name");
    check(name && name->text == "a.png", "plain table key");

    // Missing keys report absence rather than inventing a value.
    check(document.Get("nothing") == nullptr, "absent key is null");
    check(document.Tables("version") == nullptr, "a scalar is not a table array");

    if (g_failures == 0)
    {
        printf("all toml tests passed\n");
        return 0;
    }
    printf("%d failure(s)\n", g_failures);
    return 1;
}
'''


def main() -> int:
    BUILD.mkdir(parents=True, exist_ok=True)
    (BUILD / 'main.cpp').write_text(PROGRAM, encoding='utf-8')

    msvc = CL.parents[3]
    sdk_root = Path(r"C:\Program Files (x86)\Windows Kits\10")
    candidates = [p for p in (sdk_root / 'Include').iterdir() if (p / 'um/windows.h').is_file()]
    sdk = None
    for include_dir in sorted(candidates, reverse=True):
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

    command = [
        str(CL), '/nologo', '/MT', '/EHsc', '/std:c++17',
        str(ROOT / 'src/data/Toml.cpp'),
        str(BUILD / 'main.cpp'),
        '/I' + str(ROOT / 'src'),
        '/I' + str(STAGE / 'include'),
        '/Fe:' + str(BUILD / 'toml_test.exe'),
        '/Fo' + str(BUILD) + '\\',
    ]
    result = subprocess.run(command, env=env, capture_output=True, text=True, errors='replace')
    if result.returncode != 0:
        print(result.stdout[-3000:])
        print(result.stderr[-3000:])
        return 1

    run = subprocess.run([str(BUILD / 'toml_test.exe')], capture_output=True, text=True,
                         errors='replace')
    print(run.stdout)
    return run.returncode


if __name__ == '__main__':
    sys.exit(main())
