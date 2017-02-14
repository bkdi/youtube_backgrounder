//
// ContentDialogAddToPlaylist.xaml.cpp
// Implementation of the ContentDialogAddToPlaylist class
//

#include "pch.h"
#include "ContentDialogAddToPlaylist.xaml.h"
#include "PlaylistIO.h"

using namespace youtube_backgrounder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Content Dialog item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

youtube_backgrounder::ContentDialogAddToPlaylist::ContentDialogAddToPlaylist(YoutubePlaylistsCollection^ playlists_, YoutubeItem^ youtubeItem) : item(youtubeItem), playlists(playlists_)
{
	InitializeComponent();
	PlaylistsComboBox->ItemsSource = playlists->PlaylistItems;
}

void youtube_backgrounder::ContentDialogAddToPlaylist::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
	(safe_cast<YoutubePlaylist^> (PlaylistsComboBox->SelectedItem))->add(item);
	auto playlistLoader = ref new PlaylistIO();
	playlistLoader->Write(playlists);
}

void youtube_backgrounder::ContentDialogAddToPlaylist::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
}


void ContentDialogAddToPlaylist::PlaylistsComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	IsPrimaryButtonEnabled = PlaylistsComboBox->SelectedIndex > -1;
}
