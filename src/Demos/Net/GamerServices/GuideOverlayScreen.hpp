// SPDX-License-Identifier: MIT
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/Guide.hpp"
#include "Microsoft/Xna/Framework/GamerServices/MessageBoxIcon.hpp"
#include "System/IAsyncResult.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Net::GamerServicesDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::GamerServices::Guide;
using Microsoft::Xna::Framework::GamerServices::MessageBoxIcon;

// Demonstrates the static Guide overlay API: the on-screen keyboard, a message box, the
// sign-in pane and SimulateTrialMode. Each Begin*/Show* call puts up real Guide UI, drawn
// over the game by the GamerServicesComponent this app installs, and Guide.IsVisible is
// true while it is up (DemoScreen then leaves the input to the Guide).
//
// End* waits for the user's answer, so calling it straight after Begin* would run the Guide
// modally. This screen uses the asynchronous pattern instead: it keeps the IAsyncResult,
// polls IsCompleted in Update, and calls End* only once the user has answered.
class GuideOverlayScreen : public DemoScreen {
public:
    GuideOverlayScreen() : DemoScreen("GamerServices: Guide Overlay") {}

    void OnDemoUnload() override {
        // Leaving with a request still up: answer it, so nothing is left waiting on a
        // screen that no longer exists.
        if (keyboardResult_ != nullptr) {
            if (!keyboardResult_->getIsCompletedProperty()) Guide::SimulateKeyboardInputCancelEXT();
            (void)Guide::EndShowKeyboardInput(keyboardResult_);
            delete keyboardResult_;
            keyboardResult_ = nullptr;
        }
        if (messageBoxResult_ != nullptr) {
            if (!messageBoxResult_->getIsCompletedProperty()) Guide::SimulateMessageBoxClickEXT(1);
            (void)Guide::EndShowMessageBox(messageBoxResult_);
            delete messageBoxResult_;
            messageBoxResult_ = nullptr;
        }
    }

protected:
    void OnDemoUpdate(GameTime&) override {
        if (keyboardResult_ != nullptr && keyboardResult_->getIsCompletedProperty()) {
            const std::string text = Guide::EndShowKeyboardInput(keyboardResult_);
            lastKeyboardResult_ = Guide::WasKeyboardInputCanceledEXT(keyboardResult_)
                                      ? "canceled"
                                      : "\"" + text + "\"";
            delete keyboardResult_;
            keyboardResult_ = nullptr;
        }
        if (messageBoxResult_ != nullptr && messageBoxResult_->getIsCompletedProperty()) {
            const std::optional<int> button = Guide::EndShowMessageBox(messageBoxResult_);
            lastMessageBoxResult_ = button.has_value()
                                        ? "button " + std::to_string(*button) + " chosen"
                                        : std::string("canceled");
            delete messageBoxResult_;
            messageBoxResult_ = nullptr;
        }
    }

    void OnDemoInput(InputState& input) override {
        PlayerIndex playerIndex;
        if (input.IsMenuSelect(ControllingPlayer(), playerIndex) && keyboardResult_ == nullptr) {
            keyboardResult_ = Guide::BeginShowKeyboardInput(
                PlayerIndex::One, "Enter name", "Type, then Enter (Escape cancels)", "default",
                System::AsyncCallback{}, std::any{});
            keyboardCount_++;
        } else if (input.IsNewKeyPress(Keys::M, ControllingPlayer(), playerIndex) &&
                   messageBoxResult_ == nullptr) {
            messageBoxResult_ = Guide::BeginShowMessageBox(
                "Title", "Body", {"OK", "Cancel"}, 0, MessageBoxIcon::None,
                System::AsyncCallback{}, std::any{});
            messageBoxCount_++;
        } else if (input.IsNewKeyPress(Keys::T, ControllingPlayer(), playerIndex)) {
            Guide::setSimulateTrialModeProperty(!Guide::getSimulateTrialModeProperty());
        } else if (input.IsNewKeyPress(Keys::S, ControllingPlayer(), playerIndex)) {
            Guide::ShowSignIn(1, false);
            showSignInCount_++;
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        lines.push_back("Space/Enter/A/tap: BeginShowKeyboardInput   M: BeginShowMessageBox");
        lines.push_back("T: toggle SimulateTrialMode   S: Guide::ShowSignIn()");
        lines.emplace_back();
        lines.push_back("IsTrialMode: " + std::string(Guide::getIsTrialModeProperty() ? "true" : "false") +
                         "  SimulateTrialMode: " + std::string(Guide::getSimulateTrialModeProperty() ? "true" : "false"));
        lines.push_back("IsScreenSaverEnabled: " + std::string(Guide::getIsScreenSaverEnabledProperty() ? "true" : "false"));
        lines.push_back("IsVisible: " + std::string(Guide::getIsVisibleProperty() ? "true" : "false") +
                        " (true while any Guide screen is up)");
        lines.emplace_back();
        lines.push_back("BeginShowKeyboardInput calls: " + std::to_string(keyboardCount_) +
                        (keyboardResult_ != nullptr ? "  (waiting for the user)" : ""));
        if (!lastKeyboardResult_.empty()) lines.push_back("  last result: " + lastKeyboardResult_);
        lines.push_back("BeginShowMessageBox calls: " + std::to_string(messageBoxCount_) +
                        (messageBoxResult_ != nullptr ? "  (waiting for the user)" : ""));
        if (!lastMessageBoxResult_.empty()) lines.push_back("  last result: " + lastMessageBoxResult_);
        lines.push_back("ShowSignIn() calls: " + std::to_string(showSignInCount_));

        DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));
    }

private:
    System::IAsyncResult* keyboardResult_ = nullptr;
    System::IAsyncResult* messageBoxResult_ = nullptr;
    int keyboardCount_ = 0;
    std::string lastKeyboardResult_;
    int messageBoxCount_ = 0;
    std::string lastMessageBoxResult_;
    int showSignInCount_ = 0;
};

} // namespace CnaExamples::Demos::Net::GamerServicesDemos
