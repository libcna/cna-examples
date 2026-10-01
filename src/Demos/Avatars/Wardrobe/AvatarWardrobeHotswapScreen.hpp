// SPDX-License-Identifier: MIT
#pragma once

#include "Demos/Avatars/StandardAvatarScreen.hpp"

namespace CnaExamples::Demos::Avatars::WardrobeDemos {

class AvatarWardrobeHotswapScreen : public StandardAvatarScreen {
public:
    AvatarWardrobeHotswapScreen() : StandardAvatarScreen("Random Avatar Variation", Mode::Randomize) {}
};

} // namespace CnaExamples::Demos::Avatars::WardrobeDemos
