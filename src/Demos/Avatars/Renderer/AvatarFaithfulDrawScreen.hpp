// SPDX-License-Identifier: MIT
#pragma once

#include "Demos/Avatars/StandardAvatarScreen.hpp"

namespace CnaExamples::Demos::Avatars::RendererDemos {

class AvatarFaithfulDrawScreen : public StandardAvatarScreen {
public:
    AvatarFaithfulDrawScreen() : StandardAvatarScreen("Renderer State & Skeleton", Mode::Lifecycle) {}
};

} // namespace CnaExamples::Demos::Avatars::RendererDemos
