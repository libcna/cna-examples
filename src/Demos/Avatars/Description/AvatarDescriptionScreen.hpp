// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/AvatarBodyType.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AvatarDescription.hpp"
#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Avatars::DescriptionDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::AvatarBodyType;
using Microsoft::Xna::Framework::GamerServices::AvatarDescription;

// Exercises the standard random-description API and checks its opaque payload.
class AvatarDescriptionScreen : public DemoScreen {
public:
    AvatarDescriptionScreen() : DemoScreen("AvatarDescription: Random Bodies") {}

    void OnDemoLoad() override {
        const auto female = AvatarDescription::CreateRandom(AvatarBodyType::Female);
        const auto male = AvatarDescription::CreateRandom(AvatarBodyType::Male);
        femaleValid_ = female.getIsValidProperty();
        maleValid_ = male.getIsValidProperty();
        femaleType_ = female.getBodyTypeProperty();
        maleType_ = male.getBodyTypeProperty();
        femaleHeight_ = female.getHeightProperty();
        maleHeight_ = male.getHeightProperty();
        byteCount_ = male.getDescriptionProperty().size();
    }

protected:
    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        const std::vector<std::string> lines = {
            "AvatarDescription::CreateRandom creates real CNA avatar descriptions.",
            "Female: valid=" + std::string(femaleValid_ ? "true" : "false") +
                "  height=" + std::to_string(femaleHeight_) + " m",
            "Male:   valid=" + std::string(maleValid_ ? "true" : "false") +
                "  height=" + std::to_string(maleHeight_) + " m",
            "Opaque description bytes: " + std::to_string(byteCount_) + " (expected 1021)"
        };
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 82.0f), lines,
                                      mul(Color::White, TransitionAlpha()));
        const bool pass = femaleValid_ && maleValid_ && byteCount_ == 1021 &&
                          femaleType_ == AvatarBodyType::Female && maleType_ == AvatarBodyType::Male &&
                          femaleHeight_ > 0.0f && maleHeight_ > 0.0f;
        DrawVerdict(sb, font, end.Y + 10.0f,
                    pass ? Color(60, 200, 90, 255) : Color(210, 60, 60, 255),
                    mul(Color::White, TransitionAlpha()),
                    pass ? "PASS: both body types have valid descriptions"
                         : "FAIL: random avatar description did not meet its contract");
    }

private:
    bool femaleValid_ = false;
    bool maleValid_ = false;
    AvatarBodyType femaleType_ = AvatarBodyType::Male;
    AvatarBodyType maleType_ = AvatarBodyType::Female;
    float femaleHeight_ = 0.0f;
    float maleHeight_ = 0.0f;
    std::size_t byteCount_ = 0;
};

} // namespace CnaExamples::Demos::Avatars::DescriptionDemos
