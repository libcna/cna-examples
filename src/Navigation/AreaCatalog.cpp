// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

namespace CnaExamples::Navigation {

// Builds the full Home -> Area -> Category -> Demo data set. This is the
// single place new areas/categories/demos get registered as they are
// implemented; see plan.md section 8 for what is intentionally still empty.
std::vector<AreaEntry> BuildAreaCatalog() {
    return {
        AreaEntry{"Framework", {
            CategoryEntry{"Game Loop", BuildGameLoopDemos()},
            CategoryEntry{"Game Components", BuildGameComponentsDemos()},
            CategoryEntry{"Services & Dispatcher", BuildServicesDemos()},
            CategoryEntry{"Window", BuildWindowDemos()},
            CategoryEntry{"Device Manager", BuildDeviceManagerDemos()},
        }, {}},
        AreaEntry{"Math", {
            CategoryEntry{"Vectors", BuildMathVectorsDemos()},
            CategoryEntry{"Matrix & Quaternion", BuildMatrixQuaternionDemos()},
            CategoryEntry{"Geometry", BuildGeometryDemos()},
            CategoryEntry{"Curves", BuildCurvesDemos()},
            CategoryEntry{"Color & Packed Vectors", BuildColorDemos()},
        }, {}},
        AreaEntry{"Content", {
            CategoryEntry{"ContentManager Basics", BuildContentBasicsDemos()},
            CategoryEntry{"Manifest", BuildContentManifestDemos()},
            CategoryEntry{"CNJ Format", BuildContentCnjDemos()},
            CategoryEntry{"XNB Format", BuildContentXnbDemos()},
            CategoryEntry{"Errors", BuildContentErrorsDemos()},
        }, {}},
        AreaEntry{"Storage", {
            CategoryEntry{"StorageDevice", BuildStorageDeviceDemos()},
            CategoryEntry{"StorageContainer", BuildStorageContainerDemos()},
        }, {}},
        AreaEntry{"Diagnostics", {
            CategoryEntry{"Logging", BuildDiagnosticsLoggingDemos()},
            CategoryEntry{"Platform & Build", BuildDiagnosticsPlatformDemos()},
            CategoryEntry{"Backend & Capabilities", BuildDiagnosticsCapabilitiesDemos()},
            CategoryEntry{"Adapter & Display", BuildDiagnosticsAdapterDemos()},
        }, {}},
        AreaEntry{"Input", {
            CategoryEntry{"Keyboard", BuildKeyboardDemos()},
            CategoryEntry{"Mouse", BuildMouseDemos()},
            CategoryEntry{"Gamepad", BuildGamepadDemos()},
            CategoryEntry{"Touch", BuildTouchDemos()},
            CategoryEntry{"Other", BuildOtherDemos()},
        }, {}},
        AreaEntry{"Audio", {
            CategoryEntry{"SoundEffect", BuildSoundEffectDemos()},
            CategoryEntry{"SoundEffectInstance", BuildSoundEffectInstanceDemos()},
            CategoryEntry{"3D Audio", BuildAudio3DDemos()},
            CategoryEntry{"DynamicSoundEffectInstance", BuildDynamicSoundEffectInstanceDemos()},
            CategoryEntry{"Microphone", BuildMicrophoneDemos()},
            CategoryEntry{"XACT", BuildXactDemos()},
        }, {}},
        AreaEntry{"Devices", {
            CategoryEntry{"Sensors", BuildSensorsDemos()},
            CategoryEntry{"Vibration", BuildVibrationDemos()},
            CategoryEntry{"Camera", BuildCameraDemos()},
            CategoryEntry{"System & Display", BuildSystemAndDisplayDemos()},
            CategoryEntry{"Power", BuildPowerDemos()},
            CategoryEntry{"Desktop Integration", BuildDesktopIntegrationDemos()},
        }, {}},
        AreaEntry{"Net", {
            CategoryEntry{"NetworkSession", BuildNetworkSessionDemos()},
            CategoryEntry{"NetworkGamer", BuildNetworkGamerDemos()},
            CategoryEntry{"GamerServices", BuildGamerServicesDemos()},
            CategoryEntry{"Leaderboards", BuildLeaderboardsDemos()},
        }, {}},
        // Description and animation inventory need no graphics device. The
        // remaining demos draw real standard avatars and require a 3D backend.
        AreaEntry{"Avatars", {
            CategoryEntry{"AvatarDescription", BuildAvatarDescriptionDemos()},
            CategoryEntry{"AvatarRenderer", Requiring(CNA::GraphicsCapability::ThreeD, BuildAvatarRendererDemos())},
            CategoryEntry{"Lighting & Variation", Requiring(CNA::GraphicsCapability::ThreeD, BuildAvatarWardrobeDemos())},
        }, {}},
        AreaEntry{"Media", {
            CategoryEntry{"Song", BuildSongDemos()},
#if !defined(__EMSCRIPTEN__)
            CategoryEntry{"Video", BuildVideoDemos()},
#endif
            CategoryEntry{"MediaLibrary", BuildMediaLibraryDemos()},
            CategoryEntry{"Pictures", BuildPictureDemos()},
        }, {}},
        AreaEntry{"2D Graphics", {}, {
            GroupEntry{"SpriteBatch", {
                CategoryEntry{"Drawing Basics", BuildDrawingBasicsDemos()},
                CategoryEntry{"Sort Modes", BuildSortModesDemos()},
                CategoryEntry{"DrawString", BuildDrawStringDemos()},
                CategoryEntry{"Begin/End & State", BuildBeginEndStateDemos()},
            }},
            GroupEntry{"Textures & Fonts", {
                CategoryEntry{"Texture2D Basics", BuildTexture2DBasicsDemos()},
                CategoryEntry{"SaveAs & Reload", BuildSaveAsReloadDemos()},
                CategoryEntry{"SpriteFont", BuildSpriteFontDemos()},
            }},
            GroupEntry{"Device State & Blending", {
                CategoryEntry{"BlendState", BuildBlendStateDemos()},
                CategoryEntry{"SamplerState", BuildSamplerStateDemos()},
                CategoryEntry{"Viewport & Scissor", BuildViewportScissorDemos()},
            }},
            GroupEntry{"Render Targets", {
                CategoryEntry{"Render-to-Texture Basics", BuildRenderToTextureBasicsDemos()},
                CategoryEntry{"Screen Transition", BuildScreenTransitionDemos()},
                CategoryEntry{"Dispose Safety", BuildDisposeSafetyDemos()},
            }},
        }},
        // Every category here needs the 3D pipeline, which SDL_RENDERER, DX3
        // and CANVAS do not have -- see Requiring()'s own comment.
        AreaEntry{"3D Graphics", {}, {
            GroupEntry{"Primitives & Vertex Types", {
                CategoryEntry{"Vertex Types", Requiring(CNA::GraphicsCapability::ThreeD, BuildVertexTypesDemos())},
                CategoryEntry{"Primitive Types", Requiring(CNA::GraphicsCapability::ThreeD, BuildPrimitiveTypesDemos())},
                CategoryEntry{"Buffers", Requiring(CNA::GraphicsCapability::ThreeD, BuildBuffersDemos())},
            }},
            GroupEntry{"BasicEffect & Lighting", {
                CategoryEntry{"Basic Rendering", Requiring(CNA::GraphicsCapability::ThreeD, BuildBasicRenderingDemos())},
                CategoryEntry{"Lighting", Requiring(CNA::GraphicsCapability::ThreeD, BuildLightingDemos())},
                CategoryEntry{"Fog", Requiring(CNA::GraphicsCapability::ThreeD, BuildFogDemos())},
            }},
            GroupEntry{"Effects Gallery", {
                CategoryEntry{"AlphaTestEffect", Requiring(CNA::GraphicsCapability::ThreeD, BuildAlphaTestEffectDemos())},
                CategoryEntry{"DualTextureEffect", Requiring(CNA::GraphicsCapability::ThreeD, BuildDualTextureEffectDemos())},
                CategoryEntry{"EnvironmentMapEffect", Requiring(CNA::GraphicsCapability::ThreeD, BuildEnvironmentMapEffectDemos())},
                CategoryEntry{"SkinnedEffect", Requiring(CNA::GraphicsCapability::ThreeD, BuildSkinnedEffectDemos())},
                CategoryEntry{"Custom Shader", Requiring(CNA::GraphicsCapability::ThreeD, BuildCustomShaderDemos())},
                CategoryEntry{"PbrEffect", Requiring(CNA::GraphicsCapability::ThreeD, BuildPbrEffectDemos())},
            }},
            GroupEntry{"Device State, Camera & Model", {
                CategoryEntry{"Depth & Culling", Requiring(CNA::GraphicsCapability::ThreeD, BuildDepthAndCullingDemos())},
                CategoryEntry{"Camera & Projection", Requiring(CNA::GraphicsCapability::ThreeD, BuildCameraAndProjectionDemos())},
                CategoryEntry{"Model", Requiring(CNA::GraphicsCapability::ThreeD, BuildModelGroupDemos())},
            }},
            GroupEntry{"Textures, Effects & Queries", {
                CategoryEntry{"Volume & Cube Textures", Requiring(CNA::GraphicsCapability::ThreeD, BuildTexturesAndQueriesDemos())},
                CategoryEntry{"Effect Reflection", Requiring(CNA::GraphicsCapability::ThreeD, BuildEffectReflectionDemos())},
            }},
        }},
    };
}

} // namespace CnaExamples::Navigation
