#!/usr/bin/env python3
"""Assemble the th07.data/ asset directory the game reads.

    python tools/extract_assets.py             # build/run/th07.data/
    python tools/extract_assets.py --force     # rebuild it from scratch
    python tools/extract_assets.py --out DIR   # somewhere else

The directory is built from two sources, in order:

1. `assets/` — modernised content, committed to the repository. This is what the project
   owns: converted images, rewritten configuration, anything the project has finished
   reworking. It is copied first so it always wins.
2. The original archive, via thdat. Everything that has not been modernised yet is still
   only available here, so it is extracted over the top — but only for files `assets/` does
   not already provide.

The result is that a file moves from "extracted from the archive" to "committed in assets/"
by appearing there, with no build change. Once nothing is left to extract, the archive
dependency disappears on its own.

The archive is not distributed with this project. Point --archive at a copy of th07.dat,
which lives in .tools/ by default.
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

ASSETS = ROOT / "assets"
DEFAULT_ARCHIVE = TOOLS / "th07.dat"
DEFAULT_OUT = ROOT / "build/run/th07.data"

# A handful of entries the game cannot start without. Their presence is a cheap check that
# the assembled tree is usable, whichever source provided them.
REQUIRED = ["th07_0100b.ver", "ascii.anm", "ecldata1.ecl", "etama.anm"]


def copy_assets(assets: Path, out: Path) -> int:
    """Copy the committed assets over the destination. Returns the file count.

    Markdown files are skipped: they document the directory to a reader of the repository
    and have no business in the game's data.
    """
    if not assets.is_dir():
        return 0
    count = 0
    for path in sorted(assets.rglob('*')):
        if not path.is_file() or path.suffix == '.md':
            continue
        relative = path.relative_to(assets)
        target = out / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, target)
        count += 1
    return count


def extract_archive(archive: Path, out: Path) -> int:
    """Extract the original archive into a scratch directory, then merge it in.

    thdat writes every file it finds, so extracting straight into `out` would overwrite the
    committed assets. Extracting beside it and copying only the gaps keeps `assets/`
    authoritative even as more of it is filled in.
    """
    scratch = out.parent / (out.name + '.archive')
    if scratch.exists():
        shutil.rmtree(scratch)
    scratch.mkdir(parents=True)

    result = subprocess.run([str(THDAT), "-x", "d", str(archive)],
                            cwd=str(scratch), capture_output=True, text=True,
                            errors="replace")
    if result.returncode != 0:
        print("thdat failed")
        print(result.stdout[-2000:])
        print(result.stderr[-2000:])
        return -1

    added = 0
    kept = 0
    for path in sorted(scratch.rglob('*')):
        if not path.is_file():
            continue
        relative = path.relative_to(scratch)
        target = out / relative
        if target.exists():
            kept += 1
            continue
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(path, target)
        added += 1

    shutil.rmtree(scratch)
    print(f"archive contributed {added} file(s); {kept} already provided by assets/")
    return added


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--archive", type=Path, default=DEFAULT_ARCHIVE)
    parser.add_argument("--assets", type=Path, default=ASSETS)
    parser.add_argument("--out", type=Path, default=DEFAULT_OUT)
    parser.add_argument("--force", action="store_true",
                        help="rebuild even if the output directory already exists")
    parser.add_argument("--assets-only", action="store_true",
                        help="skip the archive; useful once everything is modernised")
    args = parser.parse_args()

    if args.out.exists():
        if not args.force:
            print(f"{args.out} already exists; pass --force to rebuild")
            return 0
        shutil.rmtree(args.out)
    args.out.mkdir(parents=True, exist_ok=True)

    copied = copy_assets(args.assets, args.out)
    print(f"assets/ contributed {copied} file(s)")

    if not args.assets_only:
        if not THDAT.is_file():
            print(f"thdat not found: {THDAT}")
            print("It comes with thtk. Place a thtk release under .tools/thtk-bin-*/.")
            return 1
        if not args.archive.is_file():
            print(f"archive not found: {args.archive}")
            print("Put a copy of the original th07.dat in .tools/, or pass --assets-only.")
            return 1
        if extract_archive(args.archive, args.out) < 0:
            return 1

    files = sorted(p for p in args.out.rglob("*") if p.is_file())
    total = sum(p.stat().st_size for p in files)
    print(f"{args.out}: {len(files)} files, {total / (1024 * 1024):.1f} MiB")

    missing = [name for name in REQUIRED if not (args.out / name).is_file()]
    if missing:
        print(f"missing expected files: {', '.join(missing)}")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
