// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/FriendCollection.hpp"
#include "Microsoft/Xna/Framework/GamerServices/FriendGamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/Gamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/GamerPrivilegeException.hpp"
#include "Microsoft/Xna/Framework/GamerServices/GamerServicesNotAvailableException.hpp"
#include "Microsoft/Xna/Framework/GamerServices/Guide.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamerCollection.hpp"

#include "Demos/Net/SignedInGamerDemoScreen.hpp"

namespace CnaExamples::Demos::Net::GamerServicesDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::FriendGamer;
using Microsoft::Xna::Framework::GamerServices::Gamer;
using Microsoft::Xna::Framework::GamerServices::GamerPrivilegeException;
using Microsoft::Xna::Framework::GamerServices::GamerServicesNotAvailableException;
using Microsoft::Xna::Framework::GamerServices::Guide;
using Microsoft::Xna::Framework::GamerServices::SignedInGamer;

// Demonstrates SignedInGamer::GetFriends() -- the friends of an online CNA account, and, as in
// XNA, GamerPrivilegeException for a profile that is not signed in to an online account (the
// local profile this catalog signs in) -- alongside a manually-built demo roster via
// FriendGamer::CreateInternal(), which shows FriendGamer's IsOnline/IsPlaying/IsAway fields
// populated without an account service. Also exercises Guide::ShowFriends()/ShowGamerCard():
// both show Guide screens backed by the CNA account service, and throw
// GamerServicesNotAvailableException when no service is configured -- as for the local
// profile this catalog signs in.
class FriendsAndGamerCardScreen : public SignedInGamerDemoScreen {
public:
    FriendsAndGamerCardScreen() : SignedInGamerDemoScreen("GamerServices: Friends & GamerCard") {}

    void OnDemoLoad() override {
        gamer_ = (*Gamer::getSignedInGamersProperty())[PlayerIndex::One];
        demoRoster_.push_back(FriendGamer::CreateInternal("AlphaWolf99", "Alpha", true, true, false, false, false, false));
        demoRoster_.push_back(FriendGamer::CreateInternal("NightOwl", "Night Owl", true, false, true, false, false, false));
        demoRoster_.push_back(FriendGamer::CreateInternal("QuietStorm", "Quiet Storm", false, false, false, false, false, false));
    }

protected:
    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            try {
                auto friends = gamer_->GetFriends();
                friendsResult_ = "Count " + std::to_string(friends.getCountProperty());
                friends.Dispose();
            } catch (const GamerPrivilegeException& ex) {
                friendsResult_ = "threw GamerPrivilegeException: " + ex.getMessageProperty();
            }
        } else if (input.IsNewKeyPress(Keys::F, ControllingPlayer(), playerIndex)) {
            showFriendsCount_++;
            TryShow("ShowFriends", [] { Guide::ShowFriends(PlayerIndex::One); });
        } else if (input.IsNewKeyPress(Keys::C, ControllingPlayer(), playerIndex)) {
            showGamerCardCount_++;
            TryShow("ShowGamerCard", [this] { Guide::ShowGamerCard(PlayerIndex::One, gamer_); });
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: GetFriends()   F: ShowFriends()   C: ShowGamerCard()");
        lines.push_back("GetFriends(): " + (friendsResult_.empty() ? std::string("(not called yet)") : friendsResult_));
        lines.push_back("ShowFriends() calls: " + std::to_string(showFriendsCount_) +
                         "   ShowGamerCard() calls: " + std::to_string(showGamerCardCount_));
        lines.push_back("(both need a CNA account service; without one they throw)");
        if (!lastShowResult_.empty()) lines.push_back("  " + lastShowResult_);
        lines.emplace_back();
        lines.push_back("Manually-built demo roster (FriendGamer::CreateInternal, not real friends):");
        for (const auto& f : demoRoster_) {
            lines.push_back("  " + f.getGamertagProperty() + "  Online=" + (f.getIsOnlineProperty() ? "true" : "false") +
                             "  Playing=" + (f.getIsPlayingProperty() ? "true" : "false") +
                             "  Away=" + (f.getIsAwayProperty() ? "true" : "false"));
        }

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    template <typename Show>
    void TryShow(const std::string& name, Show&& show) {
        try {
            show();
            lastShowResult_ = name + "() opened its Guide screen";
        } catch (const GamerServicesNotAvailableException& ex) {
            lastShowResult_ = name + "() threw GamerServicesNotAvailableException: " +
                              ex.getMessageProperty();
        } catch (const GamerPrivilegeException& ex) {
            lastShowResult_ = name + "() threw GamerPrivilegeException: " + ex.getMessageProperty();
        }
    }

    SignedInGamer* gamer_ = nullptr;
    std::string lastShowResult_;
    std::vector<FriendGamer> demoRoster_;
    std::string friendsResult_;
    int showFriendsCount_ = 0;
    int showGamerCardCount_ = 0;
};

} // namespace CnaExamples::Demos::Net::GamerServicesDemos
