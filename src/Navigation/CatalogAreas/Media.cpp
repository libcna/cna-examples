// SPDX-License-Identifier: MIT
#include "Navigation/AreaCatalogInternal.hpp"

#include "Demos/Media/Song/LoadAndPlayScreen.hpp"
#include "Demos/Media/Song/VolumeMuteRepeatShuffleScreen.hpp"
#include "Demos/Media/Song/QueueNavigationScreen.hpp"
#include "Demos/Media/Song/EventsScreen.hpp"
#include "Demos/Media/Song/UnsupportedFormatScreen.hpp"
#include "Demos/Media/Song/VisualizationScreen.hpp"
#if !defined(__EMSCRIPTEN__)
#include "Demos/Media/Video/LoadAndPlayScreen.hpp"
#include "Demos/Media/Video/PlaybackControlScreen.hpp"
#include "Demos/Media/Video/MultiTrackEXTScreen.hpp"
#endif
#include "Demos/Media/MediaLibrary/CatalogAccessScreen.hpp"
#include "Demos/Media/MediaLibrary/SongMetadataScreen.hpp"
#include "Demos/Media/MediaLibrary/AlbumArtistGenreScreen.hpp"
#include "Demos/Media/MediaLibrary/PlaylistScreen.hpp"
#include "Demos/Media/Pictures/PictureBrowserScreen.hpp"
#include "Demos/Media/Pictures/PictureAlbumTreeScreen.hpp"
#include "Demos/Media/Pictures/SavePictureScreen.hpp"
#include "Demos/Media/Pictures/PictureTokenScreen.hpp"

namespace CnaExamples::Navigation {

std::vector<DemoEntry> BuildSongDemos() {
    using namespace CnaExamples::Demos::Media::SongDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<LoadAndPlayScreen>(
        "Load & Play", "Song's direct-from-file NOXNA constructor + transport controls",
        {"Song", "MediaPlayer::Play", "MediaPlayer::PlayPosition"}));
    demos.push_back(MakeDemo<VolumeMuteRepeatShuffleScreen>(
        "Volume/Mute/Repeat/Shuffle", "MediaPlayer's live-adjustable playback settings",
        {"MediaPlayer::Volume", "MediaPlayer::IsMuted", "MediaPlayer::IsRepeating",
         "MediaPlayer::IsShuffled"}));
    demos.push_back(MakeDemo<QueueNavigationScreen>(
        "Queue Navigation", "Play(SongCollection, index) + MoveNext()/MovePrevious()",
        {"SongCollection", "MediaQueue", "MediaPlayer::MoveNext"}));
    demos.push_back(MakeDemo<EventsScreen>(
        "MediaPlayer Events", "ActiveSongChanged/MediaStateChanged, pumped via FrameworkDispatcher",
        {"MediaPlayer::ActiveSongChanged", "MediaPlayer::MediaStateChanged",
         "FrameworkDispatcher"}));
    demos.push_back(MakeDemo<UnsupportedFormatScreen>(
        "Unsupported Format", "A real .opus file -- constructs fine, Play() silently no-ops",
        {"Song", "MediaPlayer::State"}));
    demos.push_back(MakeDemo<VisualizationScreen>(
        "Visualization", "Live FFT spectrum + waveform from the real post-mix tap",
        {"MediaPlayer::IsVisualizationEnabled", "MediaPlayer::GetVisualizationData",
         "VisualizationData"}));
    return demos;
}

std::vector<DemoEntry> BuildVideoDemos() {
    std::vector<DemoEntry> demos;
#if defined(__EMSCRIPTEN__)
    // Returning an empty category is deliberate: the Video category disappears
    // from the web build's menu rather than offering three entries that cannot
    // work. tools/check_catalog.py counts the native tree, so it still sees 3.
    return demos;
#else
    using namespace CnaExamples::Demos::Media::VideoDemos;
    demos.push_back(MakeDemo<LoadAndPlayScreen>(
        "Load & Play", "A real FFmpeg-decoded clip, live GetTexture() every frame",
        {"Video", "VideoPlayer::Play", "VideoPlayer::GetTexture"}));
    demos.push_back(MakeDemo<PlaybackControlScreen>(
        "Playback Control", "Play/Pause/Resume/Stop + IsLooped + live Volume/IsMuted",
        {"VideoPlayer::Pause", "VideoPlayer::IsLooped", "VideoPlayer::Volume"}));
    demos.push_back(MakeDemo<MultiTrackEXTScreen>(
        "Multi-Track (EXT)", "SetAudioTrackEXT()/SetVideoTrackEXT() -- CNA extensions",
        {"VideoPlayer::SetAudioTrackEXT", "VideoPlayer::SetVideoTrackEXT"}));
    return demos;
#endif
}

std::vector<DemoEntry> BuildMediaLibraryDemos() {
    using namespace CnaExamples::Demos::Media::MediaLibraryDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<CatalogAccessScreen>(
        "Catalog Access", "Index the bundled demo library or the real OS folders, live",
        {"MediaLibrary", "MediaSource", "MediaLibrary::IsDisposed"}));
    demos.push_back(MakeDemo<SongMetadataScreen>(
        "Song Metadata", "Name/Artist/Album/Genre/Duration/TrackNumber/Rating from real tags",
        {"MediaLibrary::Songs", "Song::Album", "Song::Artist", "Song::TrackNumber"}));
    demos.push_back(MakeDemo<AlbumArtistGenreScreen>(
        "Album/Artist/Genre", "The three grouping views derived from the same song tags",
        {"AlbumCollection", "ArtistCollection", "GenreCollection", "Album::HasArt"}));
    demos.push_back(MakeDemo<PlaylistScreen>(
        "Playlists", "Real .m3u parsing, entry order preserved, playable via MediaPlayer",
        {"PlaylistCollection", "Playlist::Songs", "Playlist::Duration"}));
    return demos;
}

std::vector<DemoEntry> BuildPictureDemos() {
    using namespace CnaExamples::Demos::Media::PictureDemos;
    std::vector<DemoEntry> demos;
    demos.push_back(MakeDemo<PictureBrowserScreen>(
        "Browse", "Picture metadata next to the decoded image it describes",
        {"MediaLibrary::Pictures", "Picture::Width", "Texture2D::FromStream"}));
    demos.push_back(MakeDemo<PictureAlbumTreeScreen>(
        "Album Tree", "Walk RootPictureAlbum -> Albums -> Pictures, with Parent back-refs",
        {"MediaLibrary::RootPictureAlbum", "PictureAlbum::Albums", "PictureAlbum::Parent"}));
    demos.push_back(MakeDemo<SavePictureScreen>(
        "SavePicture", "Write a generated BMP into the library; lazy \"Saved Pictures\" creation",
        {"MediaLibrary::SavePicture", "MediaLibrary::SavedPictures"}));
    demos.push_back(MakeDemo<PictureTokenScreen>(
        "Tokens & Identity", "GetPictureFromToken round trip vs Equals/GetHashCode",
        {"MediaLibrary::GetPictureFromToken", "Picture::Equals", "Picture::Date"}));
    return demos;
}

} // namespace CnaExamples::Navigation
