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

DependencyProperty^ PlaylistsPage::_NowPlayingPlaylistProperty = DependencyProperty::Register(L"NowPlayingPlaylist", YoutubePlaylist::typeid, PlaylistsPage::typeid, nullptr);
DependencyProperty^ PlaylistsPage::_PlayerFrameProperty = DependencyProperty::Register(L"PlayerFrame", Controls::Frame::typeid, PlaylistsPage::typeid, nullptr);


PlaylistsPage::PlaylistsPage()
{
	InitializeComponent();
}

void PlaylistsPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	inputParams = safe_cast<PlaylistsPageNavParam^> (e->Parameter);

	PlaylistItemsGridView->ItemsSource = inputParams->Playlist->Items;
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
