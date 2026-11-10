// SPDX-License-Identifier: MIT
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/Achievement.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AchievementCollection.hpp"
#include "Microsoft/Xna/Framework/GamerServices/Gamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/GamerPrivilegeException.hpp"
#include "Microsoft/Xna/Framework/GamerServices/GamerServicesNotAvailableException.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamerCollection.hpp"
#include "System/DateTime.hpp"
#include "System/IO/Stream.hpp"

#include "Demos/Net/SignedInGamerDemoScreen.hpp"

namespace CnaExamples::Demos::Net::GamerServicesDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::Achievement;
using Microsoft::Xna::Framework::GamerServices::Gamer;
using Microsoft::Xna::Framework::GamerServices::GamerPrivilegeException;
using Microsoft::Xna::Framework::GamerServices::GamerServicesNotAvailableException;
using Microsoft::Xna::Framework::GamerServices::SignedInGamer;

// Demonstrates SignedInGamer::AwardAchievement()/GetAchievements() and
// Achievement::GetPicture(). Without an account service an award is kept in the local profile
// store, so GetAchievements() returns it on the next call; a title that ships an achievement
// catalog may award only what the catalog defines (GamerServicesNotAvailableException
// otherwise), and a guest cannot earn any (GamerPrivilegeException). GetPicture() opens the
// picture the catalog names, and throws GamerServicesNotAvailableException when there is none
// -- as for the demo Achievement built here, which names no picture.
class AchievementsScreen : public SignedInGamerDemoScreen {
public:
    AchievementsScreen() : SignedInGamerDemoScreen("GamerServices: Achievements") {}

    void OnDemoLoad() override {
        gamer_ = (*Gamer::getSignedInGamersProperty())[PlayerIndex::One];
    }

protected:
    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            awardCount_++;
            try {
                gamer_->AwardAchievement("demo-achievement");
                lastAwardResult_.clear();
            } catch (const GamerServicesNotAvailableException& ex) {
                lastAwardResult_ = "AwardAchievement() threw GamerServicesNotAvailableException: " +
                                   ex.getMessageProperty();
            } catch (const GamerPrivilegeException& ex) {
                lastAwardResult_ = "AwardAchievement() threw GamerPrivilegeException: " +
                                   ex.getMessageProperty();
            }
            auto achievements = gamer_->GetAchievements();
            achievementsCount_ = achievements.getCountProperty();
            achievements.Dispose();
        } else if (input.IsNewKeyPress(Keys::G, ControllingPlayer(), playerIndex)) {
            Achievement demo = Achievement::CreateInternal(
                "demo-achievement", "Demo Achievement", "Awarded from this screen",
                true, true, System::DateTime::getNowProperty());
            try {
                const std::unique_ptr<System::IO::Stream> picture(demo.GetPicture());
                lastPictureResult_ = "GetPicture() returned a " +
                                     std::to_string(picture->getLengthProperty()) + "-byte stream";
            } catch (const GamerServicesNotAvailableException& ex) {
                lastPictureResult_ = "GetPicture() threw GamerServicesNotAvailableException: " +
                                     ex.getMessageProperty();
            }
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: AwardAchievement() + GetAchievements()");
        lines.push_back("G: construct a demo Achievement and call GetPicture()");
        lines.emplace_back();
        lines.push_back("AwardAchievement() calls: " + std::to_string(awardCount_));
        if (!lastAwardResult_.empty()) lines.push_back("  " + lastAwardResult_);
        lines.push_back("GetAchievements().Count (kept in the local profile store): " +
                         std::to_string(achievementsCount_));
        lines.emplace_back();
        if (!lastPictureResult_.empty()) lines.push_back(lastPictureResult_);

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    SignedInGamer* gamer_ = nullptr;
    int awardCount_ = 0;
    int achievementsCount_ = 0;
    std::string lastPictureResult_;
    std::string lastAwardResult_;
};

} // namespace CnaExamples::Demos::Net::GamerServicesDemos
