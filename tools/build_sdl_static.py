#!/usr/bin/env python3
"""Build SDL3, SDL3_image and SDL3_ttf as static libraries into .tools/static/.

    python tools/build_sdl_static.py

The prebuilt MSVC packages ship import libraries only, so static linkage means building
from source. This is a one-time step; the game then links the archives directly and needs
no SDL DLLs at runtime.

Sources are read from repository clones, which are not part of this project:

    D:/SDL        libsdl-org/SDL
    D:/SDL_image  libsdl-org/SDL_image
    D:/SDL_ttf    libsdl-org/SDL_ttf, with external/freetype checked out

Image backends are trimmed to JPEG, which is all the game loads through SDL_image, and TTF
is built without HarfBuzz or PlutoSVG, so FreeType is its only external dependency.

The dependants are configured against the *staged* headers and import-free library
directory rather than through find_package: SDL's installed config files expect a layout
that a Win32 static build does not produce, and the game build links the archives by name
anyway.
"""

from __future__ import annotations

import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / ".tools/static-build"
STAGE = ROOT / ".tools/static"

SDL_SRC = Path(r"D:/SDL")
IMAGE_SRC = Path(r"D:/SDL_image")
TTF_SRC = Path(r"D:/SDL_ttf")

IMAGE_FORMATS = ["AVIF", "BMP", "GIF", "JXL", "LBM", "PCX", "PNG", "PNM", "QOI", "SVG",
                 "TGA", "TIF", "WEBP", "XCF", "XPM", "XV", "ANI"]


def find_cmake() -> Path | None:
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


def run(cmd: list[str], what: str) -> bool:
    print(f"== {what}")
    result = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    if result.returncode != 0:
        print(result.stdout[-5000:])
        print(result.stderr[-5000:])
        return False
    return True


def configure_and_build(cmake: Path, name: str, source: Path,
                        defines: dict[str, str], target: str = "install") -> bool:
    build_dir = BUILD / name
    configure = [
        str(cmake), "-S", str(source), "-B", str(build_dir),
        "-G", "Visual Studio 17 2022", "-A", "Win32",
        "-DCMAKE_INSTALL_PREFIX=" + str(STAGE),
        "-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded",
        # SDL_image and SDL_ttf pick which SDL3 component they need from this, and the
        # shared component does not exist in a static build.
        "-DBUILD_SHARED_LIBS=OFF",
    ]
    for key, value in defines.items():
        configure.append(f"-D{key}={value}")

    if not run(configure, f"configure {name}"):
        return False
    return run([str(cmake), "--build", str(build_dir), "--config", "Release",
                "--target", target], f"build {name}")


def main() -> int:
    cmake = find_cmake()
    if cmake is None:
        print("cmake not found")
        return 1
    for path, what in ((SDL_SRC / "CMakeLists.txt", "SDL"),
                       (IMAGE_SRC / "CMakeLists.txt", "SDL_image"),
                       (TTF_SRC / "CMakeLists.txt", "SDL_ttf"),
                       (TTF_SRC / "external/freetype/CMakeLists.txt", "freetype")):
        if not path.is_file():
            print(f"missing {what}: {path}")
            return 1

    STAGE.mkdir(parents=True, exist_ok=True)

    # 1) SDL3. Installs headers and SDL3-static.lib into the stage directory.
    if not configure_and_build(cmake, "sdl", SDL_SRC, {
        "SDL_STATIC": "ON",
        "SDL_SHARED": "OFF",
        "SDL_TESTS": "OFF",
        "SDL_TEST_LIBRARY": "OFF",
        "SDL_EXAMPLES": "OFF",
    }):
        return 1

    # 2) SDL3_image and 3) SDL3_ttf build against the staged SDL3. Pointing them at the
    # stage directory keeps them from picking up a system SDL.
    sdl_include = STAGE / "include"
    sdl_lib = STAGE / "lib"

    image_defines = {
        "SDL_STATIC": "ON",
        "SDL_SHARED": "OFF",
        "SDL3_INCLUDE_DIR": str(sdl_include),
        "SDL3_LIBRARY": str(sdl_lib / "SDL3-static.lib"),
    }
    for fmt in IMAGE_FORMATS:
        image_defines[f"SDLIMAGE_{fmt}"] = "OFF"
    image_defines["SDLIMAGE_JPG"] = "ON"
    # find_package would reject the staged config; a manual link is what the game does too.
    image_defines["SQLITE3_FOUND"] = "FALSE"
    if not configure_and_build(cmake, "image", IMAGE_SRC, image_defines):
        return 1

    ttf_defines = {
        "SDL_STATIC": "ON",
        "SDL_SHARED": "OFF",
        "SDL3_INCLUDE_DIR": str(sdl_include),
        "SDL3_LIBRARY": str(sdl_lib / "SDL3-static.lib"),
        "SDLTTF_VENDORED": "ON",
        "SDLTTF_HARFBUZZ": "OFF",
        "SDLTTF_PLUTOSVG": "OFF",
    }
    if not configure_and_build(cmake, "ttf", TTF_SRC, ttf_defines):
        return 1

    libs = sorted(STAGE.rglob("*.lib"))
    print(f"\nstaged {len(libs)} libraries under {STAGE}")
    for lib in libs:
        print(f"  {lib.relative_to(STAGE)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
