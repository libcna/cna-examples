// SPDX-License-Identifier: MIT
#pragma once

#include <exception>
#include <string>
#include <vector>

#include "SharpRuntime/SharpRuntimeHelper.hpp"
#include "System/ObjectDisposedException.hpp"

#include "Demos/DemoScreen.hpp"
#include "Demos/Storage/StorageDemoHelpers.hpp"

namespace CnaExamples::Demos::Storage::ContainerDemos {

using namespace CnaExamples::GameStateManagement;
using CnaExamples::Demos::DemoScreen;

// Dispose(), Disposing, reopening a container, and StorageDevice::DeleteContainer
// -- four things whose exact effect is easy to guess wrong.
//
// What is real, verified live below:
//   * Disposing fires exactly once no matter how many times Dispose() is
//     called -- CNA guards it with an isDisposed_ check, so it is genuinely
//     idempotent, not merely documented as such.
//   * Dispose() DOES gate the handle, as in XNA: file and directory
//     operations on a disposed container throw ObjectDisposedException.
//     IsDisposed and a repeated Dispose() stay available, and the files it
//     already wrote stay on disk.
//   * "Reopening" a container is not tracked at all: StorageDevice hands out
//     a brand new StorageContainer instance every time, with no notion of
//     "already open". Two independent handles over the same directory are
//     both perfectly valid at once; what one writes, the other sees, because
//     both simply resolve paths under the same directory on disk.
//   * StorageDevice::DeleteContainer removes the WHOLE container tree in one
//     call (fs::remove_all under the hood) -- unlike StorageContainer::
//     DeleteDirectory (see the Directories & Files screen), which refuses a
//     non-empty directory. A stale, still-open handle survives the call as a
//     C++ object; what disappears is the directory underneath it.
class ContainerLifetimeScreen : public DemoScreen {
public:
    ContainerLifetimeScreen() : DemoScreen("Storage: Container Lifetime") {}

    void OnDemoLoad() override { RunChecks(); }

    void OnDemoUnload() override { reopened_.reset(); device_.reset(); }

protected:
    void OnDemoDraw(const GameTime&, SpriteBatch& sb, SpriteFont& font) override {
        const Color tint = mul(Color::White, TransitionAlpha());
        std::vector<std::string> lines;

        if (!opened_) {
            lines.push_back("No StorageContainer could be opened.");
            for (const auto& line : log_) lines.push_back("  " + line);
            DrawLines(sb, font, Vector2(40.0f, 82.0f), lines, tint);
            return;
        }

        lines.push_back("IsDisposed before/after Dispose(): " + YesNo(disposedBefore_) + " / " +
                        YesNo(disposedAfter_) + "  (2 calls, Disposing fired " +
                        std::to_string(disposeCount_) + "x -- idempotent)");
        lines.emplace_back();
        lines.push_back("FileExists() on that SAME disposed handle: " +
                        std::string(disposedUseRefused_ ? "throws ObjectDisposedException."
                                                        : "did NOT throw."));
        lines.emplace_back();
        lines.push_back("Reopen (2nd handle, old one still alive) sees the marker it wrote: " +
                        YesNo(reopenedSeesMarker_));
        lines.push_back("DeleteContainer() removes the whole tree; a 3rd fresh handle finds it gone: " +
                        YesNo(!marketSeenAfterDelete_));
        for (const auto& line : log_)
            if (line != "Checks complete.") lines.push_back("  " + line);
        const Vector2 end = DrawLines(sb, font, Vector2(40.0f, 82.0f), lines, tint);

        DrawVerdict(sb, font, end.Y + 6.0f,
                    mul(allHold_ ? Color(40, 200, 90, 255) : Color(220, 60, 60, 255),
                        TransitionAlpha()),
                    tint,
                    allHold_ ? std::to_string(checks_) + " claims verified live."
                             : "A claim did NOT hold: " + failure_);
    }

private:
    static constexpr const char* kMarker = "marker.txt";
    static std::string YesNo(bool v) { return v ? "yes" : "no"; }

    void Note(bool ok, const std::string& what) {
        ++checks_;
        if (!ok && allHold_) { allHold_ = false; failure_ = what; }
    }

    void RunChecks() {
        log_.clear();
        device_ = OpenDevice(PlayerIndex::One);
        if (!device_) { log_.emplace_back("EndShowSelector returned nullptr"); return; }

        auto first = OpenContainer(*device_, kLifetimeContainerName);
        if (!first) { log_.emplace_back("EndOpenContainer returned nullptr"); return; }
        opened_ = true;

        try {
            first->Disposing.Add(
                [this](System::Object*, const System::EventArgs&) { ++disposeCount_; });

            disposedBefore_ = first->getIsDisposedProperty();
            Note(!disposedBefore_, "a freshly-opened container should not start disposed");

            {
                auto stream = first->CreateFile(kMarker);
                const std::string payload = "still here";
                stream->Write(reinterpret_cast<const SharpRuntime::bytecs*>(payload.data()), 0,
                             (SharpRuntime::intcs)payload.size());
                stream->Flush();
            }

            first->Dispose();
            Note(disposeCount_ == 1, "Disposing should have fired exactly once");
            disposedAfter_ = first->getIsDisposedProperty();
            Note(disposedAfter_, "IsDisposed should be true after Dispose()");

            first->Dispose();   // idempotent: must NOT raise Disposing a second time
            Note(disposeCount_ == 1, "a second Dispose() call must not re-raise Disposing");

            // Deliberately still using `first` after Dispose() -- the point of
            // this check is that it is refused.
            try {
                (void)first->FileExists(kMarker);
            } catch (const System::ObjectDisposedException&) {
                disposedUseRefused_ = true;
            }
            Note(disposedUseRefused_,
                "FileExists on a disposed handle should throw ObjectDisposedException");

            // "Reopen" while `first` is still alive: a second, independent handle.
            reopened_ = OpenContainer(*device_, kLifetimeContainerName);
            if (reopened_) {
                reopenedSeesMarker_ = reopened_->FileExists(kMarker);
                Note(reopenedSeesMarker_,
                    "a freshly-opened second handle should see the file the first one wrote");
            } else {
                Note(false, "reopening the same container returned nullptr");
            }

            device_->DeleteContainer(kLifetimeContainerName);
            auto third = OpenContainer(*device_, kLifetimeContainerName);
            if (third) {
                marketSeenAfterDelete_ = third->FileExists(kMarker);
                Note(!marketSeenAfterDelete_,
                    "DeleteContainer should have removed the marker file entirely");
                third->Dispose();
            } else {
                Note(false, "opening a fresh container after DeleteContainer returned nullptr");
            }

            reopened_.reset();
            log_.emplace_back("Checks complete.");
        } catch (const std::exception& ex) {
            log_.emplace_back(std::string("threw: ") + ex.what());
            Note(false, ex.what());
        }
    }

    std::unique_ptr<Microsoft::Xna::Framework::Storage::StorageDevice> device_;
    std::unique_ptr<Microsoft::Xna::Framework::Storage::StorageContainer> reopened_;
    std::vector<std::string> log_;
    std::string failure_;
    int checks_ = 0;
    int disposeCount_ = 0;
    bool allHold_ = true;
    bool opened_ = false;
    bool disposedBefore_ = false;
    bool disposedAfter_ = false;
    bool disposedUseRefused_ = false;
    bool reopenedSeesMarker_ = false;
    bool marketSeenAfterDelete_ = false;
};

} // namespace CnaExamples::Demos::Storage::ContainerDemos
