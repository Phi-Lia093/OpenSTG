"""

Build the port on Windows with the SDL packages in .tools.

Assets (th07.dat, thbgm.dat, msgothic.ttc) and the SDL DLLs are copied next to the
executable afterwards so it runs from build/run alone.

    python tools/build_portable.py            # configure + build
    python tools/build_portable.py --clean    # wipe build/ first
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / ".tools"
BUILD = ROOT / "build"

SDL3 = TOOLS / "sdl3/SDL3-3.4.20"
SDLIMAGE = TOOLS / "sdlimage/SDL3_image-3.4.8"
SDLTTF = TOOLS / "sdlttf/SDL3_ttf-3.2.2"

ASSETS = [
    (TOOLS / "th07.dat", "th07.dat"),
    (TOOLS / "thbgm.dat", "thbgm.dat"),
    (Path(r"C:\Windows\Fonts\msgothic.ttc"), "msgothic.ttc"),
]
DLLS = [
    (SDL3 / "lib/x86/SDL3.dll", "SDL3.dll"),
    (SDLIMAGE / "lib/x86/SDL3_image.dll", "SDL3_image.dll"),
    (SDLTTF / "lib/x86/SDL3_ttf.dll", "SDL3_ttf.dll"),
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

    if args.clean:
        shutil.rmtree(BUILD, ignore_errors=True)
    BUILD.mkdir(parents=True, exist_ok=True)

    for path, what in ((SDL3 / "lib/x86/SDL3.dll", "SDL3"),
                       (SDLTTF / "lib/x86/SDL3_ttf.dll", "SDL3_ttf")):
        if not path.exists():
            print(f"missing {what}: {path}")
            return 1

    generator = find_vs_generator(cmake)
    configure = [
        str(cmake), "-S", str(ROOT), "-B", str(BUILD),
        "-G", generator, "-A", "Win32",
        f"-DSDL3_DIR={SDL3 / 'cmake'}",
        f"-DSDL3_image_DIR={SDLIMAGE / 'cmake'}",
        f"-DSDL3_ttf_DIR={SDLTTF / 'cmake'}",
    ]
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
    for source, name in ASSETS + DLLS:
        if source.exists():
            shutil.copy2(source, run_dir / name)
        else:
            print(f"warning: missing {source}")

    print(f"\n{run_dir / 'th07.exe'}  ({(run_dir / 'th07.exe').stat().st_size} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
