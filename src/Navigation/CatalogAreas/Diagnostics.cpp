// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Diagnostics/Logging/LoggerScreen.hpp"
#include "Demos/Diagnostics/Platform/PlatformInfoScreen.hpp"
#include "Demos/Diagnostics/Capabilities/GraphicsCapabilityScreen.hpp"
#include "Demos/Diagnostics/Adapter/GraphicsAdapterScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildDiagnosticsLoggingDemos() {
    using namespace CnaExamples::Demos::Diagnostics::LoggingDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<LoggerScreen>(
        "Logger", "All seven levels emitted live, and what the filter lets through",
        {"CNA::Logger::Log", "CNA::Logger::SetMinimumLevel", "CNA::LogLevel", "CNA::LogCategory"}));
    return demos;
}

std::vector<DemoEntry> BuildDiagnosticsPlatformDemos() {
    using namespace CnaExamples::Demos::Diagnostics::PlatformDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PlatformInfoScreen>(
        "Platform & Build", "Which platform, backend and optional subsystems this binary has",
        {"CNA::getCurrentPlatform", "CNA::getCurrentGraphicsRendererName", "CNA_DEVICES"}));
    return demos;
}

std::vector<DemoEntry> BuildDiagnosticsCapabilitiesDemos() {
    using namespace CnaExamples::Demos::Diagnostics::CapabilitiesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<GraphicsCapabilityScreen>(
        "Graphics Capabilities", "All eight GraphicsCapability values, queried live",
        {"GraphicsDevice::SupportsCapability", "CNA::GraphicsCapability",
         "CNA::GraphicsRendererType"}));
    return demos;
}

std::vector<DemoEntry> BuildDiagnosticsAdapterDemos() {
    using namespace CnaExamples::Demos::Diagnostics::AdapterDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<GraphicsAdapterScreen>(
        "Adapter & Display Modes", "What could be asked for, next to what the device got",
        {"GraphicsAdapter::DefaultAdapter", "GraphicsAdapter::SupportedDisplayModes",
         "DisplayMode", "PresentationParameters"}));
    return demos;
}

} // namespace CnaExamples::Navigation
