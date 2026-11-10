// SPDX-License-Identifier: MIT
#pragma once

#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/Gamer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/GamerServicesNotAvailableException.hpp"
#include "Microsoft/Xna/Framework/GamerServices/LeaderboardEntry.hpp"
#include "Microsoft/Xna/Framework/GamerServices/LeaderboardIdentity.hpp"
#include "Microsoft/Xna/Framework/GamerServices/LeaderboardKey.hpp"
#include "Microsoft/Xna/Framework/GamerServices/LeaderboardReader.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Net::LeaderboardsDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::GamerServicesNotAvailableException;
using Microsoft::Xna::Framework::GamerServices::LeaderboardIdentity;
using Microsoft::Xna::Framework::GamerServices::LeaderboardKey;
using Microsoft::Xna::Framework::GamerServices::LeaderboardReader;

// Demonstrates LeaderboardIdentity (a pure value type, always constructible/inspectable) and
// LeaderboardReader::Read(). Without an account service the page comes from the local
// leaderboard store and is complete on return; with one it is a remote, paged read. A
// leaderboard nobody has written to yet reads as an empty page, not as an error.
class LeaderboardReaderScreen : public DemoScreen {
public:
    LeaderboardReaderScreen() : DemoScreen("Leaderboards: LeaderboardReader") {}

protected:
    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex)) {
            readAttempts_++;
            entries_.clear();
            try {
                LeaderboardIdentity identity = LeaderboardIdentity::Create(LeaderboardKey::BestScoreLifeTime);
                LeaderboardReader reader = LeaderboardReader::Read(identity, 0, 10);
                const auto entries = reader.getEntriesProperty();
                lastResult_ = "TotalLeaderboardSize " + std::to_string(reader.getTotalLeaderboardSizeProperty()) +
                              ", PageStart " + std::to_string(reader.getPageStartProperty()) +
                              ", Entries.Count " + std::to_string(entries.getCountProperty());
                for (int i = 0; i < entries.getCountProperty() && i < 3; ++i) {
                    const auto& entry = entries[i];
                    const auto* gamer = entry.getGamerProperty();
                    entries_.push_back("  " + (gamer != nullptr ? gamer->getGamertagProperty() : std::string("?")) +
                                       "  Rating " + std::to_string(entry.getRatingProperty()));
                }
                reader.Dispose();
            } catch (const GamerServicesNotAvailableException& ex) {
                lastResult_ = "Read() threw GamerServicesNotAvailableException: " + ex.getMessageProperty();
            }
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: LeaderboardReader::Read(BestScoreLifeTime, 0, 10)");
        lines.emplace_back();
        LeaderboardIdentity identity = LeaderboardIdentity::Create(LeaderboardKey::BestScoreLifeTime, 2);
        lines.push_back("LeaderboardIdentity.Key: \"" + identity.getKeyProperty() + "\"");
        lines.push_back("LeaderboardIdentity.GameMode: " + std::to_string(identity.getGameModeProperty()));
        lines.emplace_back();
        lines.push_back("Read attempts: " + std::to_string(readAttempts_));
        if (!lastResult_.empty()) lines.push_back(lastResult_);
        for (const auto& line : entries_) lines.push_back(line);

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    int readAttempts_ = 0;
    std::string lastResult_;
    std::vector<std::string> entries_;
};

} // namespace CnaExamples::Demos::Net::LeaderboardsDemos
