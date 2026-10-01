// SPDX-License-Identifier: MIT
#pragma once

#include "Demos/Avatars/StandardAvatarScreen.hpp"

namespace CnaExamples::Demos::Avatars::WardrobeDemos {

class AvatarAppearanceTintScreen : public StandardAvatarScreen {
public:
    AvatarAppearanceTintScreen() : StandardAvatarScreen("Avatar Lighting", Mode::Lighting) {}
};

} // namespace CnaExamples::Demos::Avatars::WardrobeDemos
