// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Audio/SoundEffect/PlayToneScreen.hpp"
#include "Demos/Audio/SoundEffect/MasterVolumeAndSettingsScreen.hpp"
#include "Demos/Audio/SoundEffectInstance/PlaybackControlScreen.hpp"
#include "Demos/Audio/SoundEffectInstance/VolumePitchPanScreen.hpp"
#include "Demos/Audio/SoundEffectInstance/LoopingScreen.hpp"
#include "Demos/Audio/Audio3D/Apply3DScreen.hpp"
#include "Demos/Audio/Audio3D/DopplerDistanceScreen.hpp"
#include "Demos/Audio/DynamicSoundEffectInstance/StreamingSineWaveScreen.hpp"
#include "Demos/Audio/Microphone/MicrophoneEnumerationScreen.hpp"
#include "Demos/Audio/Microphone/MicrophoneCaptureScreen.hpp"
#include "Demos/Audio/Xact/XactEngineScreen.hpp"
#include "Demos/Audio/Xact/XactCueScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildSoundEffectDemos() {
    using namespace CnaExamples::Demos::Audio::SoundEffectDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PlayToneScreen>(
        "Play a Tone", "SoundEffect::Play() -- a generated sine wave, no WAV asset needed"));
    demos.push_back(MakeDemo<MasterVolumeAndSettingsScreen>(
        "Static Settings", "MasterVolume/DistanceScale/DopplerScale/SpeedOfSound + byte<->duration math"));
    return demos;
}

std::vector<DemoEntry> BuildSoundEffectInstanceDemos() {
    using namespace CnaExamples::Demos::Audio::SoundEffectInstanceDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PlaybackControlScreen>(
        "Playback Control", "Play/Pause/Resume/Stop + the State property"));
    demos.push_back(MakeDemo<VolumePitchPanScreen>(
        "Volume/Pitch/Pan", "Live-adjustable while a looping tone plays"));
    demos.push_back(MakeDemo<LoopingScreen>(
        "Looping", "IsLooped -- play-once vs. repeat-until-stopped"));
    return demos;
}

std::vector<DemoEntry> BuildAudio3DDemos() {
    using namespace CnaExamples::Demos::Audio::Audio3DDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<Apply3DScreen>(
        "Apply3D", "AudioListener + a movable AudioEmitter recompute Volume/Pan live"));
    demos.push_back(MakeDemo<DopplerDistanceScreen>(
        "Doppler & Distance Scale", "Emitter velocity -> a real closed-form Doppler pitch shift"));
    return demos;
}

std::vector<DemoEntry> BuildDynamicSoundEffectInstanceDemos() {
    using namespace CnaExamples::Demos::Audio::DynamicSoundEffectInstanceDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<StreamingSineWaveScreen>(
        "Streaming Sine Wave", "Real-time synthesis via BufferNeeded + SubmitBuffer()"));
    return demos;
}

std::vector<DemoEntry> BuildMicrophoneDemos() {
    using namespace CnaExamples::Demos::Audio::MicrophoneDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<MicrophoneEnumerationScreen>(
        "Enumeration", "Microphone::All / Default + per-device properties"));
    demos.push_back(MakeDemo<MicrophoneCaptureScreen>(
        "Capture", "Start/Stop + GetData() + the BufferReady event"));
    return demos;
}

std::vector<DemoEntry> BuildXactDemos() {
    using namespace CnaExamples::Demos::Audio::XactDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<XactEngineScreen>(
        "Engine & Banks", "AudioEngine + WaveBank + SoundBank, and the order they must open in",
        {"AudioEngine", "WaveBank", "SoundBank", "AudioEngine::Update"}));
    demos.push_back(MakeDemo<XactCueScreen>(
        "Cues & Categories", "A Cue's full seven-flag state machine, and category-wide volume",
        {"SoundBank::GetCue", "Cue::Play", "AudioCategory::SetVolume",
         "AudioEngine::GetGlobalVariable"}));
    return demos;
}

} // namespace CnaExamples::Navigation
