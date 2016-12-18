#pragma once

#include "YoutubePlaylist.h"

namespace youtube_backgrounder
{
	public ref class PlaylistIO sealed
	{
	public:
		PlaylistIO();

		void Read(YoutubePlaylistsCollection^* playlists);
		void Write(YoutubePlaylistsCollection^ playlists);

	private:
		Platform::String^ generateJSON(YoutubePlaylistsCollection^ playlists);
		void parseJSON(Platform::String^ playlistsJSON, YoutubePlaylistsCollection^* playlists);
	};
}