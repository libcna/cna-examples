// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Graphics3D/VertexTypes/PositionColorScreen.hpp"
#include "Demos/Graphics3D/VertexTypes/PositionTextureScreen.hpp"
#include "Demos/Graphics3D/VertexTypes/PositionNormalTextureScreen.hpp"
#include "Demos/Graphics3D/PrimitiveTypes/TriangleListVsStripScreen.hpp"
#include "Demos/Graphics3D/PrimitiveTypes/LineAndPointScreen.hpp"
#include "Demos/Graphics3D/Buffers/ImmediateVsBufferedScreen.hpp"
#include "Demos/Graphics3D/Buffers/DynamicVertexBufferScreen.hpp"
#include "Demos/Graphics3D/Buffers/VertexDeclarationGotchaScreen.hpp"
#include "Demos/Graphics3D/BasicRendering/VertexColorCubeScreen.hpp"
#include "Demos/Graphics3D/BasicRendering/TexturedCubeScreen.hpp"
#include "Demos/Graphics3D/BasicRendering/AlphaBlendCubeScreen.hpp"
#include "Demos/Graphics3D/Lighting/DefaultLightingScreen.hpp"
#include "Demos/Graphics3D/Lighting/DirectionalLightsToggleScreen.hpp"
#include "Demos/Graphics3D/Lighting/AmbientAndSpecularScreen.hpp"
#include "Demos/Graphics3D/Fog/DistanceFogScreen.hpp"
#include "Demos/Graphics3D/AlphaTestEffect/AlphaCutoffScreen.hpp"
#include "Demos/Graphics3D/AlphaTestEffect/AlphaFunctionScreen.hpp"
#include "Demos/Graphics3D/DualTextureEffect/DualTextureBlendScreen.hpp"
#include "Demos/Graphics3D/EnvironmentMapEffect/ReflectiveCubeScreen.hpp"
#include "Demos/Graphics3D/EnvironmentMapEffect/EnvironmentMapTuningScreen.hpp"
#include "Demos/Graphics3D/SkinnedEffect/TwoBoneSkinningScreen.hpp"
#include "Demos/Graphics3D/CustomShader/CustomGlslShaderScreen.hpp"
#include "Demos/Graphics3D/CustomShader/InvalidShaderScreen.hpp"
#include "Demos/Graphics3D/PbrEffect/PbrMetallicRoughnessScreen.hpp"
#include "Demos/Graphics3D/DepthAndCulling/DepthStencilComparisonScreen.hpp"
#include "Demos/Graphics3D/DepthAndCulling/CullModeScreen.hpp"
#include "Demos/Graphics3D/DepthAndCulling/FillModeWireframeScreen.hpp"
#include "Demos/Graphics3D/CameraAndProjection/OrbitingCameraScreen.hpp"
#include "Demos/Graphics3D/CameraAndProjection/PerspectiveVsOrthographicScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ProceduralModelScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ModelBoneHierarchyScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ContentModelLoadScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ModelEffectMaterialScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ModelAnimationPlayerScreen.hpp"
#include "Demos/Graphics3D/ModelGroup/ModelMorphTargetScreen.hpp"
#include "Demos/Graphics3D/TexturesAndQueries/Texture3DVolumeScreen.hpp"
#include "Demos/Graphics3D/TexturesAndQueries/TextureCubeFacesScreen.hpp"
#include "Demos/Graphics3D/EffectReflection/ReflectionSurfaceScreen.hpp"
#include "Demos/Graphics3D/EffectReflection/EffectParameterScreen.hpp"
#include "Demos/Graphics3D/EffectReflection/EffectCloneScreen.hpp"
#include "Demos/Graphics3D/TexturesAndQueries/RenderTargetCubeScreen.hpp"
#include "Demos/Graphics3D/TexturesAndQueries/OcclusionQueryScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildVertexTypesDemos() {
    using namespace CnaExamples::Demos::Graphics3D::VertexTypesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PositionColorScreen>(
        "VertexPositionColor", "Position + Color only -- a flat-colored, unlit spinning cube"));
    demos.push_back(MakeDemo<PositionTextureScreen>(
        "VertexPositionTexture", "Position + UV only -- an unlit, procedurally-textured cube"));
    demos.push_back(MakeDemo<PositionNormalTextureScreen>(
        "VertexPositionNormalTexture", "Position + Normal + UV -- the vertex type BasicEffect needs to be lit"));
    return demos;
}

std::vector<DemoEntry> BuildPrimitiveTypesDemos() {
    using namespace CnaExamples::Demos::Graphics3D::PrimitiveTypesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<TriangleListVsStripScreen>(
        "TriangleList vs. TriangleStrip", "The same 4-triangle band, 12 explicit verts vs. 6 shared verts"));
    demos.push_back(MakeDemo<LineAndPointScreen>(
        "LineList / LineStrip / PointListEXT", "The same square's corners drawn 3 different primitive ways"));
    return demos;
}

std::vector<DemoEntry> BuildBuffersDemos() {
    using namespace CnaExamples::Demos::Graphics3D::BuffersDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ImmediateVsBufferedScreen>(
        "Immediate vs. Buffered Draw", "The same cube via DrawUserIndexedPrimitives vs. VertexBuffer+IndexBuffer"));
    demos.push_back(MakeDemo<DynamicVertexBufferScreen>(
        "DynamicVertexBuffer Ripple", "A 12x12 grid re-uploaded via SetData(..., Discard) every frame"));
    demos.push_back(MakeDemo<VertexDeclarationGotchaScreen>(
        "VertexDeclaration Stride Gotcha", "A confirmed real bug: an unrecognized stride silently drops color"));
    return demos;
}

std::vector<DemoEntry> BuildBasicRenderingDemos() {
    using namespace CnaExamples::Demos::Graphics3D::BasicRenderingDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<VertexColorCubeScreen>(
        "VertexColorEnabled", "Live toggle proving VertexColorEnabled gates vertex color vs. DiffuseColor"));
    demos.push_back(MakeDemo<TexturedCubeScreen>(
        "Live Texture Swap", "Cycling BasicEffect.Texture between 3 procedural textures at runtime"));
    demos.push_back(MakeDemo<AlphaBlendCubeScreen>(
        "Alpha Blending", "A translucent cube (adjustable Alpha) over an opaque one, BlendState::AlphaBlend"));
    return demos;
}

std::vector<DemoEntry> BuildLightingDemos() {
    using namespace CnaExamples::Demos::Graphics3D::LightingDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DefaultLightingScreen>(
        "EnableDefaultLighting", "The stock 3-point key/fill/back lighting rig, with a live on/off toggle"));
    demos.push_back(MakeDemo<DirectionalLightsToggleScreen>(
        "3 Directional Lights", "DirectionalLight0/1/2 (red/green/blue), independently toggleable"));
    demos.push_back(MakeDemo<AmbientAndSpecularScreen>(
        "Ambient & Specular", "Live-adjustable AmbientLightColor and SpecularPower on a lit cube"));
    return demos;
}

std::vector<DemoEntry> BuildFogDemos() {
    using namespace CnaExamples::Demos::Graphics3D::FogDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DistanceFogScreen>(
        "Distance Fog", "5 cubes receding into the distance, fading toward FogColor via FogStart/FogEnd"));
    return demos;
}

std::vector<DemoEntry> BuildAlphaTestEffectDemos() {
    using namespace CnaExamples::Demos::Graphics3D::AlphaTestEffectDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<AlphaCutoffScreen>(
        "Reference Alpha Cutoff", "A radial alpha-gradient quad, ReferenceAlpha adjustable live"));
    demos.push_back(MakeDemo<AlphaFunctionScreen>(
        "AlphaFunction", "The same gradient quad, cycling Greater/Less/GreaterEqual/Always"));
    return demos;
}

std::vector<DemoEntry> BuildDualTextureEffectDemos() {
    using namespace CnaExamples::Demos::Graphics3D::DualTextureEffectDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DualTextureBlendScreen>(
        "2-Layer Blend", "A cube with 2 independent textures (Texture + Texture2) blended per pixel"));
    return demos;
}

std::vector<DemoEntry> BuildEnvironmentMapEffectDemos() {
    using namespace CnaExamples::Demos::Graphics3D::EnvironmentMapEffectDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ReflectiveCubeScreen>(
        "Reflective Cube", "A rotating cube fully reflecting a 6-color procedural TextureCube"));
    demos.push_back(MakeDemo<EnvironmentMapTuningScreen>(
        "Amount & Fresnel", "Live-adjustable EnvironmentMapAmount and FresnelFactor on the same cube map"));
    return demos;
}

std::vector<DemoEntry> BuildSkinnedEffectDemos() {
    using namespace CnaExamples::Demos::Graphics3D::SkinnedEffectDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<TwoBoneSkinningScreen>(
        "Two-Bone Bend", "A hinged strip, SetBoneTransforms animating a live bend every frame"));
    return demos;
}

std::vector<DemoEntry> BuildCustomShaderDemos() {
    using namespace CnaExamples::Demos::Graphics3D::CustomShaderDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<CustomGlslShaderScreen>(
        "Custom GLSL Shader", "A hand-authored ShaderEffect (time-pulsed color) on a real cube"));
    demos.push_back(MakeDemo<InvalidShaderScreen>(
        "Invalid Shader Handling", "A deliberately broken shader -- IsEffectValid() surfaces the failure safely"));
    return demos;
}

std::vector<DemoEntry> BuildPbrEffectDemos() {
    using namespace CnaExamples::Demos::Graphics3D::PbrDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PbrMetallicRoughnessScreen>(
        "Metallic & Roughness",
        "The glTF metallic-roughness BRDF on a 5x3 sphere grid, verified live by pixel probe",
        {"PbrEffect"}));
    return demos;
}

std::vector<DemoEntry> BuildDepthAndCullingDemos() {
    using namespace CnaExamples::Demos::Graphics3D::DepthAndCullingDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<DepthStencilComparisonScreen>(
        "DepthStencilState: Default vs. None", "2 overlapping cubes; toggling depth testing changes what occludes what"));
    demos.push_back(MakeDemo<CullModeScreen>(
        "RasterizerState: CullMode", "A spinning quad, cycling CullNone/CullClockwise/CullCounterClockwise"));
    demos.push_back(MakeDemo<FillModeWireframeScreen>(
        "RasterizerState: FillMode", "Solid vs. WireFrame toggle on a lit cube"));
    return demos;
}

std::vector<DemoEntry> BuildCameraAndProjectionDemos() {
    using namespace CnaExamples::Demos::Graphics3D::CameraAndProjectionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<OrbitingCameraScreen>(
        "Orbiting View Matrix", "The camera itself orbits via a live-recomputed CreateLookAt View matrix"));
    demos.push_back(MakeDemo<PerspectiveVsOrthographicScreen>(
        "Perspective vs. Orthographic", "The same depth-staggered scene, side by side under both projections"));
    return demos;
}

std::vector<DemoEntry> BuildModelGroupDemos() {
    using namespace CnaExamples::Demos::Graphics3D::ModelGroupDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ProceduralModelScreen>(
        "Procedural Construction", "A real Model/ModelMesh/ModelMeshPart graph built entirely by hand, no XNB"));
    demos.push_back(MakeDemo<ModelBoneHierarchyScreen>(
        "ModelBone Hierarchy", "A parent/child bone pair -- the child inherits the parent's own transform"));
    demos.push_back(MakeDemo<ContentModelLoadScreen>(
        "Load & Traverse",
        "ContentManager.Load<Model>() from a synthesized .cnj fixture -- and a real Effects-collection gap",
        {"Model", "ModelMesh", "ModelMeshPart", "ContentManager"}));
    demos.push_back(MakeDemo<ModelEffectMaterialScreen>(
        "EffectMaterial & Effect Swapping",
        "EffectMaterial is never built by either content reader; live part.Effect swap done right",
        {"EffectMaterial", "Effect", "ModelMeshPart"}));
    demos.push_back(MakeDemo<ModelAnimationPlayerScreen>(
        "Skeletal Animation (AnimationPlayer)",
        "A real SkinningData Tag, AnimationPlayer and two SkinnedEffects, verified against the clip",
        {"AnimationPlayer", "SkinningData", "SkinnedEffect"}));
    demos.push_back(MakeDemo<ModelMorphTargetScreen>(
        "Morph Targets (MorphTargetEXT)",
        "A CPU-blended morph target driven by a hand-evaluated LINEAR weight track",
        {"MorphTargetEXT"}));
    return demos;
}

std::vector<DemoEntry> BuildTexturesAndQueriesDemos() {
    using namespace CnaExamples::Demos::Graphics3D::TexturesAndQueriesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<Texture3DVolumeScreen>(
        "Texture3D Volume & Sub-Box",
        "A volume texture written whole, then one axis-aligned sub-box, verified by GetData",
        {"Texture3D", "SurfaceFormat"}));
    demos.push_back(MakeDemo<TextureCubeFacesScreen>(
        "TextureCube Six Faces",
        "Six faces written and read back per CubeMapFace, laid out as the unfolded cross",
        {"TextureCube", "CubeMapFace"}));
    demos.push_back(MakeDemo<RenderTargetCubeScreen>(
        "RenderTargetCube Rendered Faces",
        "Each cube face bound and rendered separately, then read back",
        {"RenderTargetCube", "CubeMapFace"}));
    demos.push_back(MakeDemo<OcclusionQueryScreen>(
        "OcclusionQuery Occluded vs Visible",
        "Pixels surviving the depth test, measured behind an occluder and without one",
        {"OcclusionQuery"}));
    return demos;
}

std::vector<DemoEntry> BuildEffectReflectionDemos() {
    using namespace CnaExamples::Demos::Graphics3D::EffectReflectionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ReflectionSurfaceScreen>(
        "What Is Actually Exposed",
        "Parameters/Techniques measured live -- and why Parameters[\"World\"] is null here",
        {"Effect", "EffectParameterCollection", "EffectTechnique"}));
    demos.push_back(MakeDemo<EffectParameterScreen>(
        "Parameter Classes, Types & Values",
        "Class vs Type, and the silent SetValue/SetValueTranspose mismatch",
        {"EffectParameter", "EffectParameterClass", "EffectParameterType"}));
    demos.push_back(MakeDemo<EffectCloneScreen>(
        "Clone: What Is and Is Not Copied",
        "A raw owning pointer, copied state, and why ShaderEffect clones differ",
        {"Effect"}));
    return demos;
}

} // namespace CnaExamples::Navigation
