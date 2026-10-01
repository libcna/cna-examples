// SPDX-License-Identifier: MIT
#pragma once

#include "Demos/Avatars/StandardAvatarScreen.hpp"

namespace CnaExamples::Demos::Avatars::RendererDemos {

class AvatarRealRenderScreen : public StandardAvatarScreen {
public:
    AvatarRealRenderScreen() : StandardAvatarScreen("Male & Female Render", Mode::TwoBodies) {}
};

} // namespace CnaExamples::Demos::Avatars::RendererDemos
