// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/Net/LocalNetworkGamer.hpp"
#include "Microsoft/Xna/Framework/Net/NetworkMachine.hpp"
#include "Microsoft/Xna/Framework/Net/NetworkSession.hpp"
#include "System/InvalidOperationException.hpp"

#include "Demos/Net/NetDemoHelpers.hpp"
#include "Demos/Net/SignedInGamerDemoScreen.hpp"

namespace CnaExamples::Demos::Net::NetworkGamerDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::Net::LocalNetworkGamer;
using Microsoft::Xna::Framework::Net::NetworkMachine;

// Demonstrates NetworkGamer::Machine (the NetworkMachine grouping gamers that share a
// physical host) and NetworkMachine::RemoveFromSession(). As in XNA, only the host may
// remove another machine; called on the local machine, as here, it throws
// InvalidOperationException.
class NetworkMachineScreen : public SignedInGamerDemoScreen {
public:
    NetworkMachineScreen() : SignedInGamerDemoScreen("NetworkGamer: NetworkMachine") {}

    void OnDemoLoad() override {
        session_ = NetworkSession::Create(NetworkSessionType::Local, 1, 4);
        localGamer_ = session_->getLocalGamersProperty()[0];
    }

    void OnDemoUnload() override {
        EndSession(session_);
    }

protected:
    void OnDemoUpdate(GameTime&) override {
        session_->Update();
    }

    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            attemptCount_++;
            try {
                NetworkMachine machine = localGamer_->getMachineProperty();
                machine.RemoveFromSession();
                lastResult_ = "RemoveFromSession() returned without throwing (unexpected)";
            } catch (const System::InvalidOperationException& ex) {
                lastResult_ = std::string("RemoveFromSession() threw InvalidOperationException: ") + ex.getMessageProperty();
            }
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: call NetworkMachine::RemoveFromSession()");
        lines.emplace_back();
        const NetworkMachine& machine = localGamer_->getMachineProperty();
        lines.push_back("Machine.Gamers.Count: " + std::to_string(machine.getGamersProperty().getCountProperty()));
        lines.emplace_back();
        lines.push_back("Attempts: " + std::to_string(attemptCount_));
        if (!lastResult_.empty()) lines.push_back(lastResult_);

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    NetworkSession* session_ = nullptr;
    LocalNetworkGamer* localGamer_ = nullptr;
    int attemptCount_ = 0;
    std::string lastResult_;
};

} // namespace CnaExamples::Demos::Net::NetworkGamerDemos
