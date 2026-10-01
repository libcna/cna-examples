// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Math/Vectors/Vector2OperationsScreen.hpp"
#include "Demos/Math/Vectors/Vector3OperationsScreen.hpp"
#include "Demos/Math/Vectors/InterpolationScreen.hpp"
#include "Demos/Math/Vectors/TransformAndDistanceScreen.hpp"
#include "Demos/Math/Vectors/MathHelperScreen.hpp"
#include "Demos/Math/MatrixQuaternion/MatrixCompositionScreen.hpp"
#include "Demos/Math/MatrixQuaternion/DecomposeAndInvertScreen.hpp"
#include "Demos/Math/MatrixQuaternion/QuaternionRotationScreen.hpp"
#include "Demos/Math/MatrixQuaternion/SlerpVsLerpScreen.hpp"
#include "Demos/Math/Geometry/BoundingVolumesScreen.hpp"
#include "Demos/Math/Geometry/RayIntersectionScreen.hpp"
#include "Demos/Math/Geometry/FrustumCullingScreen.hpp"
#include "Demos/Math/Curves/CurveTangentsScreen.hpp"
#include "Demos/Math/Curves/CurveLoopTypeScreen.hpp"
#include "Demos/Math/Color/ColorConversionsScreen.hpp"
#include "Demos/Math/Color/PackedVectorGalleryScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildMathVectorsDemos() {
    using namespace CnaExamples::Demos::Math::VectorsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<Vector2OperationsScreen>(
        "Vector2 Operations", "Add/Dot/Normalize/Reflect, drawn as well as printed",
        {"Vector2::Dot", "Vector2::Normalize", "Vector2::Reflect", "Vector2::Length"}));
    demos.push_back(MakeDemo<Vector3OperationsScreen>(
        "Vector3 Cross, Dot & Reflect", "Cross's handedness, computed live rather than claimed",
        {"Vector3::Cross", "Vector3::Dot", "Vector3::Reflect"}));
    demos.push_back(MakeDemo<InterpolationScreen>(
        "Interpolation", "Lerp/SmoothStep/CatmullRom/Hermite plotted against t",
        {"MathHelper::Lerp", "MathHelper::SmoothStep", "MathHelper::CatmullRom",
         "MathHelper::Hermite", "MathHelper::Barycentric"}));
    demos.push_back(MakeDemo<TransformAndDistanceScreen>(
        "Transform & Distance", "Transform vs TransformNormal -- the bug that hides at the origin",
        {"Vector3::Transform", "Vector3::TransformNormal", "Vector2::DistanceSquared"}));
    demos.push_back(MakeDemo<MathHelperScreen>(
        "MathHelper", "WrapAngle, Clamp, and why 0.1f + 0.2f != 0.3f",
        {"MathHelper::WrapAngle", "MathHelper::Clamp", "MathHelper::WithinEpsilon"}));
    return demos;
}

std::vector<DemoEntry> BuildMatrixQuaternionDemos() {
    using namespace CnaExamples::Demos::Math::MatrixQuaternionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<MatrixCompositionScreen>(
        "Composition Order", "R*T and T*R from the same operands, drawn side by side",
        {"Matrix::CreateRotationZ", "Matrix::CreateTranslation", "Vector3::Transform"}));
    demos.push_back(MakeDemo<DecomposeAndInvertScreen>(
        "Decompose, Invert & Determinant", "Including a shear that cannot be decomposed at all",
        {"Matrix::Decompose", "Matrix::Invert", "Matrix::Determinant", "Matrix::Transpose"}));
    demos.push_back(MakeDemo<QuaternionRotationScreen>(
        "Rotation Basics", "Unit length, matrix round trip, Conjugate == Inverse",
        {"Quaternion::CreateFromAxisAngle", "Quaternion::Conjugate", "Quaternion::Inverse"}));
    demos.push_back(MakeDemo<SlerpVsLerpScreen>(
        "Slerp vs Lerp", "Measured angular velocity, constant for one and not the other",
        {"Quaternion::Slerp", "Quaternion::Lerp"}));
    return demos;
}

std::vector<DemoEntry> BuildGeometryDemos() {
    using namespace CnaExamples::Demos::Math::GeometryDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<BoundingVolumesScreen>(
        "Bounding Volumes", "Intersects (bool) vs Contains (ContainmentType), all three states",
        {"BoundingBox::Contains", "BoundingSphere::CreateFromPoints", "ContainmentType"}));
    demos.push_back(MakeDemo<RayIntersectionScreen>(
        "Ray Intersection", "Distance along the ray, and why the direction must be unit length",
        {"Ray::Intersects", "BoundingSphere", "BoundingBox", "Plane"}));
    demos.push_back(MakeDemo<FrustumCullingScreen>(
        "Frustum Culling", "An 11x11 grid culled against a rotating camera, live counts",
        {"BoundingFrustum", "BoundingFrustum::Contains", "BoundingFrustum::GetCorners"}));
    return demos;
}

std::vector<DemoEntry> BuildCurvesDemos() {
    using namespace CnaExamples::Demos::Math::CurvesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<CurveTangentsScreen>(
        "Tangents", "Identical keys, three tangent rules, three very different shapes",
        {"Curve::ComputeTangents", "CurveTangent", "Curve::Evaluate", "CurveKey"}));
    demos.push_back(MakeDemo<CurveLoopTypeScreen>(
        "Pre/Post Loop", "What a Curve returns outside the range its keys cover",
        {"Curve::PreLoop", "Curve::PostLoop", "CurveLoopType"}));
    return demos;
}

std::vector<DemoEntry> BuildColorDemos() {
    using namespace CnaExamples::Demos::Math::ColorDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ColorConversionsScreen>(
        "Conversions", "Premultiplied vs straight alpha, and why the bug hides at alpha 1",
        {"Color::FromNonPremultiplied", "Color::ToVector4", "Color::PackedValue"}));
    demos.push_back(MakeDemo<PackedVectorGalleryScreen>(
        "PackedVector Gallery", "17 formats round-tripped; the error column is the whole story",
        {"IPackedVector", "Bgr565", "NormalizedByte4", "HalfVector4", "Rgba1010102"}));
    return demos;
}

} // namespace CnaExamples::Navigation
