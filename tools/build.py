#!/usr/bin/env python3
"""Build th07.exe from src/ with the installed MSVC and the DirectX 8 SDK.

This is a plain compile-and-link. It does not compare anything against a retail
binary, does not consult a manifest, and does not verify tool identity: the goal is an
executable, and everything else can come later.

    python tools/build.py            # build build/th07.exe
    python tools/build.py --clean    # remove build products first

The compiler is whatever MSVC this machine has, invoked in its 32-bit form. The
DirectX 8 headers and libraries come from .tools/dx8sdk, extracted from the SDK this
project needs; that directory is not in git because it is a redistributable SDK rather
than source.
"""
from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
DX8 = ROOT / ".tools/dx8sdk"
OUT = ROOT / "build"
GENERATED = OUT / "generated"

# The MSVC that is installed here. The 32-bit compiler is what matters: the target is
# a Win32 executable and the sources assume the VC7-era 32-bit ABI.
MSVC_BIN = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86")
MSVC_ROOT = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207")
WINSDK_ROOT = Path(r"C:\Program Files (x86)\Windows Kits\10")

# The game's own sources. `legacy/` and `pbg4/` are internal split-outs of the same
# translation units and are included by the files below, not compiled separately.
SOURCES = [
    "AnmManager.cpp", "AsciiManager.cpp", "BombData.cpp", "BulletManager.cpp",
    "dsutil.cpp", "EclManager.cpp", "EffectManager.cpp", "Ending.cpp",
    "EnemyEclInstr.cpp", "EnemyManager.cpp", "GameManager.cpp", "Global.cpp",
    "Gui.cpp", "ItemManager.cpp", "main.cpp", "MainMenu.cpp",
    "MainMenuPreInput.cpp", "MidiOutput.cpp", "MusicRoom.cpp", "Player.cpp",
    "ReplayManager.cpp", "ResultRegister.cpp", "ResultScreen.cpp",
    "ResultScreenCtor.cpp", "ScreenEffect.cpp", "SoundPlayer.cpp", "Stage.cpp",
    "Supervisor.cpp", "TextHelper.cpp",
    "pbg4/Lzss.cpp", "pbg4/Pbg4Archive.cpp", "pbg4/Pbg4File.cpp",
]

# From the original link line. d3d8, d3dx8 and dxguid come from the SDK; the rest are
# Windows libraries.
LIBS = [
    "dinput8.lib", "user32.lib", "dsound.lib", "d3d8.lib", "d3dx8.lib",
    "dxguid.lib", "gdi32.lib", "winmm.lib", "ole32.lib",
]

# No optimisation flags beyond what the code needs: this is a build, not a match.
# /MT links the CRT statically, which the original does and which keeps the executable
# runnable without a redistributable.
FLAGS = [
    "/nologo", "/W3", "/MT", "/EHsc", "/Gr", "/Gy", "/Oi",
    "/DNDEBUG", "/DWIN32", "/D_WINDOWS",
    "/wd4060", "/wd4101", "/wd4244", "/wd4996",
]


def find_winsdk_paths() -> tuple[list[Path], list[Path]]:
    """Include and library directories for the Windows SDK, newest version first.

    The version is discovered rather than hardcoded: an installed SDK moves between
    releases and a stale path produces "cannot open windows.h", which reads like a
    broken toolchain instead of a wrong path.
    """
    base = WINSDK_ROOT
    if not base.is_dir():
        raise SystemExit(f"Windows SDK not found at {base}")

    def versions(kind: str) -> list[Path]:
        root = base / kind
        if not root.is_dir():
            return []
        found = [p for p in root.iterdir() if p.is_dir()]
        return sorted(found, key=lambda p: p.name, reverse=True)

    for version in versions("Include"):
        sub = version.name
        um = version / "um"
        if not (um / "windows.h").is_file():
            continue
        includes = []
        for name in ("ucrt", "um", "shared", "winrt", "cppwinrt"):
            path = version / name
            if path.is_dir():
                includes.append(path)
        libs = []
        for name in ("ucrt", "um"):
            path = base / "Lib" / sub / name / "x86"
            if path.is_dir():
                libs.append(path)
        if includes and libs:
            return includes, libs

    raise SystemExit(f"no usable Windows SDK under {base}")
def environment(includes: list[Path], libs: list[Path]) -> dict[str, str]:
    env = os.environ.copy()
    msvc_include = MSVC_ROOT / "include"
    msvc_lib = MSVC_ROOT / "lib" / "x86"
    sdk_include = WINSDK_ROOT
    all_includes = [str(msvc_include), str(DX8 / "include"), *map(str, includes)]
    all_libs = [str(msvc_lib), str(DX8 / "lib"), *map(str, libs)]
    # PATH needs the compiler's own directory for c1.dll/c2.dll/link.exe to load.
    env["PATH"] = f"{MSVC_BIN};{env.get('PATH', '')}"
    env["INCLUDE"] = ";".join(all_includes)
    env["LIB"] = ";".join(all_libs)
    return env


def run(command: list[str], log: Path, env: dict[str, str], cwd: Path) -> None:
    with log.open("wb") as stream:
        result = subprocess.run(command, cwd=cwd, env=env, stdout=stream,
                                stderr=subprocess.STDOUT)
    if result.returncode:
        text = log.read_text(errors="replace")
        sys.stderr.write(text[-6000:])
        raise SystemExit(f"failed: {' '.join(command[:3])} (see {log})")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--clean", action="store_true")
    parser.add_argument("--keep-going", action="store_true",
                        help="compile every file and report all errors at once")
    args = parser.parse_args()

    if args.clean and OUT.exists():
        shutil.rmtree(OUT, ignore_errors=True)
    OUT.mkdir(parents=True, exist_ok=True)
    GENERATED.mkdir(parents=True, exist_ok=True)

    if not (MSVC_BIN / "cl.exe").is_file():
        raise SystemExit(f"cl.exe not found at {MSVC_BIN}")
    if not (DX8 / "include/d3d8.h").is_file():
        raise SystemExit(f"DirectX 8 SDK not found at {DX8}")

    # The generated message table the sources include.
    subprocess.run([sys.executable, str(ROOT / "tools/generate_i18n.py"),
                    str(ROOT / "resources/csv/i18n.csv"),
                    str(GENERATED / "i18n.hpp")], check=True)

    includes, libs = find_winsdk_paths()
    env = environment(includes, libs)

    objects: list[Path] = []
    failures: list[str] = []
    for name in SOURCES:
        source = SRC / name
        if not source.is_file():
            raise SystemExit(f"missing source: {name}")
        obj = OUT / (name.replace("/", "_")[:-4] + ".obj")
        command = [
            str(MSVC_BIN / "cl.exe"), *FLAGS,
            f"/I{GENERATED}", f"/I{SRC}",
            *[f"/I{p}" for p in includes],
            f"/I{DX8 / 'include'}",
            f"/I{MSVC_ROOT / 'include'}",
            "/c", str(source), f"/Fo{obj}",
        ]
        log = OUT / (name.replace("/", "_")[:-4] + ".log")
        print(f"cl {name}", flush=True)
        with log.open("wb") as stream:
            result = subprocess.run(command, cwd=OUT, env=env, stdout=stream,
                                    stderr=subprocess.STDOUT)
        if result.returncode:
            failures.append(name)
            if not args.keep_going:
                sys.stderr.write(log.read_text(errors="replace")[-6000:])
                raise SystemExit(f"compile failed: {name}")
        else:
            objects.append(obj)

    if failures:
        for name in failures:
            sys.stderr.write(f"\n=== {name} ===\n")
            sys.stderr.write((OUT / (name.replace("/", "_")[:-4] + ".log")).read_text(errors="replace")[-3000:])
        raise SystemExit(f"{len(failures)} file(s) failed to compile")

    exe = OUT / "th07.exe"
    link_log = OUT / "link.log"
    command = [
        str(MSVC_BIN / "link.exe"), "/nologo", "/INCREMENTAL:NO",
        f"/OUT:{exe}", "/SUBSYSTEM:WINDOWS", *map(str, objects), *LIBS,
        "/MAP",
    ]
    print("link", flush=True)
    run(command, link_log, env, OUT)

    if not exe.is_file():
        raise SystemExit("linker produced no executable")
    print(f"\n{exe}  ({exe.stat().st_size} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
