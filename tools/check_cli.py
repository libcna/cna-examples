#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Check the graphics-free catalog and command-line validation paths."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

from check_catalog import collect_registrations


def run(binary: Path, *args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(binary), *args], text=True, capture_output=True, timeout=10, check=False
    )


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: tools/check_cli.py <cna_examples executable>", file=sys.stderr)
        return 2
    binary = Path(sys.argv[1]).resolve()
    if not binary.is_file():
        print(f"missing executable: {binary}", file=sys.stderr)
        return 2

    listing = run(binary, "--list-demos")
    paths = listing.stdout.splitlines()
    expected = len(collect_registrations())
    assert listing.returncode == 0, listing.stderr
    assert len(paths) == expected, f"binary lists {len(paths)} demos, source registers {expected}"
    assert len(set(paths)) == len(paths), "duplicate catalog paths"

    query = run(binary, "--list-demos", "--search", "occlusion")
    upper = run(binary, "--list-demos", "--search", "OCCLUSION")
    assert query.returncode == upper.returncode == 0
    assert query.stdout and query.stdout == upper.stdout, "search lost case independence"
    assert set(query.stdout.splitlines()) <= set(paths), "search returned an unknown path"

    invalid = (
        ("--frames", "abc"),
        ("--frames", "-1"),
        ("--frames", "0"),
        ("--frames", "12x"),
        ("--key-interval", "0"),
        ("--key-interval", "nope"),
        ("--pointer", "1,2,3garbage"),
        ("--pointer", "nan,2,3"),
    )
    for args in invalid:
        result = run(binary, "--list-demos", *args)
        assert result.returncode == 2 and result.stderr, f"accepted invalid {args}: {result}"

    valid = run(binary, "--list-demos", "--frames", "12", "--key-interval", "2",
                "--pointer", "1,2,3")
    assert valid.returncode == 0 and valid.stdout.splitlines() == paths, valid.stderr
    print(f"OK: {expected} unique demos, search, and CLI validation")
    return 0


if __name__ == "__main__":
    sys.exit(main())
