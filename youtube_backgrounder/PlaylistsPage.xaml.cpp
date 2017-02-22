//
// PlaylistsPage.xaml.cpp
// Implementation of the PlaylistsPage class
//

#include "pch.h"
#include "PlaylistsPage.xaml.h"
#include "PlayerPage.xaml.h"
#include "ContentDialogTextInput.xaml.h"
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
using namespace Windows::UI::Xaml::Interop;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

PlaylistsPage::PlaylistsPage()
{
	InitializeComponent();
}

void PlaylistsPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	inputParams = safe_cast<PlaylistsPageNavParam^> (e->Parameter);

	this->DataContext = inputParams->Playlists;
	//PlaylistsListView->ItemsSource = inputParams->Playlists->PlaylistItems;
}

void PlaylistsPage::ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	ItemsWrapGrid^ itemWrapGrid = safe_cast<ItemsWrapGrid^>(sender);

	if (e->NewSize.Width != e->PreviousSize.Width)
	{
		double itemWidth;
		if (e->NewSize.Width < 2 * minItemWidth)
		{
			itemWidth = e->NewSize.Width;
			if (itemWidth > maxItemWidth)
				itemWidth = maxItemWidth;
			else if (itemWidth < minItemWidth)
				itemWidth = minItemWidth;
		}
		else
		{
			for (unsigned int i = 1; ; ++i)
			{
				itemWidth = e->NewSize.Width / i;
				if (itemWidth >= minItemWidth && itemWidth <= maxItemWidth)
					break;
			}
		}
		itemWrapGrid->ItemWidth = itemWidth;
		itemWrapGrid->ItemHeight = itemWidth / (4.0 / 3.0);
	}
}

void youtube_backgrounder::PlaylistsPage::AddPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto dialogPlaylistNameInput = ref new ContentDialogTextInput(inputParams->Playlists);

	concurrency::create_task(dialogPlaylistNameInput->ShowAsync()).then([this, dialogPlaylistNameInput](Controls::ContentDialogResult result)
	{
		if (result == Controls::ContentDialogResult::Primary)
		{
			auto playlist = ref new YoutubePlaylist(dialogPlaylistNameInput->Text);
			inputParams->Playlists->AppendPlaylist(playlist);
			auto playlistLoader = ref new PlaylistIO;
			playlistLoader->Write(inputParams->Playlists);
		}
	});
}

void youtube_backgrounder::PlaylistsPage::DeletePlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);
		playlist->clear();

		inputParams->Playlists->DeletePlaylist(playlist);
		auto playlistLoader = ref new PlaylistIO;
		playlistLoader->Write(inputParams->Playlists);
	}
}

void youtube_backgrounder::PlaylistsPage::PlaylistListViewItemControl_PlayButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto element = safe_cast<FrameworkElement^> (sender);
	auto playlist = (safe_cast<YoutubePlaylist^> (element->DataContext));

	inputParams->NowPlayingPlaylist->reset();
	inputParams->NowPlayingPlaylist->Name = playlist->Name;
	for (auto item : playlist->Items)
		inputParams->NowPlayingPlaylist->add(item);

	inputParams->PlayerFrame->Navigate(TypeName(PlayerPage::typeid), inputParams->NowPlayingPlaylist);
}

void youtube_backgrounder::PlaylistsPage::PlaylistsListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);
		PlaylistItemsGridView->ItemsSource = playlist->Items;
	}
}

void youtube_backgrounder::PlaylistsPage::DeleteItemButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (PlaylistItemsGridView->SelectedItem != nullptr)
	{
		auto item = safe_cast<YoutubeItem^> (PlaylistItemsGridView->SelectedItem);
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);

		playlist->deleteItem(item);

		auto playlistLoader = ref new PlaylistIO;
		playlistLoader->Write(inputParams->Playlists);
	}
}
