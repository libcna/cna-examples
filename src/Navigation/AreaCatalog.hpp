// SPDX-License-Identifier: MIT
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "GameStateManagement/GameScreen.hpp"

namespace CnaExamples::Navigation {

using CnaExamples::GameStateManagement::GameScreen;

// A single runnable demonstration within a Category. `create` is a factory
// so building the catalog never constructs a screen (and its content) until
// the user actually navigates to it.
struct DemoEntry {
    std::string title;
    std::string description;

    // The CNA/XNA symbols this demo actually exercises, e.g.
    // {"MediaLibrary::Songs", "Song::Album"}. One list drives three things --
    // the footer line on the demo screen, the search index (SearchScreen), and
    // the coverage question "which API does this demo prove works?" -- so it is
    // answered in one place instead of three.
    //
    // Populated per area as areas are built or revisited; older entries that
    // predate the field simply have none yet.
    std::vector<std::string> apis;

    std::function<std::shared_ptr<GameScreen>()> create;
};

// A group of related demos within an Area (e.g. Input's "Keyboard" category).
struct CategoryEntry {
    std::string title;
    std::vector<DemoEntry> demos;
};

// A group of related Categories within an Area that has enough of them to
// warrant an extra navigation level (e.g. 2D Graphics -> "SpriteBatch" group
// -> {Drawing Basics, Sort Modes, ...} categories -> demos). Areas that
// don't need this (Input, Audio, Devices, Net, Media) leave
// AreaEntry::groups empty and populate AreaEntry::categories directly
// instead; AreaScreen picks whichever one is non-empty -- see its own
// comment for the exact rule.
struct GroupEntry {
    std::string title;
    std::vector<CategoryEntry> categories;
};

// A top-level CNA subsystem shown on the Home screen (e.g. "Input").
// Exactly one of `groups`/`categories` should be populated per Area (not
// both) -- see GroupEntry's doc comment.
struct AreaEntry {
    std::string title;
    std::vector<CategoryEntry> categories;
    std::vector<GroupEntry> groups;
};

// Total demos underneath a Category / Group / Area, for the "(N)" counts shown
// on the menus above them.
inline int CountDemos(const CategoryEntry& category) {
    return (int)category.demos.size();
}

inline int CountDemos(const GroupEntry& group) {
    int total = 0;
    for (const auto& category : group.categories) total += CountDemos(category);
    return total;
}

inline int CountDemos(const AreaEntry& area) {
    int total = 0;
    for (const auto& category : area.categories) total += CountDemos(category);
    for (const auto& group : area.groups) total += CountDemos(group);
    return total;
}

// "Title  (12)" -- the label a parent menu shows for a child that contains
// demos. Kept here so Home/Area/Group screens format it identically.
inline std::string WithCount(const std::string& title, int count) {
    return title + "   (" + std::to_string(count) + ")";
}

// Assemble the navigation tree from the area registries.
std::vector<AreaEntry> BuildAreaCatalog();

} // namespace CnaExamples::Navigation
