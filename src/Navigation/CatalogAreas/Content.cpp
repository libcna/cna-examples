// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Content/Basics/LoadAndCacheScreen.hpp"
#include "Demos/Content/Basics/ResolutionOrderScreen.hpp"
#include "Demos/Content/Manifest/ContentManifestScreen.hpp"
#include "Demos/Content/Xnb/XnbFixturesScreen.hpp"
#include "Demos/Content/Errors/ContentLoadExceptionScreen.hpp"
#include "Demos/Content/Cnj/CnjEnvelopeScreen.hpp"
#include "Demos/Content/Cnj/CustomCnjLoaderScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildContentBasicsDemos() {
    using namespace CnaExamples::Demos::Content::BasicsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<LoadAndCacheScreen>(
        "Load & Cache", "Timed loads showing the cache, and why identity cannot be compared",
        {"ContentManager::Load", "ContentManager::Unload", "ContentManager::RootDirectory"}));
    demos.push_back(MakeDemo<ResolutionOrderScreen>(
        "Asset Name Resolution", "An asset name is not a filename: .xnb, literal, then .cnj",
        {"ContentManager::Load", "ContentManager::RootDirectory"}));
    return demos;
}

std::vector<DemoEntry> BuildContentManifestDemos() {
    using namespace CnaExamples::Demos::Content::ManifestDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ContentManifestScreen>(
        "Manifest", "Every discoverable asset, plus which .xnb readers this build has",
        {"ContentManager::GetContentManifest", "ContentManager::RefreshContentManifest",
         "ContentManifestEntry", "GetXnbReaderUsageSummary"}));
    return demos;
}

std::vector<DemoEntry> BuildContentXnbDemos() {
    using namespace CnaExamples::Demos::Content::XnbDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<XnbFixturesScreen>(
        "XNB Fixtures", "Real MonoGame-produced .xnb, including an LZX-compressed one",
        {"ContentManager::Load", "Texture2DReader", "LzxDecoder"}));
    return demos;
}

std::vector<DemoEntry> BuildContentErrorsDemos() {
    using namespace CnaExamples::Demos::Content::ErrorsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ContentLoadExceptionScreen>(
        "Load Failures", "Missing, wrong-type and unsupported content, with real messages",
        {"ContentLoadException", "ContentManager::Load"}));
    return demos;
}

std::vector<DemoEntry> BuildContentCnjDemos() {
    using namespace CnaExamples::Demos::Content::CnjDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<CnjEnvelopeScreen>(
        "Envelope", "cnjVersion/type/sourceFile, parsed live -- including an invalid one",
        {"CnjEnvelope", "ParseCnjEnvelope", "cnjVersion"}));
    demos.push_back(MakeDemo<CustomCnjLoaderScreen>(
        "Custom Loaders", "Two .cnj \"type\" names, one C++ struct, no reader class to write",
        {"ContentManager::RegisterCnjLoader", "CnjLoaderFn", "ContentManager::Load"}));
    return demos;
}

} // namespace CnaExamples::Navigation
