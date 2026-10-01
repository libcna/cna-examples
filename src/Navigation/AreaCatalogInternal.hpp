// SPDX-License-Identifier: MIT
#pragma once

#include <utility>

#include "Demos/DemoScreen.hpp"
#include "Navigation/AreaCatalog.hpp"

namespace CnaExamples::Navigation {

// DemoEntry helper: builds an entry whose factory default-constructs T.
//
// The factory also hands the entry's `apis` list and breadcrumb to the screen
// it creates, so a DemoScreen can render both without having to know how it was
// reached. T must derive from DemoScreen for that to compile, which is exactly
// the constraint we want on a demo registration.
template <typename T>
DemoEntry MakeDemo(std::string title, std::string description,
                   std::vector<std::string> apis = {}) {
    DemoEntry entry;
    entry.title = std::move(title);
    entry.description = std::move(description);
    entry.apis = std::move(apis);

    std::vector<std::string> apisForFactory = entry.apis;
    entry.create = [apisForFactory] {
        auto screen = std::make_shared<T>();
        screen->SetApis(apisForFactory);
        return screen;
    };
    return entry;
}

// Marks every demo in a list as needing a graphics capability, so a backend
// that lacks it shows an explanation instead of running code that throws.
//
// Applied per category at the assembly site rather than inside each screen: CNA
// picks its backend at compile time, whole categories share a requirement, and
// this way a new demo dropped into a gated category inherits the gate instead
// of having to remember it.
inline std::vector<DemoEntry> Requiring(CNA::GraphicsCapability capability,
                                        std::vector<DemoEntry> demos) {
    for (auto& demo : demos) {
        auto inner = demo.create;
        demo.create = [inner, capability] {
            auto screen = inner();
            if (auto* demoScreen = dynamic_cast<Demos::DemoScreen*>(screen.get())) {
                demoScreen->SetRequiredCapability(capability);
            }
            return screen;
        };
    }
    return demos;
}

std::vector<DemoEntry> BuildKeyboardDemos();
std::vector<DemoEntry> BuildMouseDemos();
std::vector<DemoEntry> BuildGamepadDemos();
std::vector<DemoEntry> BuildTouchDemos();
std::vector<DemoEntry> BuildOtherDemos();
std::vector<DemoEntry> BuildSoundEffectDemos();
std::vector<DemoEntry> BuildSoundEffectInstanceDemos();
std::vector<DemoEntry> BuildAudio3DDemos();
std::vector<DemoEntry> BuildDynamicSoundEffectInstanceDemos();
std::vector<DemoEntry> BuildMicrophoneDemos();
std::vector<DemoEntry> BuildXactDemos();
std::vector<DemoEntry> BuildSensorsDemos();
std::vector<DemoEntry> BuildVibrationDemos();
std::vector<DemoEntry> BuildCameraDemos();
std::vector<DemoEntry> BuildSystemAndDisplayDemos();
std::vector<DemoEntry> BuildPowerDemos();
std::vector<DemoEntry> BuildDesktopIntegrationDemos();
std::vector<DemoEntry> BuildNetworkSessionDemos();
std::vector<DemoEntry> BuildNetworkGamerDemos();
std::vector<DemoEntry> BuildGamerServicesDemos();
std::vector<DemoEntry> BuildLeaderboardsDemos();
std::vector<DemoEntry> BuildSongDemos();
std::vector<DemoEntry> BuildVideoDemos();
std::vector<DemoEntry> BuildMediaLibraryDemos();
std::vector<DemoEntry> BuildPictureDemos();
std::vector<DemoEntry> BuildGameLoopDemos();
std::vector<DemoEntry> BuildGameComponentsDemos();
std::vector<DemoEntry> BuildServicesDemos();
std::vector<DemoEntry> BuildWindowDemos();
std::vector<DemoEntry> BuildDeviceManagerDemos();
std::vector<DemoEntry> BuildMathVectorsDemos();
std::vector<DemoEntry> BuildMatrixQuaternionDemos();
std::vector<DemoEntry> BuildGeometryDemos();
std::vector<DemoEntry> BuildCurvesDemos();
std::vector<DemoEntry> BuildColorDemos();
std::vector<DemoEntry> BuildContentBasicsDemos();
std::vector<DemoEntry> BuildContentManifestDemos();
std::vector<DemoEntry> BuildContentXnbDemos();
std::vector<DemoEntry> BuildContentErrorsDemos();
std::vector<DemoEntry> BuildStorageDeviceDemos();
std::vector<DemoEntry> BuildStorageContainerDemos();
std::vector<DemoEntry> BuildDiagnosticsLoggingDemos();
std::vector<DemoEntry> BuildDiagnosticsPlatformDemos();
std::vector<DemoEntry> BuildDiagnosticsCapabilitiesDemos();
std::vector<DemoEntry> BuildDiagnosticsAdapterDemos();
std::vector<DemoEntry> BuildContentCnjDemos();
std::vector<DemoEntry> BuildDrawingBasicsDemos();
std::vector<DemoEntry> BuildSortModesDemos();
std::vector<DemoEntry> BuildDrawStringDemos();
std::vector<DemoEntry> BuildBeginEndStateDemos();
std::vector<DemoEntry> BuildTexture2DBasicsDemos();
std::vector<DemoEntry> BuildSaveAsReloadDemos();
std::vector<DemoEntry> BuildSpriteFontDemos();
std::vector<DemoEntry> BuildBlendStateDemos();
std::vector<DemoEntry> BuildSamplerStateDemos();
std::vector<DemoEntry> BuildViewportScissorDemos();
std::vector<DemoEntry> BuildRenderToTextureBasicsDemos();
std::vector<DemoEntry> BuildScreenTransitionDemos();
std::vector<DemoEntry> BuildDisposeSafetyDemos();
std::vector<DemoEntry> BuildVertexTypesDemos();
std::vector<DemoEntry> BuildPrimitiveTypesDemos();
std::vector<DemoEntry> BuildBuffersDemos();
std::vector<DemoEntry> BuildBasicRenderingDemos();
std::vector<DemoEntry> BuildLightingDemos();
std::vector<DemoEntry> BuildFogDemos();
std::vector<DemoEntry> BuildAlphaTestEffectDemos();
std::vector<DemoEntry> BuildDualTextureEffectDemos();
std::vector<DemoEntry> BuildEnvironmentMapEffectDemos();
std::vector<DemoEntry> BuildSkinnedEffectDemos();
std::vector<DemoEntry> BuildCustomShaderDemos();
std::vector<DemoEntry> BuildPbrEffectDemos();
std::vector<DemoEntry> BuildDepthAndCullingDemos();
std::vector<DemoEntry> BuildCameraAndProjectionDemos();
std::vector<DemoEntry> BuildModelGroupDemos();
std::vector<DemoEntry> BuildTexturesAndQueriesDemos();
std::vector<DemoEntry> BuildEffectReflectionDemos();
std::vector<DemoEntry> BuildAvatarDescriptionDemos();
std::vector<DemoEntry> BuildAvatarRendererDemos();
std::vector<DemoEntry> BuildAvatarWardrobeDemos();

} // namespace CnaExamples::Navigation
