//
// ContentDialogAddToPlaylist.xaml.h
// Declaration of the ContentDialogAddToPlaylist class
//

#pragma once

#include "ContentDialogAddToPlaylist.g.h"
#include "YoutubePlaylist.h"

namespace youtube_backgrounder
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentDialogAddToPlaylist sealed
	{
	public:
		ContentDialogAddToPlaylist(YoutubePlaylistsCollection^ playlists_, YoutubeItem^ youtubeItem);
	private:
		YoutubeItem^ item;
		YoutubePlaylistsCollection^ playlists;

		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void PlaylistsComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
