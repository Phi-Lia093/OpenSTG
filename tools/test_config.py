"""Exercise the TOML subset and the config layer against the compiled game.

Builds a tiny test program that links the two translation units and checks:

  - parsing of every value type the subset claims
  - comments, blank lines, table headers
  - a malformed line reporting a line number
  - serialize -> parse -> serialize producing the same text
  - out-of-range values clamping rather than discarding the file

Usage: python tools/test_config.py
"""

from __future__ import annotations

import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / ".tools/config-test"
STATIC = ROOT / ".tools/static"

CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")


TEST_MAIN = r'''
#include <cstdio>
#include <cstring>
#include <string>

#include "Toml.hpp"
#include "ConfigFile.hpp"

static int g_failures = 0;

static void check(bool condition, const char *what)
{
    if (!condition)
    {
        printf("FAIL: %s\n", what);
        g_failures++;
    }
}

static void checkInt(const Toml::Document &doc, const char *key, long long expected)
{
    const Toml::Value *v = doc.Get(key);
    if (!v || v->type != Toml::ValueType::Integer || v->integer != expected)
    {
        printf("FAIL: %s should be %lld\n", key, expected);
        g_failures++;
    }
}

static void testParsing()
{
    const std::string text =
        "# a comment\n"
        "\n"
        "[table]\n"
        "number = 42          # trailing comment\n"
        "negative = -7\n"
        "flag = true\n"
        "off = false\n"
        "name = \"hello\"\n"
        "escaped = \"a\\tb\\nc\\\"d\"\n"
        "unicode = \"\\u00e9\"\n"
        "ratio = 1.5\n"
        "list = [1, 2, 3]\n"
        "words = [\"a\", \"b\"]\n"
        "\n"
        "[other]\n"
        "number = 5\n";

    Toml::Document doc;
    std::string error;
    check(Toml::Parse(text, doc, error), "valid document should parse");
    if (g_failures)
    {
        printf("  error was: %s\n", error.c_str());
        return;
    }

    checkInt(doc, "table.number", 42);
    checkInt(doc, "table.negative", -7);
    check(doc.Get("table.flag") && doc.Get("table.flag")->boolean, "flag is true");
    check(doc.Get("table.off") && !doc.Get("table.off")->boolean, "off is false");
    check(doc.Get("table.name") && doc.Get("table.name")->text == "hello", "plain string");
    check(doc.Get("table.escaped") &&
              doc.Get("table.escaped")->text == std::string("a\tb\nc\"d"),
          "escapes");
    check(doc.Get("table.unicode") &&
              doc.Get("table.unicode")->text == std::string("\xc3\xa9"),
          "unicode escape is UTF-8");
    check(doc.Get("table.ratio") && doc.Get("table.ratio")->type == Toml::ValueType::Float,
          "float");
    check(doc.Get("table.list") && doc.Get("table.list")->integers.size() == 3 &&
              doc.Get("table.list")->integers[2] == 3,
          "integer array");
    check(doc.Get("table.words") && doc.Get("table.words")->strings.size() == 2,
          "string array");
    checkInt(doc, "other.number", 5);
}

static void testErrors()
{
    Toml::Document doc;
    std::string error;

    check(!Toml::Parse("key = \n", doc, error), "missing value is an error");
    check(error.find("line 1") != std::string::npos, "error names the line");

    check(!Toml::Parse("[unclosed\n", doc, error), "unterminated table is an error");
    check(!Toml::Parse("no equals here\n", doc, error), "line without = is an error");
    check(!Toml::Parse("key = oops\n", doc, error), "bare word is an error");
    check(!Toml::Parse("key = \"unterminated\n", doc, error), "unterminated string");
}

static void testConfigRoundTrip()
{
    GameConfiguration config;
    memset(&config, 0, sizeof(config));
    config.lifeCount = 3;
    config.bombCount = 2;
    config.defaultDifficulty = 2;
    config.effectQuality = 1;
    config.musicMode = 1;
    config.windowed = 1;
    config.frameskipConfig = 1;
    config.disableFog = 1;
    config.disableVsync = 0;
    config.padAxisX = 500;
    config.padAxisY = 400;

    const std::string first = ConfigFile::Serialize(config);
    check(first.find("[input]") != std::string::npos, "output has an [input] table");
    check(first.find("lives ") != std::string::npos, "lives written");
    check(first.find("= 3") != std::string::npos, "lives value written");
    check(first.find("\"hard\"") != std::string::npos, "difficulty written by name");

    GameConfiguration loaded;
    memset(&loaded, 0, sizeof(loaded));
    loaded.lifeCount = 9; // a sentinel: the file must overwrite it
    check(ConfigFile::LoadText(first.c_str(), loaded), "serialized text parses");

    check(loaded.lifeCount == 3, "lives survived the round trip");
    check(loaded.bombCount == 2, "bombs survived");
    check(loaded.defaultDifficulty == 2, "difficulty survived");
    check(loaded.effectQuality == 1, "quality survived");
    check(loaded.musicMode == 1, "music mode survived");
    check(loaded.windowed == 1, "windowed survived");
    check(loaded.frameskipConfig == 1, "frameskip survived");
    check(loaded.disableFog == 1, "disable_fog survived");
    check(loaded.disableVsync == 0, "disable_vsync survived");
    check(loaded.padAxisX == 500, "pad_axis_x survived");

    const std::string second = ConfigFile::Serialize(loaded);
    check(first == second, "serialize is stable");
}

static void testClamping()
{
    // An out-of-range value must clamp, not discard the rest of the file.
    const std::string text =
        "[game]\n"
        "lives = 99\n"
        "bombs = 2\n";

    GameConfiguration config;
    memset(&config, 0, sizeof(config));
    config.lifeCount = 1;
    config.bombCount = 1;
    ConfigFile::LoadText(text.c_str(), config);

    check(config.lifeCount == 4, "lives clamped to the maximum");
    check(config.bombCount == 2, "a later good value still applies");
}

static void testPartialFile()
{
    // Absent keys keep their existing value.
    const std::string text = "[game]\nlives = 4\n";
    GameConfiguration config;
    memset(&config, 0, sizeof(config));
    config.lifeCount = 1;
    config.bombCount = 3;
    ConfigFile::LoadText(text.c_str(), config);
    check(config.lifeCount == 4, "present key applied");
    check(config.bombCount == 3, "absent key left alone");
}

int main()
{
    testParsing();
    testErrors();
    testConfigRoundTrip();
    testClamping();
    testPartialFile();

    if (g_failures == 0)
    {
        printf("all config tests passed\n");
        return 0;
    }
    printf("%d failure(s)\n", g_failures);
    return 1;
}
'''

TEST_HPP_ADDITION = '''
// Parses `text` in place of a file. Used by the tests.
bool LoadText(const char *text, GameConfiguration &config);
'''


def main() -> int:
    if not CL.is_file():
        print(f"compiler not found: {CL}")
        return 1
    if not (STATIC / 'lib/SDL3-static.lib').is_file():
        print("static SDL not built; run tools/build_sdl_static.py first")
        return 1

    if BUILD.exists():
        shutil.rmtree(BUILD)
    BUILD.mkdir(parents=True)

    (BUILD / 'test_main.cpp').write_text(TEST_MAIN, encoding='utf-8')

    sources = [
        str(ROOT / 'src/Toml.cpp'),
        str(ROOT / 'src/ConfigFile.cpp'),
        str(BUILD / 'test_main.cpp'),
    ]

    source_dir = ROOT / 'src'
    compile_cmd = [
        str(CL), '/nologo', '/MT', '/EHsc', '/std:c++17', '/O2',
        '-DTH_CONFIG_TEST',
        f'/I{source_dir}',
        # ConfigFile.hpp reaches Supervisor.hpp, which includes SDL headers. Only the
        # headers are needed; nothing from SDL is linked.
        f'/I{STATIC / "include"}',
        *sources,
        '/Fe:' + str(BUILD / 'test.exe'),
        '/Fo' + str(BUILD) + '\\\\',
    ]

    print(' '.join(compile_cmd))
    # cl needs the MSVC and Windows SDK header paths; the build script sets INCLUDE, so this
    # one does too rather than assuming an already-prepared shell. The SDK is picked by
    # whether it actually carries 32-bit libraries: an SDK install can include headers for
    # a version whose x86 libs were never installed.
    env = os.environ.copy()
    msvc_root = CL.parents[3]
    sdk_root = Path(r"C:\Program Files (x86)\Windows Kits\10")
    candidates = [p for p in (sdk_root / 'Include').iterdir() if (p / 'um/windows.h').is_file()]
    sdk = None
    for include_dir in sorted(candidates, reverse=True):
        version = include_dir.name
        if (sdk_root / 'Lib' / version / 'um/x86/kernel32.lib').is_file():
            sdk = version
            break
    if sdk is None:
        print('no Windows SDK with 32-bit libraries found')
        return 1

    env['INCLUDE'] = ';'.join([
        str(msvc_root / 'include'),
        str(sdk_root / 'Include' / sdk / 'ucrt'),
        str(sdk_root / 'Include' / sdk / 'shared'),
        str(sdk_root / 'Include' / sdk / 'um'),
    ])
    env['LIB'] = ';'.join([
        str(msvc_root / 'lib/x86'),
        str(sdk_root / 'Lib' / sdk / 'ucrt/x86'),
        str(sdk_root / 'Lib' / sdk / 'um/x86'),
    ])
    print(f'using Windows SDK {sdk}')

    result = subprocess.run(compile_cmd, env=env, capture_output=True, text=True,
                            errors='replace')
    print(result.stdout[-4000:])
    if result.returncode != 0:
        print(result.stderr[-4000:])
        return 1

    run = subprocess.run([str(BUILD / 'test.exe')], capture_output=True, text=True,
                         errors='replace')
    print(run.stdout)
    if run.stderr:
        print(run.stderr[-2000:])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
