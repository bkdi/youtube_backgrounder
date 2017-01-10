//
// SearchPage.xaml.cpp
// Implementation of the SearchPage class
//

#include "pch.h"
#include "SearchPage.xaml.h"
#include "PlayerPage.xaml.h"

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
}

void SearchPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	inputParams = safe_cast<SearchPageNavParam^> (e->Parameter);

	itemsCollection = ref new YoutubeItemsCollections;
	gridresult->ItemsSource = itemsCollection->YoutubeItems;

	Playlists = inputParams->Playlists;

	loadYoutubeItems();
}

void SearchPage::loadYoutubeItems()
{
	auto httpClient = ref new HttpClient();

	Platform::String^ url = L"https://www.googleapis.com/youtube/v3/search?part=snippet&maxResults=16&q=" + inputParams->Title + L"&type=video&key=AIzaSyCIM4EzNqi1in22f4Z3Ru3iYvLaY8tc3bo";
	if (!nextPageToken->IsEmpty())
		url += L"&pageToken=" + nextPageToken;

	auto uri = ref new Uri(url);

	concurrency::create_task(httpClient->GetStringAsync(uri)).then([this](Platform::String^ jsonSearchedList)
	{
		std::wstringstream jsonStream(jsonSearchedList->Data());
		boost::property_tree::wptree pt;

		try
		{
			read_json(jsonStream, pt);

			auto it = pt.find(L"nextPageToken");
			if (it != pt.not_found())
				nextPageToken = ref new Platform::String(it->second.get_value<std::wstring>().c_str());

			for (const boost::property_tree::wptree::value_type& item : pt.get_child(L"items"))
			{
				Platform::String^ videoId = ref new Platform::String(item.second.find(L"id")->second.find(L"videoId")->second.get_value<std::wstring>().c_str());
				Platform::String^ title = ref new Platform::String(item.second.find(L"snippet")->second.find(L"title")->second.get_value<std::wstring>().c_str());
				Platform::String^ smallThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"default")->second.find(L"url")->second.get_value<std::wstring>().c_str());
				Platform::String^ largeThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"high")->second.find(L"url")->second.get_value<std::wstring>().c_str());

				itemsCollection->AppendItem(videoId, title, smallThumbnail, largeThumbnail);
			}
		}
		catch (const boost::property_tree::json_parser_error &)
		{
			//TODO:
		}

		EmptyPageInfoStackPanel->Visibility = itemsCollection->YoutubeItems->Size > 0 ? Windows::UI::Xaml::Visibility::Collapsed : Windows::UI::Xaml::Visibility::Visible;
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
		for (unsigned int i = 1; ; ++i)
		{
			auto itemWidth = e->NewSize.Width / i;
			if (itemWidth >= minItemWidth && itemWidth <= maxItemWidth)
			{
				itemWrapGrid->ItemWidth = itemWidth;
				itemWrapGrid->ItemHeight = itemWidth / (4.0/3.0);
				break;
			}
		}
	}
}

void SearchPage::scrollResult_ViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e)
{
	if (!e->IsIntermediate)
	{
		auto verticalOffset = scrollResult->VerticalOffset;
		auto maxVerticalOffset = scrollResult->ScrollableHeight;

		if (maxVerticalOffset < 0 || verticalOffset == maxVerticalOffset)
			loadYoutubeItems();
	}
}

Windows::Foundation::EventRegistrationToken youtube_backgrounder::SizeChange::eventToken;
DependencyProperty^ youtube_backgrounder::SizeChange::IsEnabledProperty = DependencyProperty::RegisterAttached("IsEnabled", bool::typeid, SizeChange::typeid, ref new PropertyMetadata(nullptr, ref new PropertyChangedCallback(&youtube_backgrounder::SizeChange::OnIsEnabledChanged)));

DependencyProperty^ youtube_backgrounder::SizeChange::ActualWidthProperty = DependencyProperty::RegisterAttached("ActualWidth", double::typeid, SizeChange::typeid, nullptr);