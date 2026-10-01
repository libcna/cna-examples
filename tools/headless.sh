#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
#
# Run cna_examples with SDL's offscreen video driver, never the real desktop.
#
#   tools/headless.sh --demo "Media/Pictures/Browse" --frames 90 --screenshot /tmp/a.png
#
# Override SDL_VIDEODRIVER=x11 to use Xvfb on a build without SDL offscreen.

set -uo pipefail

export SDL_VIDEODRIVER="${SDL_VIDEODRIVER:-offscreen}"
unset WAYLAND_DISPLAY

BUILD="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/build"

if [[ ! -x "$BUILD/cna_examples" ]]; then
    echo "cna_examples not built -- run: cmake --build build --parallel --target cna_examples" >&2
    exit 1
fi

cd "$BUILD"
if [[ "$SDL_VIDEODRIVER" == x11 ]]; then
    exec xvfb-run -a ./cna_examples "$@"
fi
exec ./cna_examples "$@"
