cmake_minimum_required(VERSION 3.20)

project(CnaExamples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# CNA dependency — consumed as a sibling add_subdirectory, so CNA's own CMake
# machinery (SDL vendoring, renderer
# selection, ...) is fully reused rather than duplicated here.
# ---------------------------------------------------------------------------

# OPENGLES3 is CNA's Linux default renderer and the successor to the old
# EASYGL selection. A preset or -DCNA_GRAPHICS_RENDERER=<X> on the command
# line still wins because CACHE variables set earlier on the command line are
# not overwritten here.
if(NOT DEFINED CNA_GRAPHICS_RENDERER)
    set(CNA_GRAPHICS_RENDERER "OPENGLES3" CACHE STRING
        "Graphics renderer to use (SDL_RENDERER, OPENGLES3, OPENGL33, VULKAN, WEBGPU, ...)")
endif()

set(CNA_BUILD_TESTS           OFF CACHE BOOL "Build CNA tests"           FORCE)
set(CNA_BUILD_EXAMPLES        OFF CACHE BOOL "Build CNA examples"        FORCE)
set(SHARP_RUNTIME_BUILD_TESTS OFF CACHE BOOL "Build sharp-runtime tests" FORCE)

# The Devices area (src/Demos/Devices/) demonstrates CNA::Devices::* (Camera,
# Clipboard, DisplayInfo, FileDialog, Locale, MessageBox, PowerInfo,
# SystemInfo, SystemTray, UrlLauncher) -- all compiled out entirely unless
# this is ON (default OFF in CNA itself, since these are CNA-specific
# extensions beyond XNA 4.0/WP7, not something every CNA consumer wants).
set(CNA_DEVICES ON CACHE BOOL "Enable CNA-specific device/sensor extensions" FORCE)

# CNA's extended graphics layer (reported by the Diagnostics area's Platform
# screen) lives entirely behind this flag -- default OFF in CNA itself, same
# reasoning as CNA_DEVICES above (an extension beyond XNA 4.0).
set(CNA_CNAEXT ON CACHE BOOL "Enable CNA's extended graphics layer" FORCE)

# CNA and sharp-runtime are sibling checkouts (../cna and ../sharp-runtime).
# Both are CACHE paths, so -DCNA_EXAMPLES_CNA_ROOT=<path> or
# -DCNA_SHARP_RUNTIME_ROOT=<path> can point a build at another checkout.
# CNA_EXAMPLES_CNA_ROOT is also where cmake/ExamplesHelpers.cmake borrows the
# XACT generator and .xnb fixtures from.
set(CNA_EXAMPLES_CNA_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../cna"
    CACHE PATH "Path to the CNA checkout this build consumes")
set(CNA_SHARP_RUNTIME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../sharp-runtime"
    CACHE PATH "Path to the sharp-runtime checkout consumed by CNA")

add_subdirectory("${CNA_EXAMPLES_CNA_ROOT}" CNA_BUILD)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ExamplesHelpers)

# ---------------------------------------------------------------------------
# cna_examples — the single browsable demo-catalog application.
# See plan.md for the navigation architecture and Area/Category/Demo layout.
# ---------------------------------------------------------------------------

add_executable(cna_examples
    src/Program.cpp
    src/Navigation/AreaCatalog.cpp
    src/Navigation/CatalogAreas/Input.cpp
    src/Navigation/CatalogAreas/Audio.cpp
    src/Navigation/CatalogAreas/Devices.cpp
    src/Navigation/CatalogAreas/Net.cpp
    src/Navigation/CatalogAreas/Media.cpp
    src/Navigation/CatalogAreas/Framework.cpp
    src/Navigation/CatalogAreas/Math.cpp
    src/Navigation/CatalogAreas/Content.cpp
    src/Navigation/CatalogAreas/Storage.cpp
    src/Navigation/CatalogAreas/Diagnostics.cpp
    src/Navigation/CatalogAreas/Graphics2D.cpp
    src/Navigation/CatalogAreas/Graphics3D.cpp
    src/Navigation/CatalogAreas/Avatars.cpp
)

target_include_directories(cna_examples PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Scoped to this target only -- CNA's own targets (added above via
# add_subdirectory) are not ours to warn about or fix here.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cna_examples PRIVATE -Wall -Wextra)
endif()

cna_examples_configure_target(cna_examples)

# Fast, graphics-free checks. The screenshot sweep remains an explicit local
# verification step because it needs a working renderer.
enable_testing()
find_package(Python3 COMPONENTS Interpreter QUIET)
if(Python3_Interpreter_FOUND)
    add_test(NAME catalog
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/check_catalog.py")
    add_test(NAME layout
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/check_layout.py")
    if(NOT EMSCRIPTEN)
        add_test(NAME cli
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/check_cli.py"
                    "$<TARGET_FILE:cna_examples>")
    endif()
endif()
