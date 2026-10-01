// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Avatars/Description/AvatarDescriptionScreen.hpp"
#include "Demos/Avatars/Description/AvatarNameTablesScreen.hpp"
#include "Demos/Avatars/Renderer/AvatarFaithfulDrawScreen.hpp"
#include "Demos/Avatars/Renderer/AvatarRealRenderScreen.hpp"
#include "Demos/Avatars/Renderer/AvatarAnimationPresetCycleScreen.hpp"
#include "Demos/Avatars/Wardrobe/AvatarAppearanceTintScreen.hpp"
#include "Demos/Avatars/Wardrobe/AvatarWardrobeHotswapScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildAvatarDescriptionDemos() {
    using namespace CnaExamples::Demos::Avatars::DescriptionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<AvatarDescriptionScreen>(
        "Random Body Descriptions",
        "CreateRandom(Female/Male): valid 1021-byte descriptions, body type and height",
        {"AvatarDescription::CreateRandom", "AvatarDescription::IsValid"}));
    demos.push_back(MakeDemo<AvatarNameTablesScreen>(
        "Animation Preset Inventory",
        "Construct all 31 AvatarAnimation presets and inspect duration and 71 bone transforms",
        {"AvatarAnimation", "AvatarAnimationPreset"}));
    return demos;
}

std::vector<DemoEntry> BuildAvatarRendererDemos() {
    using namespace CnaExamples::Demos::Avatars::RendererDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<AvatarFaithfulDrawScreen>(
        "Renderer State & Skeleton",
        "Follow Loading to Ready and inspect ParentBones and BindPose while rendering",
        {"AvatarRenderer", "AvatarRendererState"}));
    demos.push_back(MakeDemo<AvatarRealRenderScreen>(
        "Male & Female Render",
        "Two standard AvatarRenderers draw random male and female bodies side by side",
        {"AvatarRenderer::Draw", "AvatarDescription::CreateRandom"}));
    demos.push_back(MakeDemo<AvatarAnimationPresetCycleScreen>(
        "Animation Preset Cycling",
        "Cycle all 31 standard AvatarAnimation presets and advance them over time",
        {"AvatarAnimation::Update", "AvatarAnimationPreset"}));
    return demos;
}

std::vector<DemoEntry> BuildAvatarWardrobeDemos() {
    using namespace CnaExamples::Demos::Avatars::WardrobeDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<AvatarAppearanceTintScreen>(
        "Avatar Lighting",
        "Alternate AvatarRenderer LightColor between neutral and warm light",
        {"AvatarRenderer::LightColor", "AvatarRenderer::Draw"}));
    demos.push_back(MakeDemo<AvatarWardrobeHotswapScreen>(
        "Random Avatar Variation",
        "Replace a random AvatarDescription and its renderer every three seconds",
        {"AvatarDescription::CreateRandom", "AvatarRenderer"}));
    return demos;
}

} // namespace CnaExamples::Navigation
