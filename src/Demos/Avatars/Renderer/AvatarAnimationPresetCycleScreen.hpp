// SPDX-License-Identifier: MIT
#pragma once

#include "Demos/Avatars/StandardAvatarScreen.hpp"

namespace CnaExamples::Demos::Avatars::RendererDemos {

class AvatarAnimationPresetCycleScreen : public StandardAvatarScreen {
public:
    AvatarAnimationPresetCycleScreen() : StandardAvatarScreen("Animation Preset Cycle", Mode::Presets) {}
};

} // namespace CnaExamples::Demos::Avatars::RendererDemos
