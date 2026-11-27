#!/usr/bin/env python3
"""Turn the original PBG4 archive into the th07.data/ asset directory.

    python tools/extract_assets.py            # write build/run/th07.data/
    python tools/extract_assets.py --out DIR  # write somewhere else

The game reads its assets from `th07.data/` next to the executable, one file per entry.
thdat from thtk does the extraction; this script only drives it and checks the result.

The archive is not distributed with this project. Point --archive at a copy of the
original th07.dat, which lives in .tools/ by default.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / ".tools"
THTK = TOOLS / "thtk-bin-12"
THDAT = THTK / "thdat.exe"

DEFAULT_ARCHIVE = TOOLS / "th07.dat"
DEFAULT_OUT = ROOT / "build/run/th07.data"

# A handful of entries the game cannot start without. Their presence is a cheap check that
# the extraction actually produced a usable tree.
REQUIRED = ["th07_0100b.ver", "ascii.anm", "ecldata1.ecl", "etama.anm"]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--force", action="store_true",
                        help="re-extract even if the output directory already exists")
    args = parser.parse_args()

    if not THDAT.is_file():
        print(f"thdat not found: {THDAT}")
        print("It comes with thtk. Place a thtk release under .tools/thtk-bin-*/.")
        return 1
    if not args.archive.is_file():
        print(f"archive not found: {args.archive}")
        print("The original th07.dat is not distributed; put a copy in .tools/.")
        return 1

    if args.out.exists():
        if not args.force:
            print(f"{args.out} already exists; pass --force to re-extract")
            return 0
        shutil.rmtree(args.out)

    args.out.mkdir(parents=True, exist_ok=True)

    # thdat writes into the current directory, so run it from the destination.
    result = subprocess.run(
        [str(THDAT), "-x", "d", str(args.archive)],
        cwd=str(args.out), capture_output=True, text=True, errors="replace")
    if result.returncode != 0:
        print("thdat failed")
        print(result.stdout[-2000:])
        print(result.stderr[-2000:])
        return 1

    files = sorted(p for p in args.out.rglob("*") if p.is_file())
    print(f"extracted {len(files)} files into {args.out}")

    missing = [name for name in REQUIRED if not (args.out / name).is_file()]
    if missing:
        print(f"missing expected files: {', '.join(missing)}")
        return 1

    total = sum(p.stat().st_size for p in files)
    print(f"total {total / (1024 * 1024):.1f} MiB")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
