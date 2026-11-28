#!/usr/bin/env python3
"""Build Lua 5.4 as a static library into .tools/static/.

    python tools/build_lua_static.py

Lua ships a Makefile for gcc, not a CMake project, so this invokes the compiler directly
rather than driving a build system. The result is a single archive and a header directory
staged beside the SDL ones, so the game links against it the same way.

Reads the source from D:/lua-5.4.7, which is not part of this project.

Two files are left out: lua.c and luac.c are the standalone interpreter and compiler, each
with its own main(). The game embeds the library and calls into it, so neither belongs.
"""

from __future__ import annotations

import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
STAGE = ROOT / ".tools/static"
BUILD = ROOT / ".tools/lua-build"

LUA_SRC = Path(r"D:/lua-5.4.7/src")

# The sources that make up the library. Everything in src/ except the two programs.
EXCLUDED = {"lua.c", "luac.c"}

CL = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\cl.exe")
LIB = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86\lib.exe")


def find_environment() -> tuple[dict[str, str], str] | None:
    """INCLUDE and LIB for a 32-bit build, with the SDK that actually carries x86 libs."""
    import os

    if not CL.is_file():
        print(f"compiler not found: {CL}")
        return None

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
        print("no Windows SDK with 32-bit libraries found")
        return None

    env = os.environ.copy()
    env["INCLUDE"] = ";".join([
        str(msvc / "include"),
        str(sdk_root / "Include" / sdk / "ucrt"),
        str(sdk_root / "Include" / sdk / "shared"),
        str(sdk_root / "Include" / sdk / "um"),
    ])
    env["LIB"] = ";".join([
        str(msvc / "lib/x86"),
        str(sdk_root / "Lib" / sdk / "ucrt/x86"),
        str(sdk_root / "Lib" / sdk / "um/x86"),
    ])
    return env, sdk


def main() -> int:
    if not (LUA_SRC / "lua.h").is_file():
        print(f"Lua source not found: {LUA_SRC}")
        print("Put a Lua 5.4 release there, with its src/ directory intact.")
        return 1

    environment = find_environment()
    if environment is None:
        return 1
    env, sdk = environment
    print(f"using Windows SDK {sdk}")

    if BUILD.exists():
        shutil.rmtree(BUILD)
    BUILD.mkdir(parents=True)

    sources = sorted(p for p in LUA_SRC.glob("*.c") if p.name not in EXCLUDED)
    print(f"compiling {len(sources)} source files")

    objects = []
    for source in sources:
        command = [
            str(CL), "/nologo", "/c", "/O2", "/MT", "/TC",
            # Lua is C89 and builds cleanly with these; /W1 keeps the output readable since
            # the sources are not ours to fix.
            "/W1",
            f"/I{LUA_SRC}",
            f"/Fo{BUILD / (source.stem + '.obj')}",
            str(source),
        ]
        result = subprocess.run(command, env=env, capture_output=True, text=True,
                                errors="replace")
        if result.returncode != 0:
            print(f"failed: {source.name}")
            print(result.stdout[-3000:])
            print(result.stderr[-3000:])
            return 1
        objects.append(BUILD / (source.stem + ".obj"))

    print(f"archiving {len(objects)} objects")

    archive = STAGE / "lib/lua54-static.lib"
    archive.parent.mkdir(parents=True, exist_ok=True)
    if archive.exists():
        archive.unlink()

    command = [str(LIB), "/nologo", f"/OUT:{archive}"] + [str(o) for o in objects]
    result = subprocess.run(command, env=env, capture_output=True, text=True,
                            errors="replace")
    if result.returncode != 0:
        print(result.stdout[-3000:])
        print(result.stderr[-3000:])
        return 1

    # Header directory, laid out so the game can #include <lua/lua.hpp> or "lua.h" the same
    # way it includes SDL's headers.
    header_target = STAGE / "include/lua"
    if header_target.exists():
        shutil.rmtree(header_target)
    header_target.mkdir(parents=True)
    for header in LUA_SRC.glob("*.h"):
        shutil.copy2(header, header_target / header.name)

    print(f"\nstaged {archive.relative_to(ROOT)} "
          f"({archive.stat().st_size / 1024:.0f} KiB)")
    print(f"headers in {header_target.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
