// SPDX-License-Identifier: MIT
#pragma once

#include <array>
#include <functional>
#include <string>
#include <vector>

#include "CNA/Platform/PlatformException.hpp"
#include "Microsoft/Xna/Framework/Input/Mouse.hpp"
#include "Microsoft/Xna/Framework/Input/MouseCursor.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Input::MouseDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::Input::Mouse;
using Microsoft::Xna::Framework::Input::MouseCursor;

// Demonstrates the CNAEXT/EXT MouseCursor stock-shape gallery (a MonoGame-
// derived extension, not part of XNA 4.0/FNA): Space/Enter/A/tap cycles
// Mouse::SetCursor() through every stock cursor so the change is visible by
// moving the mouse over the window. A video driver without cursors (SDL's
// offscreen driver, for one) refuses with CNA::Platform::PlatformException;
// the screen reports that instead of failing.
class CursorShapesScreen : public DemoScreen {
public:
    CursorShapesScreen() : DemoScreen("Mouse: Cursor Shapes (EXT)") {}

    void OnDemoUnload() override {
        // Leave the system in a normal state for whatever screen is next.
        if (changed_) TrySetCursor(MouseCursor::getArrowProperty());
        changed_ = false;
    }

protected:
    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            index_ = (index_ + 1) % (int)kCursors.size();
            if (TrySetCursor(kCursors[index_].get())) changed_ = true;
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap cycles the cursor shape.");
        lines.push_back("Move the mouse over this window to see it.");
        lines.emplace_back();
        lines.push_back(std::string("Current: ") + kCursors[index_].name);
        if (!refusal_.empty()) {
            lines.emplace_back();
            lines.push_back("This platform refused the cursor:");
            lines.push_back("  " + refusal_);
        }

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    struct NamedCursor {
        const char* name;
        std::function<MouseCursor&()> get;
    };

    static inline const std::vector<NamedCursor> kCursors = {
        {"Arrow", [] () -> MouseCursor& { return MouseCursor::getArrowProperty(); }},
        {"Crosshair", [] () -> MouseCursor& { return MouseCursor::getCrosshairProperty(); }},
        {"Hand", [] () -> MouseCursor& { return MouseCursor::getHandProperty(); }},
        {"IBeam", [] () -> MouseCursor& { return MouseCursor::getIBeamProperty(); }},
        {"No", [] () -> MouseCursor& { return MouseCursor::getNoProperty(); }},
        {"SizeAll", [] () -> MouseCursor& { return MouseCursor::getSizeAllProperty(); }},
        {"SizeNESW", [] () -> MouseCursor& { return MouseCursor::getSizeNESWProperty(); }},
        {"SizeNS", [] () -> MouseCursor& { return MouseCursor::getSizeNSProperty(); }},
        {"SizeNWSE", [] () -> MouseCursor& { return MouseCursor::getSizeNWSEProperty(); }},
        {"SizeWE", [] () -> MouseCursor& { return MouseCursor::getSizeWEProperty(); }},
        {"Wait", [] () -> MouseCursor& { return MouseCursor::getWaitProperty(); }},
        {"WaitArrow", [] () -> MouseCursor& { return MouseCursor::getWaitArrowProperty(); }},
    };

    bool TrySetCursor(MouseCursor& cursor) {
        try {
            Mouse::SetCursor(cursor);
            return true;
        } catch (const CNA::Platform::PlatformException& ex) {
            refusal_ = ex.what();
            return false;
        }
    }

    int index_ = 0;
    bool changed_ = false;
    std::string refusal_;
};

} // namespace CnaExamples::Demos::Input::MouseDemos
