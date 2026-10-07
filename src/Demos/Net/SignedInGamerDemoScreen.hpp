// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/Gamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/Guide.hpp"
#include "Microsoft/Xna/Framework/GamerServices/SignedInGamerCollection.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Net {

using namespace CnaExamples::GameStateManagement;

// Base for the Net demos that need a signed-in gamer: creating, finding or
// joining a NetworkSession, and every SignedInGamer member.
//
// As on XNA, no gamer is signed in yet when such a screen loads. A profile that
// signs in at startup (CNA_GAMER_SERVICES_AUTO_SIGN_IN, or "autoSignIn" in the
// profile store) appears at the first GamerServicesDispatcher update; otherwise
// the player signs in through the Guide. The demo therefore loads once a gamer
// is signed in, and offers Guide::ShowSignIn until then -- indexing an empty
// SignedInGamers or creating a session with nobody to put in it aborted these
// demos.
class SignedInGamerDemoScreen : public DemoScreen {
public:
    using DemoScreen::DemoScreen;

protected:
    [[nodiscard]] bool PrerequisitesMet() const override {
        return Microsoft::Xna::Framework::GamerServices::Gamer::getSignedInGamersProperty()
                   ->getCountProperty() > 0;
    }

    void OnNotReadyInput(InputState& input) override {
        using Microsoft::Xna::Framework::GamerServices::Guide;
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex) && !Guide::getIsVisibleProperty()) {
            Guide::ShowSignIn(1, false);
        }
    }

    void DrawNotReady(SpriteBatch& spriteBatch, SpriteFont& font, float alpha) override {
        const std::vector<std::string> lines = {
            "This demo needs a signed-in gamer.",
            "",
            "Space/Enter/A/tap: Guide.ShowSignIn(1, false)",
            "or start with CNA_GAMER_SERVICES_AUTO_SIGN_IN=<profile name>",
        };
        DrawLines(spriteBatch, font, Vector2(40.0f, 90.0f), lines, mul(Color(210, 190, 120), alpha));
    }
};

} // namespace CnaExamples::Demos::Net
