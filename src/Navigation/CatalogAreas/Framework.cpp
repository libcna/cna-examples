// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Framework/GameLoop/FixedVsVariableTimeStepScreen.hpp"
#include "Demos/Framework/GameLoop/TargetElapsedTimeScreen.hpp"
#include "Demos/Framework/GameLoop/IsRunningSlowlyScreen.hpp"
#include "Demos/Framework/GameLoop/SuppressDrawScreen.hpp"
#include "Demos/Framework/GameComponents/ComponentLifecycleScreen.hpp"
#include "Demos/Framework/GameComponents/UpdateDrawOrderScreen.hpp"
#include "Demos/Framework/GameComponents/CollectionEventsScreen.hpp"
#include "Demos/Framework/GameComponents/EnabledVisibleScreen.hpp"
#include "Demos/Framework/Services/GameServiceContainerScreen.hpp"
#include "Demos/Framework/Services/FrameworkDispatcherScreen.hpp"
#include "Demos/Framework/Services/LaunchParametersScreen.hpp"
#include "Demos/Framework/Window/TitleAndClientBoundsScreen.hpp"
#include "Demos/Framework/Window/ClientSizeChangedScreen.hpp"
#include "Demos/Framework/Window/OrientationScreen.hpp"
#include "Demos/Framework/DeviceManager/ResolutionAndFullScreenScreen.hpp"
#include "Demos/Framework/DeviceManager/VSyncAndMultiSamplingScreen.hpp"
#include "Demos/Framework/DeviceManager/DeviceEventsScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildGameLoopDemos() {
    using namespace CnaExamples::Demos::Framework::GameLoopDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<FixedVsVariableTimeStepScreen>(
        "Fixed vs Variable Time Step", "Toggle IsFixedTimeStep and watch the frame-delta spread",
        {"Game::IsFixedTimeStep", "GameTime::ElapsedGameTime"}));
    demos.push_back(MakeDemo<TargetElapsedTimeScreen>(
        "TargetElapsedTime", "60/30/10 Hz ticks; TotalGameTime keeps real-world speed",
        {"Game::TargetElapsedTime", "GameTime::TotalGameTime"}));
    demos.push_back(MakeDemo<IsRunningSlowlyScreen>(
        "IsRunningSlowly", "Burn real CPU in Update until the loop falls behind",
        {"GameTime::IsRunningSlowly"}));
    demos.push_back(MakeDemo<SuppressDrawScreen>(
        "SuppressDraw & ResetElapsedTime", "Update keeps counting while Draw is skipped",
        {"Game::SuppressDraw", "Game::ResetElapsedTime"}));
    return demos;
}

std::vector<DemoEntry> BuildGameComponentsDemos() {
    using namespace CnaExamples::Demos::Framework::GameComponentsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ComponentLifecycleScreen>(
        "Lifecycle", "Add/remove a live GameComponent; when Initialize actually runs",
        {"GameComponent", "Game::Components", "GameComponentCollection::Contains"}));
    demos.push_back(MakeDemo<UpdateDrawOrderScreen>(
        "Update/Draw Order", "Swap DrawOrder and read the real draw sequence back",
        {"DrawableGameComponent::DrawOrder", "GameComponent::UpdateOrder"}));
    demos.push_back(MakeDemo<CollectionEventsScreen>(
        "Collection Events", "ComponentAdded/ComponentRemoved, logged by the handlers",
        {"GameComponentCollection::ComponentAdded", "GameComponentCollection::ComponentRemoved"}));
    demos.push_back(MakeDemo<EnabledVisibleScreen>(
        "Enabled & Visible", "Enabled gates Update, Visible gates Draw -- counters diverge",
        {"GameComponent::Enabled", "DrawableGameComponent::Visible", "EnabledChanged"}));
    return demos;
}

std::vector<DemoEntry> BuildServicesDemos() {
    using namespace CnaExamples::Demos::Framework::ServicesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<GameServiceContainerScreen>(
        "GameServiceContainer", "Register/resolve your own service; the built-in device service",
        {"Game::Services", "GameServiceContainer::AddService", "IGraphicsDeviceService"}));
    demos.push_back(MakeDemo<FrameworkDispatcherScreen>(
        "FrameworkDispatcher", "The pump Game::Update calls for you, and why it matters",
        {"FrameworkDispatcher::Update"}));
    demos.push_back(MakeDemo<LaunchParametersScreen>(
        "LaunchParameters", "This process's own command line, parsed into key/value pairs",
        {"Game::LaunchParameters", "LaunchParameters::ContainsKey"}));
    return demos;
}

std::vector<DemoEntry> BuildWindowDemos() {
    using namespace CnaExamples::Demos::Framework::WindowDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<TitleAndClientBoundsScreen>(
        "Title & ClientBounds", "Change the real OS window title; ClientBounds vs viewport",
        {"GameWindow::Title", "GameWindow::ClientBounds", "GameWindow::ScreenDeviceName"}));
    demos.push_back(MakeDemo<ClientSizeChangedScreen>(
        "ClientSizeChanged", "Resize the window and watch the event log fill in",
        {"GameWindow::ClientSizeChanged", "GameWindow::AllowUserResizing"}));
    demos.push_back(MakeDemo<OrientationScreen>(
        "Display Orientation", "CurrentOrientation, derived from the window's aspect on desktop",
        {"GameWindow::CurrentOrientation", "DisplayOrientation", "SupportedOrientations"}));
    return demos;
}

std::vector<DemoEntry> BuildDeviceManagerDemos() {
    using namespace CnaExamples::Demos::Framework::DeviceManagerDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ResolutionAndFullScreenScreen>(
        "Resolution & FullScreen", "Preferred* is a request; nothing happens until ApplyChanges()",
        {"GraphicsDeviceManager::PreferredBackBufferWidth", "ApplyChanges", "IsFullScreen"}));
    demos.push_back(MakeDemo<VSyncAndMultiSamplingScreen>(
        "VSync & MultiSampling", "Measure the frame rate with v-sync on and off",
        {"SynchronizeWithVerticalRetrace", "PreferMultiSampling", "GraphicsCapability"}));
    demos.push_back(MakeDemo<DeviceEventsScreen>(
        "Device Events", "Force a real device reset and log all five manager events",
        {"PreparingDeviceSettings", "DeviceReset", "DeviceResetting", "DeviceCreated"}));
    return demos;
}

} // namespace CnaExamples::Navigation
