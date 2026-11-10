// SPDX-License-Identifier: MIT
#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Microsoft/Xna/Framework/Graphics/BasicEffect.hpp"
#include "Microsoft/Xna/Framework/Graphics/DepthStencilState.hpp"
#include "Microsoft/Xna/Framework/Graphics/PrimitiveType.hpp"
#include "Microsoft/Xna/Framework/Graphics/RasterizerState.hpp"
#include "Microsoft/Xna/Framework/Graphics/Viewport.hpp"
#include "Microsoft/Xna/Framework/MathHelper.hpp"
#include "Microsoft/Xna/Framework/Matrix.hpp"

#include "Demos/DemoScreen.hpp"
#include "Demos/Graphics3D/Geometry3DHelpers.hpp"

namespace CnaExamples::Demos::Graphics3D::BuffersDemos {

using namespace CnaExamples::GameStateManagement;
using Microsoft::Xna::Framework::MathHelper;
using Microsoft::Xna::Framework::Matrix;
using Microsoft::Xna::Framework::Graphics::BasicEffect;
using Microsoft::Xna::Framework::Graphics::DepthStencilState;
using Microsoft::Xna::Framework::Graphics::PrimitiveType;
using Microsoft::Xna::Framework::Graphics::RasterizerState;
using Microsoft::Xna::Framework::Graphics::Viewport;

// Demonstrates why a vertex declaration's stride is not sizeof() of the C++
// vertex structure. CNA's stock vertex types are polymorphic C++ objects
// (IVertexType gives them a vtable, and Color is a 16-byte object here rather
// than XNA's 4-byte value), so sizeof(VertexPositionColor) is larger than the
// 16-byte GPU stream XNA defines. VertexPositionColor's VertexDeclaration
// describes that GPU stream, not the C++ object.
//
// Both cubes draw the same vertices. The left one uses the typed overload; the
// right one passes VertexPositionColor's own declaration as well. CNA converts
// the values into the stream that declaration describes, so the two match.
// The trap that remains is a custom vertex structure embedding a Color: it is
// not trivially copyable, so the declaration-taking overload refuses it at
// compile time. Store the colour's packed std::uint32_t value instead.
class VertexDeclarationGotchaScreen : public DemoScreen {
public:
    VertexDeclarationGotchaScreen() : DemoScreen("Buffers: VertexDeclaration Stride Gotcha") {}

    void OnDemoLoad() override {
        auto& device = GetScreenManager()->getGraphicsDeviceProperty();
        mesh_ = BuildCubeColorMesh(0.8f, {
            Color(220, 60, 60, 255), Color(60, 220, 90, 255), Color(60, 110, 220, 255),
            Color(220, 200, 60, 255), Color(200, 60, 220, 255), Color(60, 220, 210, 255),
        });
        effect_.emplace(device);
        effect_->VertexColorEnabled = true;
    }

    void OnDemoUnload() override {
        effect_.reset();
    }

protected:
    void OnDemoUpdate(GameTime& gameTime) override {
        spin_ += (float)gameTime.getElapsedGameTimeProperty().getTotalSecondsProperty();
    }

    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        std::vector<std::string> lines;
        const int stride = VertexPositionColor::getVertexDeclarationStatic().getVertexStrideProperty();
        lines.push_back("Left: typed DrawUserIndexedPrimitives(VertexPositionColor*, ...).");
        lines.push_back("Right: the SAME data plus VertexPositionColor's own VertexDeclaration.");
        lines.push_back("sizeof(VertexPositionColor) = " + std::to_string(sizeof(VertexPositionColor)) +
                        " bytes, declared stride = " + std::to_string(stride) + ".");
        lines.push_back("The declaration describes the GPU stream, not the C++ object.");
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 90.0f), lines, mul(Color::White, TransitionAlpha()));

        sb.End();

        auto& device = GetScreenManager()->getGraphicsDeviceProperty();
        const Viewport original = device.getViewportProperty();
        Viewport scene(0, (int)end.Y + 20, original.getWidthProperty(), 380);
        device.setViewportProperty(scene);
        device.setDepthStencilStateProperty(DepthStencilState::Default);
        device.setRasterizerStateProperty(RasterizerState::CullNone);

        effect_->View = Matrix::CreateLookAt(Vector3(0.0f, 1.2f, 4.0f), Vector3::Zero, Vector3(0.0f, 1.0f, 0.0f));
        effect_->Projection = Matrix::CreatePerspectiveFieldOfView(
            MathHelper::PiOver4, (float)scene.getWidthProperty() / (float)scene.getHeightProperty(), 0.1f, 100.0f);
        const Matrix spin = Matrix::CreateRotationY(spin_);

        // The typed overload packs the values into the stock GPU stream.
        effect_->World = Matrix::CreateTranslation(-1.4f, 0.0f, 0.0f) * spin;
        effect_->Apply();
        device.DrawUserIndexedPrimitives(PrimitiveType::TriangleList, mesh_.vertices.data(), 0,
                                         (int)mesh_.vertices.size(), mesh_.indices.data(), 0,
                                         (int)mesh_.indices.size() / 3);

        // With the type's own declaration the values are converted into the
        // stream it describes, never read as raw C++ object bytes.
        effect_->World = Matrix::CreateTranslation(1.4f, 0.0f, 0.0f) * spin;
        effect_->Apply();
        device.DrawUserIndexedPrimitives(PrimitiveType::TriangleList, mesh_.vertices.data(), 0,
                                         (int)mesh_.vertices.size(), mesh_.indices.data(), 0,
                                         (int)mesh_.indices.size() / 3,
                                         VertexPositionColor::getVertexDeclarationStatic());

        device.setViewportProperty(original);
        device.setRasterizerStateProperty(RasterizerState::CullCounterClockwise);
        device.setDepthStencilStateProperty(DepthStencilState::None);
        sb.Begin();

        const Color tint = mul(Color::White, TransitionAlpha());
        sb.DrawString(font, "Typed overload", Vector2(40.0f, (float)original.getHeightProperty() - 60.0f), tint);
        sb.DrawString(font, "+ its VertexDeclaration",
                      Vector2(540.0f, (float)original.getHeightProperty() - 60.0f), tint);
    }

private:
    CubeColorMesh mesh_;
    std::optional<BasicEffect> effect_;
    float spin_ = 0.0f;
};

} // namespace CnaExamples::Demos::Graphics3D::BuffersDemos
