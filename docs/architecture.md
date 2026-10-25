# Architecture

This document records the stable design decisions behind `cna-examples`. Current catalog counts,
build commands, verification results, and outstanding validation gaps belong in
[`README.md`](../README.md). Completed roadmaps and investigation logs remain available in Git
history rather than in active documentation.

## Purpose and boundaries

`cna-examples` is one cross-platform application containing a browsable catalog of live CNA/XNA
demonstrations. It is intentionally different from the neighbouring projects:

- `../cna` is the framework being demonstrated and is consumed with `add_subdirectory`.
- `../sharp-runtime` is pulled in transitively by CNA.
- `../cna-samples` contains separate ports of official XNA samples. It is a reference for build
  structure and the Game State Management pattern, not a runtime dependency.
- `../cna/examples` contains CNA's development-oriented tests and standalone demos. This project
  may reuse a proven technique from there, but presents it through the single catalog application.

The application uses public `Microsoft::Xna::Framework` and `CNA::*` APIs. A demo should teach or
verify an API rather than reproduce backend internals. Backend pixel tests and ports of complete
Microsoft samples therefore stay in their respective sibling repositories.

## Catalog and navigation

The runtime hierarchy is data driven:

```text
Home
|- Search
`- Area
   |- Category
   |  `- Demo
   `- Group                 optional, used by areas with many categories
      `- Category
         `- Demo
```

`src/Navigation/AreaCatalog.cpp` assembles the hierarchy. Each area owns its registrations in
`src/Navigation/CatalogAreas/<Area>.cpp`, while `AreaCatalog.hpp` contains the public data
structures shared by navigation and the headless harness. Exactly one of `AreaEntry::categories`
and `AreaEntry::groups` is populated.

A `DemoEntry` contains a title, description, API-symbol list, and a lazy screen factory. The API
list has three consumers: the footer displayed on the demo, the search index, and the answer to
"which API does this demo exercise?" Keeping this metadata beside the registration prevents
separate inventories from drifting.

The navigation screens are backed by the local `GameStateManagement` screen stack, adapted as an
independent implementation of the XNA Game State Management pattern. Menus support keyboard,
gamepad, mouse, and touch navigation, including drag scrolling. Breadcrumbs are assigned when a
screen is opened rather than duplicated in each demo.

## Demo-screen lifecycle

Every runnable leaf derives from `Demos::DemoScreen`. It owns the common title, breadcrumb, API
footer, Back interaction, layout helpers, and capability/prerequisite handling. Concrete screens
use these hooks instead of overriding the underlying `GameScreen` lifecycle directly:

- `OnDemoLoad()` / `OnDemoUnload()` for resources;
- `OnDemoUpdate()` for live state;
- `OnDemoInput()` for demo-specific input;
- `OnDemoDraw()` for rendering.

`LoadContent()` and `UnloadContent()` are final so capability gating cannot be bypassed by a new
demo. `OnDemoUnload()` must restore any global state changed by a screen, such as back-buffer size,
media state, or device settings. A clean screenshot after the screen opens does not prove that
state is restored when it closes, so stateful demos need an explicit restoration check.

Application prerequisites such as a signed-in gamer are separate from graphics capabilities.
Until `PrerequisitesMet()` succeeds, the screen remains unloaded and renders its not-ready state.

## Renderer capability gating

CNA selects its graphics renderer at configure time through `CNA_GRAPHICS_RENDERER`; this project
defaults to `OPENGLES3`. The application must not assume that every renderer implements a full 3D
pipeline. `SDL_RENDERER`, for example, is deliberately 2D-only.

`Requiring()` in `AreaCatalogInternal.hpp` attaches a `GraphicsCapability` requirement at the
catalog assembly site, normally to an entire category. `DemoScreen` checks that requirement before
load, update, input, and draw. Gating the draw path alone is insufficient because most 3D screens
create GPU resources during loading. On an unsupported renderer the demo displays an explanatory
panel instead of executing any guarded hook.

Capability requirements belong in the catalog rather than in individual demo classes. This makes
new screens in a guarded category inherit the correct behaviour automatically.

## Content and licensing

This repository is MIT-licensed and does not vendor third-party media. Its checked-in audio,
video, images, and metadata are synthetic: they are generated from `ffmpeg` sources or built
procedurally at runtime.

Two formats need special handling:

- CNA reads `.xnb` but does not produce it. The Content demos borrow MonoGame-generated fixtures
  from `../cna/tests/assets/xnb` at build time; they are not committed here. The Calibri font
  fixture is excluded because it embeds a proprietary rasterised glyph atlas.
- XACT demos use CNA's existing `XactFileGen.hpp` directly from the sibling checkout to generate
  banks. The source is not copied into this repository. If it is unavailable, the demos compile
  to an explanatory unavailable state.

The app must remain usable when optional borrowed fixtures are absent. A demo should report that
condition on screen rather than turn it into a build failure or crash.

## Verification model

Verification has several layers because a successful render is not enough to establish correct
behaviour:

1. `tools/check_catalog.py` checks definitions, registrations, duplicate reachability, required
   includes, and the current inventory documented in README.
2. `tools/check_layout.py` rejects known hard-coded layout hazards.
3. `tools/check_cli.py` compares the built executable's flattened catalog with the source model
   and checks search and command-line errors.
4. `tools/headless.sh` and `tools/sweep.sh` render demos and capture screenshots.
5. `tools/check_shots.py` detects blank and overflowing captures.
6. Programs under `tools/checks/` assert factual claims made by screens that catch and display
   their own exceptions. Such a screen can look clean even when every operation failed.

When a result is objectively measurable, prefer an on-screen verdict and a corresponding
assertion over visual judgement alone. Hardware-dependent behaviour still needs a physical-device
pass; the outstanding passes are listed in README's verification status.

## Deliberate limits

A genuine SystemLink host/discover/join round trip is not represented by one demo screen. CNA
allows only one active public `NetworkSession` per process, so a real public-API round trip needs
two operating-system processes. CNA's own tests can pair a session with an internal raw network
host, but using an internal implementation type would violate this catalog's public-API boundary.
Adding a dedicated two-process orchestration mechanism was judged disproportionate for one demo.

Compiled XNA `.fx` bytecode that CNA rejects is demonstrated as an error path rather than treated
as missing catalog coverage. Platform support beyond the targets listed in README remains gated by
CNA itself.
