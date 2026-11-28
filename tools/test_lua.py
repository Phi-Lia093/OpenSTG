"""Prove the Lua embedding works before building anything on it.

The whole ANM plan rests on one mechanism: a Lua script, run once at load, calls C++
functions that append instructions to a buffer. If that mechanism is unsound -- if the
calling conventions are wrong, or the buffer does not survive, or the library is missing
something -- everything built on top of it fails later and expensively.

This builds a small program that:

  * creates a Lua state
  * exposes a `record(opcode, time, ...)` function that appends to a vector
  * runs a script calling it
  * prints what was collected, and checks it against an expected list

It is intentionally not part of the game; it is the feasibility check the plan calls for.
"""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / ".tools/lua-test"
STAGE = ROOT / ".tools/static"

CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")

PROGRAM = r'''
// Lua is a C library. Its headers declare their functions with C linkage only when they are
// compiled as C, so a C++ caller has to say so, or the symbols come out mangled and the link
// fails on every one of them.
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <cstdio>
#include <cstring>
#include <vector>

// One instruction as the VM will see it: the opcode, and a time. The real host also stores
// flags and four bytes per argument; those are exercised in the real thing.
struct Recorded
{
    int opcode;
    int time;
    double arg0;
    double arg1;
};

static std::vector<Recorded> g_records;

// record(opcode, time, [arg0], [arg1])
//
// Called with a colon, as `a:record(...)`, so the script table arrives as argument 1 and
// the caller's arguments start at 2. The colon form is what the generated scripts will use,
// so the offset is established here rather than discovered later.
static int l_record(lua_State *L)
{
    Recorded entry{};
    entry.opcode = (int)luaL_checkinteger(L, 2);
    entry.time = (int)luaL_checkinteger(L, 3);
    entry.arg0 = luaL_optnumber(L, 4, 0.0);
    entry.arg1 = luaL_optnumber(L, 5, 0.0);
    g_records.push_back(entry);
    return 0;
}

// A table with named methods, the way anm.script(0) will return one.
static const luaL_Reg kMethods[] = {
    {"record", l_record},
    {NULL, NULL},
};

static int l_anm_script(lua_State *L)
{
    luaL_checkinteger(L, 1);
    lua_newtable(L);
    luaL_setfuncs(L, kMethods, 0);
    return 1;
}

int main()
{
    lua_State *L = luaL_newstate();
    if (!L)
    {
        printf("FAIL: luaL_newstate returned null\n");
        return 1;
    }
    luaL_openlibs(L);

    lua_newtable(L);
    lua_pushcfunction(L, l_anm_script);
    lua_setfield(L, -2, "script");
    lua_setglobal(L, "anm");

    const char *script =
        "local a = anm.script(0)\n"
        "a:record(3, 0, 0)\n"
        "a:record(22, 0)\n"
        "a:record(8, 0, 0)\n"
        "a:record(17, 60, 400.0, 200.0)\n"
        "a:record(18, 256, 409.6)\n";

    if (luaL_loadstring(L, script) != LUA_OK || lua_pcall(L, 0, 0, 0) != LUA_OK)
    {
        printf("FAIL: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }
    lua_close(L);

    struct Expected
    {
        int opcode;
        int time;
        double arg0;
    };
    const Expected expected[] = {
        {3, 0, 0.0},
        {22, 0, 0.0},
        {8, 0, 0.0},
        {17, 60, 400.0},
        {18, 256, 409.6},
    };
    const int expectedCount = 5;

    printf("collected %zu instruction(s), expected %d\n", g_records.size(), expectedCount);
    for (size_t i = 0; i < g_records.size(); i++)
    {
        printf("  [%zu] opcode=%d time=%d arg0=%g arg1=%g\n", i, g_records[i].opcode,
               g_records[i].time, g_records[i].arg0, g_records[i].arg1);
    }

    if ((int)g_records.size() != expectedCount)
    {
        printf("FAIL: wrong count\n");
        return 1;
    }
    for (int i = 0; i < expectedCount; i++)
    {
        if (g_records[i].opcode != expected[i].opcode ||
            g_records[i].time != expected[i].time ||
            g_records[i].arg0 != expected[i].arg0)
        {
            printf("FAIL: instruction %d differs\n", i);
            return 1;
        }
    }

    // A float that must survive the trip exactly, since animation positions depend on it.
    if (g_records[4].arg0 != 409.6)
    {
        printf("FAIL: 409.6 did not round-trip\n");
        return 1;
    }

    printf("PASS: Lua drove the C++ collector and the values round-tripped\n");
    return 0;
}
'''


def find_environment():
    msvc = CL.parents[3]
    sdk_root = Path(r"C:\Program Files (x86)\Windows Kits\10")
    candidates = [p for p in (sdk_root / "Include").iterdir() if (p / "um/windows.h").is_file()]
    sdk = None
    for include_dir in sorted(candidates, reverse=True):
        version = include_dir.name
        if (sdk_root / "Lib" / version / "um/x86/kernel32.lib").is_file():
            sdk = version
            break
    if sdk is None:
        return None

    env = os.environ.copy()
    env["INCLUDE"] = ";".join([
        str(msvc / "include"),
        str(sdk_root / "Include" / sdk / "ucrt"),
        str(sdk_root / "Include" / sdk / "shared"),
        str(sdk_root / "Include" / sdk / "um"),
        str(STAGE / "include"),
    ])
    env["LIB"] = ";".join([
        str(msvc / "lib/x86"),
        str(sdk_root / "Lib" / sdk / "ucrt/x86"),
        str(sdk_root / "Lib" / sdk / "um/x86"),
        str(STAGE / "lib"),
    ])
    return env


def main() -> int:
    if not (STAGE / "lib/lua54-static.lib").is_file():
        print("Lua static library not built; run tools/build_lua_static.py first")
        return 1

    env = find_environment()
    if env is None:
        print("no usable Windows SDK")
        return 1

    BUILD.mkdir(parents=True, exist_ok=True)
    (BUILD / "main.cpp").write_text(PROGRAM, encoding="utf-8")

    command = [
        str(CL), "/nologo", "/MT", "/EHsc", "/std:c++17",
        str(BUILD / "main.cpp"),
        "/Fe:" + str(BUILD / "lua_test.exe"),
        "/Fo" + str(BUILD) + "\\\\",
        "/link", "lua54-static.lib",
        "user32.lib", "gdi32.lib", "winmm.lib", "ole32.lib", "advapi32.lib",
        "shell32.lib", "setupapi.lib", "oleaut32.lib", "uuid.lib", "version.lib",
    ]
    result = subprocess.run(command, env=env, capture_output=True, text=True,
                            errors="replace")
    print(result.stdout[-3000:])
    if result.returncode != 0:
        print(result.stderr[-3000:])
        return 1

    run = subprocess.run([str(BUILD / "lua_test.exe")], capture_output=True, text=True,
                         errors="replace")
    print(run.stdout)
    if run.stderr:
        print(run.stderr[-2000:])
    return run.returncode


if __name__ == "__main__":
    sys.exit(main())
