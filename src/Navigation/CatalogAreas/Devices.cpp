// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Devices/Sensors/AccelerometerScreen.hpp"
#include "Demos/Devices/Sensors/GyroscopeScreen.hpp"
#include "Demos/Devices/Sensors/CompassScreen.hpp"
#include "Demos/Devices/Sensors/MotionScreen.hpp"
#include "Demos/Devices/Vibration/VibrateScreen.hpp"
#include "Demos/Devices/Camera/CameraCaptureScreen.hpp"
#include "Demos/Devices/SystemAndDisplay/SystemInfoScreen.hpp"
#include "Demos/Devices/SystemAndDisplay/DisplayInfoScreen.hpp"
#include "Demos/Devices/SystemAndDisplay/LocaleScreen.hpp"
#include "Demos/Devices/Power/PowerInfoScreen.hpp"
#include "Demos/Devices/DesktopIntegration/ClipboardScreen.hpp"
#include "Demos/Devices/DesktopIntegration/MessageBoxScreen.hpp"
#include "Demos/Devices/DesktopIntegration/FileDialogScreen.hpp"
#include "Demos/Devices/DesktopIntegration/UrlLauncherScreen.hpp"
#include "Demos/Devices/DesktopIntegration/SystemTrayScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildSensorsDemos() {
    using namespace CnaExamples::Demos::Devices::SensorsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<AccelerometerScreen>(
        "Accelerometer", "Real XNA 4.0/WP7 API -- try this on Android"));
    demos.push_back(MakeDemo<GyroscopeScreen>(
        "Gyroscope", "Real XNA 4.0/WP7 API -- try this on Android"));
    demos.push_back(MakeDemo<CompassScreen>(
        "Compass", "Real XNA 4.0/WP7 API + the Calibrate event -- try this on Android"));
    demos.push_back(MakeDemo<MotionScreen>(
        "Motion (fused)", "Attitude + DeviceAcceleration + DeviceRotationRate + Gravity"));
    return demos;
}

std::vector<DemoEntry> BuildVibrationDemos() {
    using namespace CnaExamples::Demos::Devices::VibrationDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<VibrateScreen>(
        "VibrateController", "The device's vibration motor -- try this on Android"));
    return demos;
}

std::vector<DemoEntry> BuildCameraDemos() {
    using namespace CnaExamples::Demos::Devices::CameraDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<CameraCaptureScreen>(
        "Live Capture", "CNA::Devices::Camera -- works with a desktop webcam too"));
    return demos;
}

std::vector<DemoEntry> BuildSystemAndDisplayDemos() {
    using namespace CnaExamples::Demos::Devices::SystemAndDisplayDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<SystemInfoScreen>(
        "System Info", "CPU core count + system RAM"));
    demos.push_back(MakeDemo<DisplayInfoScreen>(
        "Display Info", "Content scale + safe area, beyond what GameWindow covers"));
    demos.push_back(MakeDemo<LocaleScreen>(
        "Locale", "The user's OS-level preferred language(s)/region(s)"));
    return demos;
}

std::vector<DemoEntry> BuildPowerDemos() {
    using namespace CnaExamples::Demos::Devices::PowerDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PowerInfoScreen>(
        "PowerInfo", "Device battery/power status (CNA::Devices)"));
    return demos;
}

std::vector<DemoEntry> BuildDesktopIntegrationDemos() {
    using namespace CnaExamples::Demos::Devices::DesktopIntegrationDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<ClipboardScreen>(
        "Clipboard", "Read/write the system clipboard's text"));
    demos.push_back(MakeDemo<MessageBoxScreen>(
        "MessageBox", "Native modal OS dialogs (blocks while open)"));
    demos.push_back(MakeDemo<FileDialogScreen>(
        "FileDialog", "Native async open-file/open-folder dialogs"));
    demos.push_back(MakeDemo<UrlLauncherScreen>(
        "UrlLauncher", "Opens a URL in the system's default handler"));
    demos.push_back(MakeDemo<SystemTrayScreen>(
        "SystemTray", "A real notification-area icon with a menu (desktop only)"));
    return demos;
}

} // namespace CnaExamples::Navigation
