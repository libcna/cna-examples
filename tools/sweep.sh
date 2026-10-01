#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
#
# Screenshot every demo (or the ones matching a filter) headlessly,
# using the app's own --demo/--screenshot driver. Used for the verification
# sweeps described in plan.md Phase F1.
#
# Usage:
#   tools/sweep.sh                       # every demo
#   tools/sweep.sh Media                 # only demos whose path contains "Media"
#   OUT=/tmp/shots tools/sweep.sh Input  # choose the output directory
#
# Exit status is non-zero if any demo failed to produce a screenshot.

set -uo pipefail

# SDL's offscreen driver supports the default OpenGL ES renderer and avoids
# dependence on an X server. SDL_VIDEODRIVER=x11 selects the Xvfb fallback.
export SDL_VIDEODRIVER="${SDL_VIDEODRIVER:-offscreen}"
unset WAYLAND_DISPLAY

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="$ROOT/build"
OUT="${OUT:-$ROOT/build/screenshots}"
FILTER="${1:-}"
FRAMES="${FRAMES:-90}"

if [[ ! -x "$BUILD/cna_examples" ]]; then
    echo "cna_examples not built -- run: cmake --build build --parallel --target cna_examples" >&2
    exit 1
fi

mkdir -p "$OUT"
OUT="$(cd "$OUT" && pwd)"

# Clear stale output first. A renamed or deleted demo otherwise leaves its old
# .png behind forever, so check_shots.py counts more screenshots than there are
# demos and a removed screen looks like it is still passing. Only a full
# (unfiltered) sweep clears, since a filtered one is not authoritative.
if [[ -z "$FILTER" ]]; then
    rm -f "$OUT"/*.png "$OUT"/*.log
fi
cd "$BUILD"
runner=()
if [[ "$SDL_VIDEODRIVER" == x11 ]]; then runner=(xvfb-run -a); fi

mapfile -t DEMOS < <(./cna_examples --list-demos | { [[ -n "$FILTER" ]] && grep -F "$FILTER" || cat; })

if [[ ${#DEMOS[@]} -eq 0 ]]; then
    echo "no demos matched '$FILTER'" >&2
    exit 1
fi

echo "Sweeping ${#DEMOS[@]} demo(s) into $OUT"
failures=0

for path in "${DEMOS[@]}"; do
    # One file per demo, named after its path with separators flattened.
    safe="${path//\//__}"
    safe="${safe// /_}"
    png="$OUT/$safe.png"
    log="$OUT/$safe.log"

    if "${runner[@]}" ./cna_examples --demo "$path" --frames "$FRAMES" --screenshot "$png" \
            > "$log" 2>&1 && [[ -s "$png" ]]; then
        printf '  ok   %s\n' "$path"
    else
        printf '  FAIL %s   (see %s)\n' "$path" "$log"
        failures=$((failures + 1))
    fi
done

echo
if [[ $failures -eq 0 ]]; then
    echo "All ${#DEMOS[@]} demo(s) rendered."
else
    echo "$failures of ${#DEMOS[@]} demo(s) failed."
fi
exit $(( failures > 0 ))
