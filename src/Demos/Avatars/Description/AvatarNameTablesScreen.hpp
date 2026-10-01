// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <exception>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/AvatarAnimation.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AvatarAnimationPreset.hpp"
#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Avatars::DescriptionDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::AvatarAnimation;
using Microsoft::Xna::Framework::GamerServices::AvatarAnimationPreset;

// Builds each public preset and inspects its real duration and skeleton.
class AvatarNameTablesScreen : public DemoScreen {
public:
    AvatarNameTablesScreen() : DemoScreen("AvatarAnimation: Preset Inventory") {}

    void OnDemoLoad() override {
        try {
            for (int i = 0; i <= (int)AvatarAnimationPreset::MaleYawn; ++i) {
                AvatarAnimation animation(static_cast<AvatarAnimationPreset>(i));
                const double seconds = animation.getLengthProperty().getTotalSecondsProperty();
                minLength_ = std::min(minLength_, seconds);
                maxLength_ = std::max(maxLength_, seconds);
                if (seconds > 0.0 && animation.getBoneTransformsProperty().getCountProperty() == 71)
                    ++validPresets_;
                ++totalPresets_;
            }
        } catch (const std::exception& ex) {
            error_ = ex.what();
        }
    }

protected:
    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines = {
            "Every AvatarAnimationPreset constructs a real AvatarAnimation.",
            "Presets: " + std::to_string(totalPresets_) + " (expected 31)",
            "Nonzero duration and 71 bone transforms: " + std::to_string(validPresets_),
            "Length range: " + std::to_string(minLength_) + " to " +
                std::to_string(maxLength_) + " seconds"
        };
        if (!error_.empty()) lines.push_back("Error: " + error_);
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 82.0f), lines,
                                      mul(Color::White, TransitionAlpha()));
        const bool pass = error_.empty() && totalPresets_ == 31 && validPresets_ == 31;
        DrawVerdict(sb, font, end.Y + 10.0f,
                    pass ? Color(60, 200, 90, 255) : Color(210, 60, 60, 255),
                    mul(Color::White, TransitionAlpha()),
                    pass ? "PASS: all 31 presets have animation data"
                         : "FAIL: preset inventory was incomplete");
    }

private:
    int totalPresets_ = 0;
    int validPresets_ = 0;
    double minLength_ = 1.0e9;
    double maxLength_ = 0.0;
    std::string error_;
};

} // namespace CnaExamples::Demos::Avatars::DescriptionDemos
