//
// PlaylistsPage.xaml.cpp
// Implementation of the PlaylistsPage class
//

#include "pch.h"
#include "PlaylistsPage.xaml.h"
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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

DependencyProperty^ PlaylistsPage::_PlayerFrameProperty = DependencyProperty::Register(L"PlayerFrame", Controls::Frame::typeid, PlaylistsPage::typeid, nullptr);

PlaylistsPage::PlaylistsPage()
{
	InitializeComponent();
}

void PlaylistsPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	inputParams = safe_cast<PlaylistsPageNavParam^> (e->Parameter);

	PlaylistsListView->ItemsSource = inputParams->Playlists->PlaylistItems;
	PlayerFrame = inputParams->PlayerFrame;
}

void PlaylistsPage::ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	ItemsWrapGrid^ itemWrapGrid = safe_cast<ItemsWrapGrid^>(sender);

	if (e->NewSize.Width != e->PreviousSize.Width)
	{
		for (unsigned int i = 1; ; ++i)
		{
			auto itemWidth = e->NewSize.Width / i;
			if (itemWidth >= minItemWidth && itemWidth <= maxItemWidth)
			{
				itemWrapGrid->ItemWidth = itemWidth;
				itemWrapGrid->ItemHeight = itemWidth / (4.0 / 3.0);
				break;
			}
		}
	}
}

/*double itemWidth = 0.0;
for (unsigned int i = 1; i <= gridresult->Items->Size; ++i)
{
	itemWidth = e->NewSize.Width / i;
	if (itemWidth >= minItemWidth && itemWidth <= maxItemWidth)
		break;
}
if (itemWidth > maxItemWidth)
itemWidth = maxItemWidth;
else if (itemWidth < minItemWidth)
	itemWidth = minItemWidth;

itemWrapGrid->ItemWidth = itemWidth;
itemWrapGrid->ItemHeight = itemWidth / (4.0 / 3.0);*/

void PlaylistsPage::AddNewPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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

void PlaylistsPage::DeletePlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);
		playlist->clear();

		inputParams->Playlists->DeletePlaylist(playlist);
	}
}

void PlaylistsPage::PlaylistsListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);

		PlaylistItemsGridView->ItemsSource = playlist->getItems();
	}
}

