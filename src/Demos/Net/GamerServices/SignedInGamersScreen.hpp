// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/Gamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamerCollection.hpp"
#include "System/EventHandler.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Net::GamerServicesDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::Gamer;
using Microsoft::Xna::Framework::GamerServices::SignedInGamer;

// Demonstrates Gamer::SignedInGamers and the static SignedIn/SignedOut events. As on XNA, a
// profile signed in at startup (CNA_GAMER_SERVICES_AUTO_SIGN_IN, or "autoSignIn" in the profile
// store) appears, raising SignedIn, at the first GamerServicesDispatcher update -- this app's own
// GamerServicesComponent drives it (see CnaExamplesGame.hpp); Guide::ShowSignIn signs one in
// later. With nobody signed in the collection is simply empty.
class SignedInGamersScreen : public DemoScreen {
public:
    SignedInGamersScreen() : DemoScreen("GamerServices: Signed-In Gamers") {}

    void OnDemoLoad() override {
        signedInToken_ = SignedInGamer::SignedIn.Add(
            [this](System::Object*, const auto&) { signedInCount_++; });
        signedOutToken_ = SignedInGamer::SignedOut.Add(
            [this](System::Object*, const auto&) { signedOutCount_++; });
    }

    void OnDemoUnload() override {
        SignedInGamer::SignedIn.Remove(signedInToken_);
        SignedInGamer::SignedOut.Remove(signedOutToken_);
    }

protected:
    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        auto* gamers = Gamer::getSignedInGamersProperty();
        lines.push_back("SignedInGamers.Count: " + std::to_string(gamers->getCountProperty()));
        lines.emplace_back();

        for (SignedInGamer* g : *gamers) {
            lines.push_back(g->getGamertagProperty() + " (" + g->getDisplayNameProperty() + ")" +
                             "  Player" + std::to_string((int)g->getPlayerIndexProperty() + 1) +
                             "  Live=" + (g->getIsSignedInToLiveProperty() ? "true" : "false") +
                             "  Guest=" + (g->getIsGuestProperty() ? "true" : "false"));
        }
        lines.emplace_back();
        lines.push_back("SignedIn fired: " + std::to_string(signedInCount_) +
                         "   SignedOut fired: " + std::to_string(signedOutCount_));
        lines.push_back("(raised as profiles sign in and out)");

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    System::EventHandler<Microsoft::Xna::Framework::GamerServices::SignedInEventArgs>::Token signedInToken_{};
    System::EventHandler<Microsoft::Xna::Framework::GamerServices::SignedOutEventArgs>::Token signedOutToken_{};
    int signedInCount_ = 0;
    int signedOutCount_ = 0;
};

} // namespace CnaExamples::Demos::Net::GamerServicesDemos
