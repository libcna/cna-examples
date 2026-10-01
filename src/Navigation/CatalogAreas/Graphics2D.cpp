// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Graphics2D/DrawingBasics/PositionDrawScreen.hpp"
#include "Demos/Graphics2D/DrawingBasics/DestinationRectangleScreen.hpp"
#include "Demos/Graphics2D/DrawingBasics/SourceRectangleScreen.hpp"
#include "Demos/Graphics2D/DrawingBasics/RotationOriginScreen.hpp"
#include "Demos/Graphics2D/DrawingBasics/ScaleAndEffectsScreen.hpp"
#include "Demos/Graphics2D/SortModes/DeferredSortScreen.hpp"
#include "Demos/Graphics2D/SortModes/ImmediateSortScreen.hpp"
#include "Demos/Graphics2D/SortModes/TextureSortScreen.hpp"
#include "Demos/Graphics2D/SortModes/BackToFrontSortScreen.hpp"
#include "Demos/Graphics2D/SortModes/FrontToBackSortScreen.hpp"
#include "Demos/Graphics2D/DrawString/BasicTextScreen.hpp"
#include "Demos/Graphics2D/DrawString/TransformedTextScreen.hpp"
#include "Demos/Graphics2D/DrawString/FlippedTextScreen.hpp"
#include "Demos/Graphics2D/DrawString/StringBuilderScreen.hpp"
#include "Demos/Graphics2D/BeginEndState/NestedBeginThrowsScreen.hpp"
#include "Demos/Graphics2D/BeginEndState/DrawOutsideBeginThrowsScreen.hpp"
#include "Demos/Graphics2D/BeginEndState/BlendStateLeakScreen.hpp"
#include "Demos/Graphics2D/BeginEndState/TransformMatrixScreen.hpp"
#include "Demos/Graphics2D/Texture2DBasics/ProceduralCreationScreen.hpp"
#include "Demos/Graphics2D/Texture2DBasics/SetDataGetDataRoundTripScreen.hpp"
#include "Demos/Graphics2D/Texture2DBasics/FromStreamFormatsScreen.hpp"
#include "Demos/Graphics2D/Texture2DBasics/SurfaceFormatMatrixScreen.hpp"
#include "Demos/Graphics2D/Texture2DBasics/PropertiesAndDisposeScreen.hpp"
#include "Demos/Graphics2D/SaveAsReload/SaveAsPngRoundTripScreen.hpp"
#include "Demos/Graphics2D/SaveAsReload/SaveAsJpegRoundTripScreen.hpp"
#include "Demos/Graphics2D/SpriteFont/MeasureStringScreen.hpp"
#include "Demos/Graphics2D/SpriteFont/DefaultCharacterFallbackScreen.hpp"
#include "Demos/Graphics2D/SpriteFont/LineSpacingAndSpacingScreen.hpp"
#include "Demos/Graphics2D/SpriteFont/HandBuiltFontScreen.hpp"
#include "Demos/Graphics2D/BlendState/BlendModeComparisonScreen.hpp"
#include "Demos/Graphics2D/BlendState/PremultipliedAlphaGotchaScreen.hpp"
#include "Demos/Graphics2D/SamplerState/FilterComparisonScreen.hpp"
#include "Demos/Graphics2D/SamplerState/AddressModeScreen.hpp"
#include "Demos/Graphics2D/ViewportScissor/ViewportInspectorScreen.hpp"
#include "Demos/Graphics2D/ViewportScissor/ScissorClippingScreen.hpp"
#include "Demos/Graphics2D/ViewportScissor/ViewportScissorResetGotchaScreen.hpp"
#include "Demos/Graphics2D/RenderToTextureBasics/DrawIntoRenderTargetScreen.hpp"
#include "Demos/Graphics2D/RenderToTextureBasics/MultiTargetRoundTripScreen.hpp"
#include "Demos/Graphics2D/ScreenTransition/FadeTransitionScreen.hpp"
#include "Demos/Graphics2D/DisposeSafety/DisposeWhileBoundThrowsScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildDrawingBasicsDemos() {
    using namespace CnaExamples::Demos::Graphics2D::DrawingBasicsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PositionDrawScreen>(
        "Position Draw", "Draw(texture, Vector2 position, Color) + the NOXNA float x,y overload"));
    demos.push_back(MakeDemo<DestinationRectangleScreen>(
        "Destination Rectangle", "Stretching a texture to fill a Rectangle, 3 sizes side by side"));
    demos.push_back(MakeDemo<SourceRectangleScreen>(
        "Source Rectangle", "Cropping one cell out of a 4x4 sprite-sheet atlas"));
    demos.push_back(MakeDemo<RotationOriginScreen>(
        "Rotation & Origin", "Spin-in-place vs. orbit -- the same rotation, two different origins"));
    demos.push_back(MakeDemo<ScaleAndEffectsScreen>(
        "Scale & SpriteEffects", "float vs. Vector2 scale + FlipHorizontally/FlipVertically"));
    return demos;
}

std::vector<DemoEntry> BuildSortModesDemos() {
    using namespace CnaExamples::Demos::Graphics2D::SortModesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DeferredSortScreen>(
        "Deferred", "The default -- submission order preserved, depth ignored"));
    demos.push_back(MakeDemo<ImmediateSortScreen>(
        "Immediate", "Each Draw() flushes as its own GPU call, not queued"));
    demos.push_back(MakeDemo<TextureSortScreen>(
        "Texture", "Sorted by raw Texture2D* pointer value -- literally, not a smart heuristic"));
    demos.push_back(MakeDemo<BackToFrontSortScreen>(
        "BackToFront", "Sorted by layerDepth descending -- largest depth drawn first"));
    demos.push_back(MakeDemo<FrontToBackSortScreen>(
        "FrontToBack", "Sorted by layerDepth ascending -- opposite top layer from BackToFront"));
    return demos;
}

std::vector<DemoEntry> BuildDrawStringDemos() {
    using namespace CnaExamples::Demos::Graphics2D::DrawStringDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<BasicTextScreen>(
        "Basics", "DrawString() + MeasureString() side by side"));
    demos.push_back(MakeDemo<TransformedTextScreen>(
        "Transform", "Rotation/origin/scale on text, animated live"));
    demos.push_back(MakeDemo<FlippedTextScreen>(
        "SpriteEffects & Newline Quirk", "Whole-string mirroring + a real MeasureString(\"\\n\") gotcha"));
    demos.push_back(MakeDemo<StringBuilderScreen>(
        "StringBuilder Overload", "The same DrawString()/MeasureString() surface, fed a StringBuilder"));
    return demos;
}

std::vector<DemoEntry> BuildBeginEndStateDemos() {
    using namespace CnaExamples::Demos::Graphics2D::BeginEndStateDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<NestedBeginThrowsScreen>(
        "Nested Begin() Throws", "Begin() twice without an intervening End()"));
    demos.push_back(MakeDemo<DrawOutsideBeginThrowsScreen>(
        "Draw Outside Begin() Throws", "Draw()/DrawString() with no open Begin() session"));
    demos.push_back(MakeDemo<BlendStateLeakScreen>(
        "BlendState Leaks Past End()", "A real, documented gotcha -- End() never restores device state"));
    demos.push_back(MakeDemo<TransformMatrixScreen>(
        "Begin() Transform Matrix", "The full 7-arg Begin() overload -- an orbiting-camera matrix"));
    return demos;
}

std::vector<DemoEntry> BuildTexture2DBasicsDemos() {
    using namespace CnaExamples::Demos::Graphics2D::Texture2DBasicsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ProceduralCreationScreen>(
        "Procedural Creation", "CreateFromPixels (NOXNA) vs. Texture2D(w,h)+SetData (XNA-idiomatic)"));
    demos.push_back(MakeDemo<SetDataGetDataRoundTripScreen>(
        "SetData/GetData Round-Trip", "Write a known pattern, read it back, verify byte-for-byte"));
    demos.push_back(MakeDemo<PropertiesAndDisposeScreen>(
        "Properties & Dispose", "Width/Height/Bounds + the IsDisposed/HasBackend lifecycle"));
    demos.push_back(MakeDemo<SurfaceFormatMatrixScreen>(
        "SurfaceFormat Matrix",
        "All 19 formats constructed for real -- there is no SupportsFormat() query",
        {"Texture2D", "SurfaceFormat"}));
    demos.push_back(MakeDemo<FromStreamFormatsScreen>(
        "FromStream Formats & Failures",
        "A real PNG, a hand-built BMP, and what happens on empty/noise/truncated bytes",
        {"Texture2D"}));
    return demos;
}

std::vector<DemoEntry> BuildSaveAsReloadDemos() {
    using namespace CnaExamples::Demos::Graphics2D::SaveAsReloadDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<SaveAsPngRoundTripScreen>(
        "SaveAsPng Round-Trip", "A real PNG file, written then reloaded from disk -- not a stub"));
    demos.push_back(MakeDemo<SaveAsJpegRoundTripScreen>(
        "SaveAsJpeg Round-Trip", "Same pattern -- honestly notes JPEG's lossy compression"));
    return demos;
}

std::vector<DemoEntry> BuildSpriteFontDemos() {
    using namespace CnaExamples::Demos::Graphics2D::SpriteFontDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<MeasureStringScreen>(
        "MeasureString", "Real measured width/height for short, long, and multi-line strings"));
    demos.push_back(MakeDemo<DefaultCharacterFallbackScreen>(
        "DefaultCharacter Fallback", "With a fallback set vs. unset -- one throws, one doesn't"));
    demos.push_back(MakeDemo<LineSpacingAndSpacingScreen>(
        "LineSpacing & Spacing", "Live-adjustable, on the app's own shared menu font"));
    demos.push_back(MakeDemo<HandBuiltFontScreen>(
        "Hand-Built From Scratch", "A brand-new SpriteFont via its public NOXNA constructor"));
    return demos;
}

std::vector<DemoEntry> BuildBlendStateDemos() {
    using namespace CnaExamples::Demos::Graphics2D::BlendStateDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<BlendModeComparisonScreen>(
        "Mode Comparison", "Opaque/AlphaBlend/Additive/NonPremultiplied on overlapping quads"));
    demos.push_back(MakeDemo<PremultipliedAlphaGotchaScreen>(
        "Premultiplied Alpha", "The classic mismatched-pairing gotcha, shown correct vs. wrong"));
    return demos;
}

std::vector<DemoEntry> BuildSamplerStateDemos() {
    using namespace CnaExamples::Demos::Graphics2D::SamplerStateDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<FilterComparisonScreen>(
        "Filter Comparison", "PointClamp (crisp) vs. LinearClamp (smooth) on an upscaled texture"));
    demos.push_back(MakeDemo<AddressModeScreen>(
        "Address Mode", "Wrap (tiles) vs. Clamp (smears) via an oversized source rectangle"));
    return demos;
}

std::vector<DemoEntry> BuildViewportScissorDemos() {
    using namespace CnaExamples::Demos::Graphics2D::ViewportScissorDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ViewportInspectorScreen>(
        "Inspector", "Real live Viewport X/Y/Width/Height/AspectRatio, temporarily resizable"));
    demos.push_back(MakeDemo<ScissorClippingScreen>(
        "Scissor Clipping", "GraphicsDevice.ScissorRectangle + RasterizerState, set directly"));
    demos.push_back(MakeDemo<ViewportScissorResetGotchaScreen>(
        "Reset on RT Switch", "A real gotcha -- SetRenderTarget silently resets both"));
    return demos;
}

std::vector<DemoEntry> BuildRenderToTextureBasicsDemos() {
    using namespace CnaExamples::Demos::Graphics2D::RenderToTextureBasicsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DrawIntoRenderTargetScreen>(
        "Draw Into & Sample", "Render into a RenderTarget2D once, then draw it like any texture"));
    demos.push_back(MakeDemo<MultiTargetRoundTripScreen>(
        "Multi-Target Round Trip", "Switching between 2 render targets and the backbuffer"));
    return demos;
}

std::vector<DemoEntry> BuildScreenTransitionDemos() {
    using namespace CnaExamples::Demos::Graphics2D::ScreenTransitionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<FadeTransitionScreen>(
        "Fade Transition Effect", "A live, animated render-target-as-post-effect-surface demo"));
    return demos;
}

std::vector<DemoEntry> BuildDisposeSafetyDemos() {
    using namespace CnaExamples::Demos::Graphics2D::DisposeSafetyDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DisposeWhileBoundThrowsScreen>(
        "Dispose While Bound Throws", "Disposing a RenderTarget2D that's still the active target"));
    return demos;
}

} // namespace CnaExamples::Navigation
