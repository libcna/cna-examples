#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Check that demo classes, per-area registrations, and current counts agree.

This exists because they silently drifted apart once already: plan.md claimed
12 Audio demo screens when there were 10, and nothing anywhere would have
noticed. With ~290 screens planned, a number in prose is a number nobody
re-derives by hand.

Checks performed:

  1. Every class defined in src/Demos/**/*Screen.hpp is either registered with
     MakeDemo<> in the catalog, or is a base class (something else derives
     from it). A screen that is written but never wired up is unreachable.
  2. Every MakeDemo<Class> registration has a class definition on disk.
  3. No class is registered twice (two menu entries running the same screen is
     almost always a copy-paste slip, not intent).
  4. The README current-status total, category count, and area table match the
     actual catalog. The plan's current-state total matches too.

Usage:
  tools/check_catalog.py            # check, exit non-zero on any disagreement
  tools/check_catalog.py --list     # also print the per-area inventory
"""

from __future__ import annotations

import collections
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEMOS_DIR = ROOT / "src" / "Demos"
CATALOG = ROOT / "src" / "Navigation" / "AreaCatalog.cpp"
AREA_SOURCES = ROOT / "src" / "Navigation" / "CatalogAreas"

CLASS_RE = re.compile(r"^class\s+(\w+)\s*:\s*public\s+(\w+)", re.MULTILINE)
NAMESPACE_RE = re.compile(r"^namespace\s+([\w:]+)\s*\{", re.MULTILINE)
BUILD_FN_RE = re.compile(r"^std::vector<DemoEntry>\s+(\w+)\s*\(", re.MULTILINE)
USING_NS_RE = re.compile(r"using\s+namespace\s+([\w:]+)\s*;")
MAKEDEMO_RE = re.compile(r"MakeDemo<(\w+)>")

# Class names alone are ambiguous on purpose: Song and Video both have a
# LoadAndPlayScreen, and four Input categories each have a
# StateEqualityHashScreen. Everything below is keyed by the namespace-qualified
# name so those are distinct rather than colliding.


def collect_classes() -> tuple[dict[str, Path], set[str]]:
    """Return (qualified class name -> defining file, set of base class names)."""
    defined: dict[str, Path] = {}
    bases: set[str] = set()

    for path in sorted(DEMOS_DIR.rglob("*Screen.hpp")):
        text = path.read_text(encoding="utf-8")
        namespaces = NAMESPACE_RE.findall(text)
        namespace = namespaces[0] if namespaces else ""
        for name, base in CLASS_RE.findall(text):
            defined[f"{namespace}::{name}" if namespace else name] = path
            bases.add(base)
    return defined, bases


def collect_registrations() -> list[str]:
    """Qualified class names registered with MakeDemo<> in area sources.

    Each Build*Demos() function opens with the `using namespace` that its
    MakeDemo<> arguments resolve against, so the qualification is recovered by
    reading that per function rather than guessing from the class name.
    """
    registrations: list[str] = []
    for path in sorted(AREA_SOURCES.glob("*.cpp")):
        text = path.read_text(encoding="utf-8")
        starts = [m.start() for m in BUILD_FN_RE.finditer(text)]
        for index, start in enumerate(starts):
            end = starts[index + 1] if index + 1 < len(starts) else len(text)
            body = text[start:end]
            using = USING_NS_RE.search(body)
            namespace = using.group(1) if using else ""
            for cls in MAKEDEMO_RE.findall(body):
                registrations.append(f"{namespace}::{cls}" if namespace else cls)
    return registrations


def area_of(path: Path) -> str:
    relative = path.relative_to(DEMOS_DIR)
    return relative.parts[0] if len(relative.parts) > 1 else "(shared)"


def main() -> int:
    if not DEMOS_DIR.is_dir() or not CATALOG.is_file() or not AREA_SOURCES.is_dir():
        print("run this from a cna-examples checkout", file=sys.stderr)
        return 2

    defined, bases = collect_classes()
    registrations = collect_registrations()
    registered = set(registrations)

    problems: list[str] = []

    # 1. Defined but neither registered nor used as a base. Base classes are
    #    matched on their bare name, since a `: public Foo` reference is written
    #    unqualified when the base is visible via a using-directive.
    for name, path in sorted(defined.items()):
        if name in registered or name.rsplit("::", 1)[-1] in bases:
            continue
        problems.append(
            f"{name} ({path.relative_to(ROOT)}) is defined but never registered "
            f"with MakeDemo<> and nothing derives from it -- it is unreachable"
        )

    # 2. Registered but not defined anywhere.
    for name in sorted(registered - set(defined)):
        problems.append(f"MakeDemo<{name}> is registered but no *Screen.hpp defines it")

    # 3. Registered more than once.
    for name, count in sorted(collections.Counter(registrations).items()):
        if count > 1:
            problems.append(f"MakeDemo<{name}> is registered {count} times")

    # An omitted include can leave a perfectly counted registration that fails
    # only at C++ compile time. Require each Area source to include its demos.
    for name in sorted(registered & set(defined)):
        header = defined[name]
        source = AREA_SOURCES / f"{area_of(header)}.cpp"
        include = f'#include "{header.relative_to(ROOT / "src")}"'
        if not source.is_file() or include not in source.read_text(encoding="utf-8"):
            problems.append(f"{source.relative_to(ROOT)} must include {include} for {name}")

    total = len(registrations)

    # 4. Check the designated current-status fields, not a stray historical
    # mention of the same number elsewhere in a long roadmap.
    catalog_text = CATALOG.read_text(encoding="utf-8")
    actual_areas = len(re.findall(r"\bAreaEntry\{", catalog_text))
    actual_categories = len(re.findall(r"\bCategoryEntry\{", catalog_text))
    readme = (ROOT / "README.md").read_text(encoding="utf-8")
    status = re.search(r"\*\*(\d+) areas, (\d+) categories, (\d+) demo screens\*\*", readme)
    if not status or tuple(map(int, status.groups())) != (actual_areas, actual_categories, total):
        problems.append(f"README.md current status must say {actual_areas} areas, "
                        f"{actual_categories} categories, {total} demo screens")

    per_area = collections.Counter(area_of(defined[name]) for name in registrations if name in defined)
    display_names = {"Graphics2D": "2D Graphics", "Graphics3D": "3D Graphics"}
    for area, count in sorted(per_area.items()):
        display = display_names.get(area, area)
        row = re.search(r"^\|\s*" + re.escape(display) + r"\s*\|[^\n]*\|\s*(\d+)\s*\|$",
                        readme, re.MULTILINE)
        if not row or int(row.group(1)) != count:
            problems.append(f"README.md area table: {display} must have {count} screens")

    plan = (ROOT / "plan.md").read_text(encoding="utf-8")
    current = re.search(r"^\*\*(\d+) areas, (\d+) categories, (\d+) demo screens\*\*",
                        plan, re.MULTILINE)
    if not current or tuple(map(int, current.groups())) != (actual_areas, actual_categories, total):
        problems.append(f"plan.md current state must say {actual_areas} areas, "
                        f"{actual_categories} categories, {total} demo screens")

    area_starts = list(re.finditer(r'\bAreaEntry\{"([^"]+)"', catalog_text))
    group_total = 0
    for index, match in enumerate(area_starts):
        end = area_starts[index + 1].start() if index + 1 < len(area_starts) else len(catalog_text)
        block = catalog_text[match.start():end]
        name = match.group(1)
        groups = len(re.findall(r"\bGroupEntry\{", block))
        categories = len(re.findall(r"\bCategoryEntry\{", block))
        group_total += groups
        area = next((source for source, display in display_names.items() if display == name), name)
        row = re.search(r"^\|\s*" + re.escape(name) +
                        r"\s*\|\s*([^|]+)\|\s*(\d+)\s*\|\s*(\d+)\s*\|$",
                        plan, re.MULTILINE)
        expected_groups = "—" if groups == 0 else str(groups)
        if (not row or row.group(1).strip() != expected_groups or
                int(row.group(2)) != categories or int(row.group(3)) != per_area[area]):
            problems.append(f"plan.md current table: {name} must have "
                            f"{groups} groups, {categories} categories, {per_area[area]} screens")
    summary = re.search(r"^\| \*\*Total\*\* \| \*\*(\d+)\*\* \| \*\*(\d+)\*\* \| \*\*(\d+)\*\* \|$",
                        plan, re.MULTILINE)
    if not summary or tuple(map(int, summary.groups())) != (group_total, actual_categories, total):
        problems.append(f"plan.md current table total must be {group_total} groups, "
                        f"{actual_categories} categories, {total} screens")

    if "--list" in sys.argv:
        print("Demo screens per area:")
        for area, count in sorted(per_area.items()):
            print(f"  {area:<12} {count}")
        print(f"  {'TOTAL':<12} {total}")
        base_names = sorted(n.rsplit("::", 1)[-1] for n in defined
                            if n.rsplit("::", 1)[-1] in bases)
        print(f"Base classes (not demos): {', '.join(base_names) or '(none)'}")
        print()

    if problems:
        print(f"{len(problems)} problem(s):")
        for problem in problems:
            print(f"  - {problem}")
        return 1

    print(f"OK: {total} demo screens, all defined, registered exactly once, and documented.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
