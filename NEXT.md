# NEXT — current follow-up work

This file is intentionally short. [`README.md`](README.md) is the current inventory and build
guide; [`docs/architecture.md`](docs/architecture.md) records stable design decisions. Completed
roadmaps and investigation logs are available in Git history.

## Baseline

Last recorded technical status: **2026-10-01**.

- Native catalog: **248 demos in 79 categories and 13 areas**.
- The 2026-09-28 `OPENGLES3` sweep produced 248/248 screenshots with no blank or overflowing
  screen.
- The Emscripten `WEBGL2` build linked successfully. Its Node CLI listed 245 demos; the three
  Video demos are intentionally omitted because CNA disables video on web.
- The seven Avatars demos use the current standard XNA avatar API and passed a 7/7 native sweep.
- Fast checks are `ctest --test-dir build --output-on-failure`; the full renderer check is
  `tools/sweep.sh` followed by `tools/check_shots.py build/screenshots`.

## Open verification

1. Run the web build in a browser and verify rendering, navigation, input, and content loading.
2. Re-run the complete catalog against `SDL_RENDERER`. Its last full pass predates the removal of
   `RenderPipelineSettingsScreen`, so the current 248-screen catalog has not had a fresh pass.
3. Exercise hardware-dependent demos with real devices when available: gamepad, touchscreen,
   joystick/haptics, webcam, Android sensors/vibration, and the human-interaction paths for
   MessageBox/FileDialog.

These are validation gaps, not known failures. No feature roadmap is currently active.

## Workflow for new demos

1. Read the current CNA headers and search `src/Demos/` before declaring an API uncovered.
2. Add the `DemoScreen` subclass under `Demos/<Area>/<Category>/` and register it in the matching
   `Navigation/CatalogAreas/<Area>.cpp` file.
3. Put global-state restoration in `OnDemoUnload()` and verify that restoration explicitly.
4. Run the new demo through `tools/headless.sh`; use a measurable verdict for checkable claims.
5. Add a direct checker under `tools/checks/` when the screen catches errors or states facts that a
   screenshot cannot establish.
6. Update README if the inventory changed, then run CTest and a scoped screenshot sweep before the
   full sweep.
