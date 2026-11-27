#!/usr/bin/env python3
"""

Build the game on Windows with SDL linked statically, and assemble build/run/.

    python tools/build_portable.py            # configure + build + assemble
    python tools/build_portable.py --clean    # wipe build/ first

The executable links SDL3, SDL3_image and SDL3_ttf as static libraries, so build/run/
holds no SDL DLLs; everything it needs at runtime is a Windows system DLL. The archives
come from tools/build_sdl_static.py, which has to run once beforehand.

Game data is the th07.data/ asset directory, produced from the original archive by
tools/extract_assets.py, plus thbgm.dat and a font.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / ".tools"
BUILD = ROOT / "build"

STATIC = TOOLS / "static"

# Files copied next to the executable. The font is embedded in the resource section, so the
# only game data that has to sit beside the executable is the music stream; the rest is the
# th07.data/ directory produced by extract_assets.py.
ASSETS = [
    (TOOLS / "thbgm.dat", "thbgm.dat"),
]


def find_cmake() -> Path | None:
    """cmake from PATH, or the copy that ships with Visual Studio."""
    found = shutil.which("cmake")
    if found:
        return Path(found)
    for candidate in (
        Path(r"D:\toolchain\VS\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"),
        Path(r"C:\Program Files\CMake\bin\cmake.exe"),
    ):
        if candidate.is_file():
            return candidate
    return None


def find_vs_generator(cmake: Path) -> str:
    """Newest Visual Studio generator this cmake knows about."""
    result = subprocess.run([str(cmake), "--help"], capture_output=True, text=True,
                            errors="replace")
    for version in ("17 2022", "16 2019", "15 2017"):
        if f"Visual Studio {version}" in result.stdout:
            return f"Visual Studio {version}"
    raise SystemExit("no supported Visual Studio generator found")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--clean", action="store_true")
    args = parser.parse_args()

    cmake = find_cmake()
    if cmake is None:
        print("cmake not found")
        return 1

    for lib, what in ((STATIC / "lib/SDL3-static.lib", "SDL3"),
                      (STATIC / "lib/SDL3_image-static.lib", "SDL3_image"),
                      (STATIC / "lib/SDL3_ttf-static.lib", "SDL3_ttf")):
        if not lib.is_file():
            print(f"missing static {what}: {lib}")
            print("Run: python tools/build_sdl_static.py")
            return 1

    if args.clean:
        shutil.rmtree(BUILD, ignore_errors=True)
    BUILD.mkdir(parents=True, exist_ok=True)

    generator = find_vs_generator(cmake)
    configure = [str(cmake), "-S", str(ROOT), "-B", str(BUILD),
                 "-G", generator, "-A", "Win32"]
    print(" ".join(configure))
    result = subprocess.run(configure, capture_output=True, text=True, errors="replace")
    print(result.stdout[-4000:])
    if result.returncode != 0:
        print(result.stderr[-4000:])
        return 1

    result = subprocess.run([str(cmake), "--build", str(BUILD), "--config", "Release"],
                            capture_output=True, text=True, errors="replace")
    print(result.stdout[-6000:])
    if result.returncode != 0:
        print(result.stderr[-4000:])
        return 1

    exe = next(BUILD.rglob("th07.exe"), None)
    if exe is None:
        print("no th07.exe produced")
        return 1

    run_dir = BUILD / "run"
    run_dir.mkdir(parents=True, exist_ok=True)
    shutil.copy2(exe, run_dir / "th07.exe")
    for source, name in ASSETS:
        if source.exists():
            shutil.copy2(source, run_dir / name)
        else:
            print(f"warning: missing {source}")

    # The game data: unpack the original archive into th07.data/ unless it is already there.
    if not (run_dir / "th07.data").is_dir():
        extract = subprocess.run(
            [sys.executable, str(ROOT / "tools/extract_assets.py"),
             "--out", str(run_dir / "th07.data")],
            capture_output=True, text=True, errors="replace")
        print(extract.stdout[-2000:])
        if extract.returncode != 0:
            print("asset extraction failed; the game will not start without th07.data/")
            return 1

    print(f"\n{run_dir / 'th07.exe'}  ({(run_dir / 'th07.exe').stat().st_size} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
