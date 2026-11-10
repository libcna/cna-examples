// SPDX-License-Identifier: MIT
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/Graphics/BasicEffect.hpp"
#include "Microsoft/Xna/Framework/Graphics/EffectParameterCollection.hpp"
#include "Microsoft/Xna/Framework/Graphics/EffectTechnique.hpp"
#include "Microsoft/Xna/Framework/Graphics/EffectTechniqueCollection.hpp"

#include "Demos/DemoScreen.hpp"

namespace CnaExamples::Demos::Graphics3D::EffectReflectionDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::Graphics::BasicEffect;

// XNA's Effect reflection API -- Parameters, Techniques, Passes, Annotations
// -- and what CNA's stock effects put in it. As in XNA, BasicEffect exposes
// the parameters of its compiled shader: WorldViewProj, DiffuseColor,
// DirLight0Direction and the rest, 21 in all, under one technique called
// "BasicEffect". XNA code that reflects over a stock effect therefore ports
// unchanged.
//
// What still needs care is a name the effect does not declare: the lookup
// returns a null pointer rather than throwing, so
//
//     effect.Parameters["NoSuchName"]->SetValue(matrix);   // nullptr deref
//
// must be checked first. See the EffectParameter screen for the parameter
// classes and types themselves.
//
// Everything below is measured from the live objects, not asserted from memory.
class ReflectionSurfaceScreen : public DemoScreen {
public:
    ReflectionSurfaceScreen() : DemoScreen("Effect Reflection: What Is Actually Exposed") {}

    void OnDemoLoad() override {
        auto& device = GetScreenManager()->getGraphicsDeviceProperty();
        effect_.emplace(device);

        parameterCount_ = effect_->getParametersProperty().getCountProperty();
        techniqueCount_ = effect_->getTechniquesProperty().getCountProperty();

        if (auto* current = effect_->getCurrentTechniqueProperty()) {
            currentTechnique_ = current->getNameProperty();
            passCount_ = current->getPassesProperty().getCountProperty();
            annotationCount_ = current->getAnnotationsProperty().getCountProperty();
        }

        worldFound_ = effect_->getParametersProperty()["World"] != nullptr;
        // The remaining trap, exercised rather than described: a by-name
        // lookup of an undeclared name, which code would dereference.
        missingIsNull_ = effect_->getParametersProperty()["NoSuchName"] == nullptr;

        claimHolds_ = parameterCount_ == 21 && techniqueCount_ == 1 &&
                      currentTechnique_ == "BasicEffect" && worldFound_ && missingIsNull_;
    }

    void OnDemoUnload() override { effect_.reset(); }

protected:
    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        const Color tint = mul(Color::White, TransitionAlpha());
        std::vector<std::string> lines;

        lines.push_back("Live BasicEffect on this device:");
        lines.push_back("  Parameters.Count      " + std::to_string(parameterCount_) +
                        "        Techniques.Count  " + std::to_string(techniqueCount_));
        lines.push_back("  CurrentTechnique      \"" + currentTechnique_ + "\"  passes " +
                        std::to_string(passCount_) + ", annotations " +
                        std::to_string(annotationCount_));
        lines.push_back("  Parameters[\"World\"]        " +
                        std::string(worldFound_ ? "found" : "nullptr"));
        lines.push_back("  Parameters[\"NoSuchName\"]   " +
                        std::string(missingIsNull_ ? "nullptr" : "found"));
        lines.emplace_back();
        lines.push_back("As in XNA, the stock effects expose their compiled shader's parameters:");
        lines.push_back("WorldViewProj, DiffuseColor, DirLight0Direction and the rest.");
        lines.emplace_back();
        lines.push_back("A name the effect does not declare returns nullptr and does NOT");
        lines.push_back("throw, so this line must be checked first:");
        lines.push_back("  effect.Parameters[\"NoSuchName\"]->SetValue(m);");
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 82.0f), lines, tint);

        DrawVerdict(sb, font, end.Y + 6.0f,
                    mul(claimHolds_ ? Color(40, 200, 90, 255) : Color(230, 170, 40, 255),
                        TransitionAlpha()),
                    tint,
                    claimHolds_ ? "Verified live: 21 parameters, 1 technique, null for a missing name."
                                : "This renderer reports something else -- numbers above are live.");
    }

private:
    std::optional<BasicEffect> effect_;
    std::string currentTechnique_ = "(none)";
    int parameterCount_ = 0;
    int techniqueCount_ = 0;
    int passCount_ = 0;
    int annotationCount_ = 0;
    bool worldFound_ = false;
    bool missingIsNull_ = false;
    bool claimHolds_ = false;
};

} // namespace CnaExamples::Demos::Graphics3D::EffectReflectionDemos
