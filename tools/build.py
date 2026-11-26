#!/usr/bin/env python3
"""Build th07.exe from src/ with the installed MSVC and the DirectX 8 SDK.

    python tools/build.py             # compile + link, then populate build/run-ready
    python tools/build.py --keep-tmp  # same, but leave intermediate objects and logs

Layout the script produces:

    build/                     nothing but the files the game needs at runtime
        th07.exe               the built executable
        th07.dat               copied from .tools, PBG4 archive (graphics, ECL, msg)
        thbgm.dat              copied from .tools, ZWAV archive (music)
        th07.cfg               generated, windowed mode, see write_config()
    build/tmp/                 intermediates, safe to delete at any time
        obj/                   .obj
        log/                   cl.exe and link.exe output
        generated/             i18n.hpp, and the map file
        tmp/*.dat              staging for the dat copies, so a copy is not a
                               repeated 424 MB read from .tools

The compiler is whatever MSVC this machine has, invoked in its 32-bit form. The
DirectX 8 headers and libraries come from .tools/dx8sdk; that directory is not in git
because it is a redistributable SDK rather than source. The .dat files are the retail
game's own data and are not in git either.
"""
from __future__ import annotations

import argparse
import os
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC = ROOT / "src"
DX8 = ROOT / ".tools/dx8sdk"
TOOLS = ROOT / ".tools"
OUT = ROOT / "build"
TMP = OUT / "tmp"
OBJ = TMP / "obj"
LOG = TMP / "log"
GENERATED = TMP / "generated"
STAGE = TMP / "dat"
LIB = TMP / "lib"

# Files copied next to the executable. The game opens both by relative name.
DATA_FILES = ["th07.dat", "thbgm.dat"]

# The MSVC that is installed here. The 32-bit compiler is what matters: the target is
# a Win32 executable and the sources assume the VC7-era 32-bit ABI.
MSVC_BIN = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207\bin\Hostx86\x86")
MSVC_ROOT = Path(r"D:\toolchain\VS\VC\Tools\MSVC\14.44.35207")
WINSDK_ROOT = Path(r"C:\Program Files (x86)\Windows Kits\10")

# The game's own sources. `legacy/` headers are internal split-outs of the same
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
    "/Fd" + str(OBJ / "th07.pdb"),
]


# --- the configuration file -------------------------------------------------------
#
# GameConfiguration lives in src/Supervisor.hpp and is asserted there to be 0x38 bytes.
# Its layout is what this writes, field by field, so the values below stay readable
# instead of being a hex blob. Change the struct, change this.
#
#     ControllerMapping controllerMapping;  9 x i16 = 0x12 bytes
#     (2 bytes of padding: the i32 below is 4-aligned)
#     i32   version                          must be 0x70002 or LoadConfig() reinitialises
#     i16   padAxisX, padAxisY
#     u8    lifeCount, bombCount, colorMode16bit, musicMode, playSounds,
#           defaultDifficulty, windowed, frameskipConfig, effectQuality,
#           slowMode, shotSlow
#     u8    unused_27[13]
#     u32   opts                             bitfield union
#
# Supervisor::LoadConfig() rejects the whole file unless every field is in range and
# the size matches exactly, then silently falls back to its defaults -- which are
# fullscreen. So this has to be right, not approximately right.

# Global.hpp: struct ControllerMapping, initialised in Supervisor.cpp as
# {0, 1, 2, 4, -1, -1, -1, -1, 3} -- the same mapping the game uses when no config
# exists, so the controls match the default the original ships with.
CONTROLLER_MAPPING = (0, 1, 2, 4, -1, -1, -1, -1, 3)

# Supervisor::LoadConfig() defaults, except where noted.
VERSION = 0x70002
PAD_AXIS = 600
LIFE_COUNT = 2
BOMB_COUNT = 3
# 255 means "not decided yet": main.cpp InitD3dDevice() resolves it to 0 (32-bit) or 1
# (16-bit) from the adapter's display mode, but only in the FULLSCREEN branch. In
# windowed mode that branch never runs, so 255 would persist.
# LoadConfig() validates `colorMode16bit < 2` BEFORE any of that, so writing 255 here
# makes the file fail validation, get discarded, and fall back to the built-in defaults
# -- which are fullscreen. Write the resolved value instead: 0 = 32-bit.
COLOR_MODE_16BIT = 0
MUSIC_MODE = 1          # MUSIC_WAV; thbgm.dat is present, and WAV needs no MIDI device
PLAY_SOUNDS = 1
DEFAULT_DIFFICULTY = 1  # DIFF_NORMAL
WINDOWED = 1            # the one field this script changes from the original default
FRAMESKIP = 0
EFFECT_QUALITY = 2      # QUALITY_BEAUTIFUL
SLOW_MODE = 0
SHOT_SLOW = 1
UNUSED_27 = bytes(13)
OPTS = 0

# Offsets below were read off the compiler, not counted by hand:
#   sizeof(GameConfiguration) = 0x38, sizeof(ControllerMapping) = 0x12.
# ControllerMapping ends at 0x12, but the i32 version is 4-aligned, so it lands at 0x14
# after a 2-byte hole (the header's "// pad 2"). The `2x` in FORMAT writes that hole.
#
#   controllerMapping 0x00   9 x i16
#   version           0x14   i32, must be 0x70002
#   padAxisX/Y        0x18/0x1a
#   lifeCount         0x1c .. shotSlow 0x26   11 x u8
#   unused_27         0x27   13 x u8
#   opts              0x34   u32 bitfield
FORMAT = "<9h 2x I h h 11B 13s I"
CONFIG_SIZE = 0x38
WINDOWED_OFFSET = 0x22


def config_bytes() -> bytes:
    blob = struct.pack(
        FORMAT,
        *CONTROLLER_MAPPING,
        VERSION,
        PAD_AXIS,
        PAD_AXIS,
        LIFE_COUNT,
        BOMB_COUNT,
        COLOR_MODE_16BIT,
        MUSIC_MODE,
        PLAY_SOUNDS,
        DEFAULT_DIFFICULTY,
        WINDOWED,
        FRAMESKIP,
        EFFECT_QUALITY,
        SLOW_MODE,
        SHOT_SLOW,
        UNUSED_27,
        OPTS,
    )
    if len(blob) != CONFIG_SIZE:
        raise SystemExit(
            f"config layout is {len(blob)} bytes but GameConfiguration is {CONFIG_SIZE}"
        )
    # Guard the one field this script exists to set. A wrong offset here means the game
    # silently starts fullscreen, which is the failure this build is meant to avoid.
    if blob[WINDOWED_OFFSET] != WINDOWED:
        raise SystemExit(
            f"windowed field is not at {WINDOWED_OFFSET:#x} "
            f"(found {blob[WINDOWED_OFFSET]})"
        )
    # Reproduce LoadConfig()'s own validation. If any of these fail the game discards
    # the file and regenerates defaults, which are fullscreen -- so a config that fails
    # here is worse than no config at all, because the build still looks successful.
    for name, value, limit in (
        ("lifeCount", LIFE_COUNT, 5),
        ("bombCount", BOMB_COUNT, 4),
        ("colorMode16bit", COLOR_MODE_16BIT, 2),
        ("musicMode", MUSIC_MODE, 3),
        ("playSounds", PLAY_SOUNDS, 2),
        ("defaultDifficulty", DEFAULT_DIFFICULTY, 6),
        ("windowed", WINDOWED, 2),
        ("frameskipConfig", FRAMESKIP, 3),
        ("effectQuality", EFFECT_QUALITY, 3),
        ("slowMode", SLOW_MODE, 2),
        ("shotSlow", SHOT_SLOW, 2),
    ):
        if not 0 <= value < limit:
            raise SystemExit(
                f"config {name}={value} is out of range (must be < {limit}); "
                "the game would discard the file and start fullscreen"
            )
    return blob


def write_config(path: Path) -> None:
    blob = config_bytes()
    path.write_bytes(blob)
    print(f"cfg {path.name}  windowed={WINDOWED}  {len(blob)} bytes")


# --- toolchain --------------------------------------------------------------------


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


# --- data files -------------------------------------------------------------------


def stage_data(dat_path: Path) -> Path | None:
    """Copy a .dat from .tools into build/, going through build/tmp/dat.

    The staging copy makes the build reproducible without re-reading 424 MB from the
    source tree every time, and it means a failed copy never leaves a truncated file in
    build/ where the game would find it.
    """
    source = TOOLS / dat_path.name
    if not source.is_file():
        return None
    STAGE.mkdir(parents=True, exist_ok=True)
    staged = STAGE / dat_path.name
    if not staged.is_file() or staged.stat().st_size != source.stat().st_size:
        print(f"dat {dat_path.name}  staging from .tools", flush=True)
        shutil.copyfile(source, staged)
    shutil.copyfile(staged, dat_path)
    return dat_path


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--keep-tmp", action="store_true",
                        help="keep build/tmp (objects, logs) instead of deleting it")
    parser.add_argument("--keep-going", action="store_true",
                        help="compile every file and report all errors at once")
    args = parser.parse_args()

    # The build output is only ever the game's runtime files, so it starts empty. A
    # stale th07.exe from an older build must not be mistaken for a fresh one.
    if OUT.exists():
        shutil.rmtree(OUT, ignore_errors=True)
    for path in (OUT, TMP, OBJ, LOG, GENERATED, STAGE, LIB):
        path.mkdir(parents=True, exist_ok=True)

    if not (MSVC_BIN / "cl.exe").is_file():
        raise SystemExit(f"cl.exe not found at {MSVC_BIN}")
    if not (DX8 / "include/d3d8.h").is_file():
        raise SystemExit(f"DirectX 8 SDK not found at {DX8}")

    # The generated message table the sources include.
    subprocess.run([sys.executable, str(ROOT / "tools/generate_i18n.py"),
                    str(ROOT / "tools/i18n.csv"),
                    str(GENERATED / "i18n.hpp")], check=True)

    includes, libs = find_winsdk_paths()
    env = environment(includes, libs)

    objects: list[Path] = []
    failures: list[str] = []
    for name in SOURCES:
        source = SRC / name
        if not source.is_file():
            raise SystemExit(f"missing source: {name}")
        stem = name.replace("/", "_")[:-4]
        obj = OBJ / (stem + ".obj")
        command = [
            str(MSVC_BIN / "cl.exe"), *FLAGS,
            f"/I{GENERATED}", f"/I{SRC}",
            *[f"/I{p}" for p in includes],
            f"/I{DX8 / 'include'}",
            f"/I{MSVC_ROOT / 'include'}",
            "/c", str(source), f"/Fo{obj}",
        ]
        log = LOG / (stem + ".log")
        print(f"cl {name}", flush=True)
        with log.open("wb") as stream:
            result = subprocess.run(command, cwd=OBJ, env=env, stdout=stream,
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
            stem = name.replace("/", "_")[:-4]
            sys.stderr.write(f"\n=== {name} ===\n")
            sys.stderr.write((LOG / (stem + ".log")).read_text(errors="replace")[-3000:])
        raise SystemExit(f"{len(failures)} file(s) failed to compile")

    exe = OUT / "th07.exe"
    link_log = LOG / "link.log"
    command = [
        str(MSVC_BIN / "link.exe"), "/nologo", "/INCREMENTAL:NO",
        f"/OUT:{exe}", "/SUBSYSTEM:WINDOWS", *map(str, objects), *LIBS,
        "/MAP:" + str(GENERATED / "th07.map"),
        # An executable-only link still emits an import library, its .exp sidecar and a
        # .pdb next to /OUT. Send them to tmp so build/ holds nothing but what the game
        # needs. They live in their own directory because link derives the .exp name
        # from the .lib path.
        "/IMPLIB:" + str(LIB / "th07.lib"),
        "/PDB:" + str(LIB / "th07.pdb"),
    ]
    print("link", flush=True)
    run(command, link_log, env, OBJ)

    if not exe.is_file():
        raise SystemExit("linker produced no executable")

    missing = []
    for name in DATA_FILES:
        if stage_data(OUT / name) is None:
            missing.append(name)
    write_config(OUT / "th07.cfg")

    if not args.keep_tmp:
        shutil.rmtree(TMP, ignore_errors=True)

    print(f"\n{exe}  ({exe.stat().st_size} bytes)")
    for name in DATA_FILES:
        if name not in missing:
            print(f"{OUT / name}  ({OUT.joinpath(name).stat().st_size} bytes)")
    if missing:
        print(f"\nwarning: missing from .tools: {', '.join(missing)}", file=sys.stderr)
        print("the game will not start without them", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
