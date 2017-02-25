//
// SearchPage.xaml.cpp
// Implementation of the SearchPage class
//

#include "pch.h"
#include "SearchPage.xaml.h"
#include "PlayerPage.xaml.h"
#include "PlayerPage2.xaml.h"
#include "YoutubeAPI_const.h"

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
using namespace Windows::Storage;
using namespace Windows::Web::Http;
using namespace Windows::UI::Xaml::Interop;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

DependencyProperty^ SearchPage::_PlaylistsProperty = DependencyProperty::Register("Playlists", YoutubePlaylistsCollection::typeid, SearchPage::typeid, nullptr);

SearchPage::SearchPage()
{
	InitializeComponent();

	itemsCollection = ref new YoutubeItemsCollections;
	gridresult->ItemsSource = itemsCollection->YoutubeItems;

	youtubeSearch = ref new YoutubeSearch;
}

void SearchPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	if (e->Parameter != nullptr)
	{
		inputParams = safe_cast<SearchPageNavParam^> (e->Parameter);

		Playlists = inputParams->Playlists;

		if (!inputParams->Title->IsEmpty())
			loadYoutubeItems(false);
	}
	else
		EmptyPageInfoStackPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void SearchPage::loadYoutubeItems(bool loadMore)
{
	IAsyncOperation<YoutubeItemsCollections^>^ oper;
	if (loadMore)
		oper = youtubeSearch->searchMore(10);
	else
		oper = youtubeSearch->search(inputParams->Title, inputParams->ResultsOrder, 16);

	concurrency::create_task(oper).then([this](YoutubeItemsCollections^ searchedResults)
	{
		if(searchedResults != nullptr)
			itemsCollection->Append(searchedResults);
	}).then([this](concurrency::task<void> t)
	{
		if (itemsCollection->YoutubeItems->Size == 0)
			EmptyPageInfoStackPanel->Visibility = Windows::UI::Xaml::Visibility::Visible;

		try
		{
			t.get();
		}
		catch (Platform::COMException^ e)
		{
			auto dialog = ref new Windows::UI::Popups::MessageDialog("Error loading YouTube items. Please check your internet connection.");
			dialog->ShowAsync();
		}
		catch (Platform::NullReferenceException^ e)
		{

		}
	});
}

void SearchPage::gridresult_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	auto youtubeItem = safe_cast<YoutubeItem^> (e->ClickedItem);

	inputParams->NowPlayingPlaylist->reset();
	inputParams->NowPlayingPlaylist->add(youtubeItem);
	inputParams->PlayerFrame->Navigate(TypeName(PlayerPage::typeid), inputParams->NowPlayingPlaylist);
}

void SearchPage::ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
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

void SearchPage::scrollResult_ViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e)
{
	if (!e->IsIntermediate)
	{
		auto verticalOffset = scrollResult->VerticalOffset;
		auto maxVerticalOffset = scrollResult->ScrollableHeight;

		if (maxVerticalOffset < 0 || verticalOffset == maxVerticalOffset)
			loadYoutubeItems(true);
	}
}

Windows::Foundation::EventRegistrationToken youtube_backgrounder::SizeChange::eventToken;
DependencyProperty^ youtube_backgrounder::SizeChange::IsEnabledProperty = DependencyProperty::RegisterAttached("IsEnabled", bool::typeid, SizeChange::typeid, ref new PropertyMetadata(nullptr, ref new PropertyChangedCallback(&youtube_backgrounder::SizeChange::OnIsEnabledChanged)));

DependencyProperty^ youtube_backgrounder::SizeChange::ActualWidthProperty = DependencyProperty::RegisterAttached("ActualWidth", double::typeid, SizeChange::typeid, nullptr);