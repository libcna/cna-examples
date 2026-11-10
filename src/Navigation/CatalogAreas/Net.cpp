// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Net/NetworkSession/LocalSessionLifecycleScreen.hpp"
#include "Demos/Net/NetworkSession/SystemLinkHostScreen.hpp"
#include "Demos/Net/NetworkSession/DiscoverAndJoinScreen.hpp"
#include "Demos/Net/NetworkSession/PacketRoundTripScreen.hpp"
#include "Demos/Net/NetworkSession/SimulatedConditionsScreen.hpp"
#include "Demos/Net/NetworkSession/SessionPropertiesAndEventsScreen.hpp"
#include "Demos/Net/NetworkGamer/GamerRosterScreen.hpp"
#include "Demos/Net/NetworkGamer/NetworkMachineScreen.hpp"
#include "Demos/Net/GamerServices/SignedInGamersScreen.hpp"
#include "Demos/Net/GamerServices/ProfilePresencePrivilegesScreen.hpp"
#include "Demos/Net/GamerServices/AchievementsScreen.hpp"
#include "Demos/Net/GamerServices/FriendsAndGamerCardScreen.hpp"
#include "Demos/Net/GamerServices/GuideOverlayScreen.hpp"
#include "Demos/Net/Leaderboards/LeaderboardReaderScreen.hpp"
#include "Demos/Net/Leaderboards/LeaderboardWriterScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildNetworkSessionDemos() {
    using namespace CnaExamples::Demos::Net::NetworkSessionDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<LocalSessionLifecycleScreen>(
        "Local Lifecycle", "StartGame/EndGame/RemoveGamer -- NetworkSessionState transitions"));
    demos.push_back(MakeDemo<SystemLinkHostScreen>(
        "SystemLink Host", "A real UDP-backed session -- run a 2nd instance to join it"));
    demos.push_back(MakeDemo<DiscoverAndJoinScreen>(
        "Discover & Join", "Find() -- real LAN/loopback UDP discovery, then Join()"));
    demos.push_back(MakeDemo<PacketRoundTripScreen>(
        "Packet Round-Trip", "PacketWriter/PacketReader round trips, Write(Color)/ReadColor() included"));
    demos.push_back(MakeDemo<SessionPropertiesAndEventsScreen>(
        "Properties & Events", "NetworkSessionProperties + GamerJoined/GamerLeft/HostChanged"));
    demos.push_back(MakeDemo<SimulatedConditionsScreen>(
        "Simulated Latency & Packet Loss",
        "Inert auto-properties in FNA; really implemented here, and measured",
        {"NetworkSession"}));
    return demos;
}

std::vector<DemoEntry> BuildNetworkGamerDemos() {
    using namespace CnaExamples::Demos::Net::NetworkGamerDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<GamerRosterScreen>(
        "Roster", "AllGamers/LocalGamers/RemoteGamers/PreviousGamers + per-gamer state"));
    demos.push_back(MakeDemo<NetworkMachineScreen>(
        "NetworkMachine", "Gamer::Machine -- RemoveFromSession() refuses the local machine"));
    return demos;
}

std::vector<DemoEntry> BuildGamerServicesDemos() {
    using namespace CnaExamples::Demos::Net::GamerServicesDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<SignedInGamersScreen>(
        "Signed-In Gamers", "SignedInGamers + the SignedIn/SignedOut events"));
    demos.push_back(MakeDemo<ProfilePresencePrivilegesScreen>(
        "Profile/Presence/Privileges", "GetProfile() + live-adjustable Presence + Privileges"));
    demos.push_back(MakeDemo<AchievementsScreen>(
        "Achievements", "AwardAchievement/GetAchievements in the local profile store + GetPicture()"));
    demos.push_back(MakeDemo<FriendsAndGamerCardScreen>(
        "Friends & GamerCard", "GetFriends() needs an online account + a manually-built demo roster"));
    demos.push_back(MakeDemo<GuideOverlayScreen>(
        "Guide Overlay", "Keyboard input, message box and sign-in pane, completed asynchronously"));
    return demos;
}

std::vector<DemoEntry> BuildLeaderboardsDemos() {
    using namespace CnaExamples::Demos::Net::LeaderboardsDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<LeaderboardReaderScreen>(
        "LeaderboardReader", "LeaderboardIdentity + Read() of a page from the leaderboard store"));
    demos.push_back(MakeDemo<LeaderboardWriterScreen>(
        "LeaderboardWriter", "LeaderboardEntry (real value type) + the writer's own GetLeaderboard() entry"));
    return demos;
}

} // namespace CnaExamples::Navigation
