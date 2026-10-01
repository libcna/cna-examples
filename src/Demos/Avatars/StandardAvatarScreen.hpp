// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/GamerServices/AvatarAnimation.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AvatarDescription.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AvatarRenderer.hpp"
#include "Microsoft/Xna/Framework/GamerServices/AvatarRendererState.hpp"
#include "Microsoft/Xna/Framework/Graphics/Viewport.hpp"
#include "Microsoft/Xna/Framework/Input/Keys.hpp"
#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Avatars {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::Matrix;
using Microsoft::Xna::Framework::Vector3;
using Microsoft::Xna::Framework::GamerServices::AvatarAnimation;
using Microsoft::Xna::Framework::GamerServices::AvatarAnimationPreset;
using Microsoft::Xna::Framework::GamerServices::AvatarBodyType;
using Microsoft::Xna::Framework::GamerServices::AvatarDescription;
using Microsoft::Xna::Framework::GamerServices::AvatarRenderer;
using Microsoft::Xna::Framework::GamerServices::AvatarRendererState;
using Microsoft::Xna::Framework::Graphics::Viewport;
using Microsoft::Xna::Framework::Input::Keys;

// Shared scene setup for five distinct demonstrations of CNA's current,
// standard XNA avatar API. The renderer owns its built-in assets; no borrowed
// SkinnedModelEXT files or retired extension methods are needed.
class StandardAvatarScreen : public DemoScreen {
public:
    enum class Mode { Lifecycle, TwoBodies, Presets, Lighting, Randomize };

    StandardAvatarScreen(std::string title, Mode mode)
        : DemoScreen(std::move(title)), mode_(mode) {}

    void OnDemoLoad() override {
        try {
            CreateAvatar(AvatarBodyType::Male);
            if (mode_ == Mode::TwoBodies) {
                secondDescription_ = std::make_unique<AvatarDescription>(
                    AvatarDescription::CreateRandom(AvatarBodyType::Female));
                secondRenderer_ = std::make_unique<AvatarRenderer>(secondDescription_.get(), true);
            }
            animation_ = std::make_unique<AvatarAnimation>(AvatarAnimationPreset::Stand0);
        } catch (const std::exception& ex) {
            error_ = ex.what();
        }
    }

    void OnDemoUnload() override {
        animation_.reset();
        secondRenderer_.reset();
        renderer_.reset();
        secondDescription_.reset();
        description_.reset();
    }

    void OnDemoUpdate(GameTime& gameTime) override {
        if (!animation_ || !renderer_) return;
        animation_->Update(gameTime.getElapsedGameTimeProperty(), true);
        elapsed_ += gameTime.getElapsedGameTimeProperty().getTotalSecondsProperty();
        if (elapsed_ < 3.0) return;
        elapsed_ = 0.0;
        if (mode_ == Mode::Presets) {
            NextPreset();
        } else if (mode_ == Mode::Lighting) {
            lightOn_ = !lightOn_;
            renderer_->setLightColorProperty(lightOn_ ? Vector3(1.0f, 0.35f, 0.2f)
                                                      : Vector3(0.65f, 0.65f, 0.65f));
        } else if (mode_ == Mode::Randomize) {
            CreateAvatar(AvatarBodyType::Male);
            ++randomizations_;
        }
    }

    void OnDemoInput(InputState& input) override {
        PlayerIndex player;
        if (!input.IsNewKeyPress(Keys::Space, ControllingPlayer(), player)) return;
        if (mode_ == Mode::Presets) NextPreset();
        if (mode_ == Mode::Randomize) {
            CreateAvatar(AvatarBodyType::Male);
            ++randomizations_;
        }
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        switch (mode_) {
            case Mode::Lifecycle:
                lines.push_back("A valid AvatarDescription loads into the standard AvatarRenderer.");
                lines.push_back("State: " + StateName(renderer_.get()) + "   ParentBones: " +
                                std::to_string(renderer_ ? renderer_->getParentBonesProperty().getCountProperty() : 0));
                if (renderer_ && renderer_->getStateProperty() == AvatarRendererState::Ready)
                    lines.push_back("BindPose: " +
                                    std::to_string(renderer_->getBindPoseProperty().getCountProperty()) + " bones");
                break;
            case Mode::TwoBodies:
                lines.push_back("Two standard AvatarRenderers draw random male and female bodies.");
                lines.push_back("Male: " + StateName(renderer_.get()) + "   Female: " +
                                StateName(secondRenderer_.get()));
                break;
            case Mode::Presets:
                lines.push_back("AvatarAnimation::Update loops through all 31 standard presets.");
                lines.push_back("Preset index: " + std::to_string(presetIndex_) +
                                "   Length: " + (animation_ ?
                                std::to_string(animation_->getLengthProperty().getTotalSecondsProperty()) : "0") +
                                " s   Space: next");
                break;
            case Mode::Lighting:
                lines.push_back("AvatarRenderer::LightColor alternates every three seconds.");
                lines.push_back(std::string("Current light: ") + (lightOn_ ? "warm red" : "neutral"));
                break;
            case Mode::Randomize:
                lines.push_back("AvatarDescription::CreateRandom supplies a new body every three seconds.");
                lines.push_back("Replacements: " + std::to_string(randomizations_) +
                                "   Height: " + (description_ ?
                                std::to_string(description_->getHeightProperty()) : "0") +
                                " m   Space: replace");
                break;
        }
        if (!error_.empty()) lines.push_back("Avatar error: " + error_);
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 82.0f), lines,
                                      mul(Color::White, TransitionAlpha()));
        const bool valid = error_.empty() && description_ && description_->getIsValidProperty() &&
                           animation_ && renderer_;
        DrawVerdict(sb, font, end.Y + 8.0f,
                    valid ? Color(60, 200, 90, 255) : Color(210, 60, 60, 255),
                    mul(Color::White, TransitionAlpha()),
                    valid ? "PASS: standard avatar objects active" : "FAIL: avatar unavailable");
        if (!valid) return;

        sb.End();
        auto& device = GetScreenManager()->getGraphicsDeviceProperty();
        const Viewport original = device.getViewportProperty();
        const int top = std::min((int)end.Y + 55, original.getHeightProperty() / 2);
        const int height = std::max(80, original.getHeightProperty() - top - 60);
        try {
            if (mode_ == Mode::TwoBodies && secondRenderer_) {
                const int half = original.getWidthProperty() / 2;
                DrawOne(device, *renderer_, *description_, Viewport(0, top, half, height));
                DrawOne(device, *secondRenderer_, *secondDescription_,
                        Viewport(half, top, original.getWidthProperty() - half, height));
            } else {
                DrawOne(device, *renderer_, *description_,
                        Viewport(0, top, original.getWidthProperty(), height));
            }
        } catch (const std::exception& ex) {
            error_ = ex.what();
        }
        device.setViewportProperty(original);
        sb.Begin();
    }

private:
    static std::string StateName(const AvatarRenderer* renderer) {
        if (!renderer) return "Unavailable";
        switch (renderer->getStateProperty()) {
            case AvatarRendererState::Loading: return "Loading";
            case AvatarRendererState::Ready: return "Ready";
            case AvatarRendererState::Unavailable: return "Unavailable";
        }
        return "Unknown";
    }

    void CreateAvatar(AvatarBodyType bodyType) {
        renderer_.reset();
        description_ = std::make_unique<AvatarDescription>(AvatarDescription::CreateRandom(bodyType));
        renderer_ = std::make_unique<AvatarRenderer>(description_.get(), true);
    }

    void NextPreset() {
        try {
            presetIndex_ = (presetIndex_ + 1) % 31;
            animation_ = std::make_unique<AvatarAnimation>(
                static_cast<AvatarAnimationPreset>(presetIndex_));
        } catch (const std::exception& ex) {
            error_ = ex.what();
        }
    }

    void DrawOne(Microsoft::Xna::Framework::Graphics::GraphicsDevice& device,
                 AvatarRenderer& renderer, const AvatarDescription& description,
                 const Viewport& viewport) {
        device.setViewportProperty(viewport);
        const float height = std::max(description.getHeightProperty(), 1.4f);
        renderer.setWorldProperty(Matrix::getIdentityProperty());
        renderer.setViewProperty(Matrix::CreateLookAt(Vector3(0.0f, height * 0.6f, 3.0f),
                                                      Vector3(0.0f, height * 0.52f, 0.0f), Vector3::Up));
        renderer.setProjectionProperty(Matrix::CreatePerspectiveFieldOfView(
            0.78539816f, viewport.getAspectRatioProperty(), 0.1f, 100.0f));
        renderer.Draw(animation_.get());
    }

    Mode mode_;
    std::unique_ptr<AvatarDescription> description_;
    std::unique_ptr<AvatarDescription> secondDescription_;
    std::unique_ptr<AvatarRenderer> renderer_;
    std::unique_ptr<AvatarRenderer> secondRenderer_;
    std::unique_ptr<AvatarAnimation> animation_;
    std::string error_;
    int presetIndex_ = 0;
    int randomizations_ = 0;
    double elapsed_ = 0.0;
    bool lightOn_ = false;
};

} // namespace CnaExamples::Demos::Avatars
