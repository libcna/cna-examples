// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Storage/Device/StorageDeviceScreen.hpp"
#include "Demos/Storage/Container/SaveGameRoundTripScreen.hpp"
#include "Demos/Storage/Container/DirectoriesAndFilesScreen.hpp"
#include "Demos/Storage/Container/ContainerLifetimeScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildStorageDeviceDemos() {
    using namespace CnaExamples::Demos::Storage::DeviceDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<StorageDeviceScreen>(
        "StorageDevice", "XNA's fake-async selector, per PlayerIndex, and where saves land",
        {"StorageDevice::BeginShowSelector", "StorageDevice::EndShowSelector",
         "StorageDevice::FreeSpace", "StorageDevice::GetStorageRootEXT"}));
    return demos;
}

std::vector<DemoEntry> BuildStorageContainerDemos() {
    using namespace CnaExamples::Demos::Storage::ContainerDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<SaveGameRoundTripScreen>(
        "Save Game Round Trip", "Write, read back and delete a save that survives a restart",
        {"StorageContainer::CreateFile", "StorageContainer::OpenFile",
         "StorageContainer::FileExists", "StorageContainer::DeleteFile"}));
    demos.push_back(MakeDemo<DirectoriesAndFilesScreen>(
        "Directories & Files", "CreateDirectory/GetDirectoryNames/GetFileNames -- and why listing is not recursive",
        {"StorageContainer::CreateDirectory", "StorageContainer::DeleteDirectory",
         "StorageContainer::GetDirectoryNames", "StorageContainer::GetFileNames"}));
    demos.push_back(MakeDemo<ContainerLifetimeScreen>(
        "Container Lifetime", "Dispose, Disposing, reopening a container, and DeleteContainer",
        {"StorageContainer::Dispose", "StorageContainer::Disposing",
         "StorageDevice::DeleteContainer"}));
    return demos;
}

} // namespace CnaExamples::Navigation
