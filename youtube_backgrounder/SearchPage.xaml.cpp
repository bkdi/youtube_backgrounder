//
// SearchPage.xaml.cpp
// Implementation of the SearchPage class
//

#include "pch.h"
#include "SearchPage.xaml.h"
#include "PlayerPage.xaml.h"

#include "YoutubeExtractor.h"
#include "YouTubeSignatureDecrypt.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

SearchPage::SearchPage()
{
	InitializeComponent();
	InitializeTransportControls();
}

void SearchPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	currentSearchedTitle = safe_cast<Platform::String^> (e->Parameter);
	itemsCollection = ref new YoutubeItemsCollections;
	gridresult->ItemsSource = itemsCollection->YoutubeMiniatures;

	loadYoutubeItems();
}

void youtube_backgrounder::SearchPage::loadYoutubeItems()
{
	auto httpClient = ref new HttpClient();

	Platform::String^ url = L"https://www.googleapis.com/youtube/v3/search?part=snippet&maxResults=16&q=" + currentSearchedTitle + L"&type=video&key=AIzaSyCIM4EzNqi1in22f4Z3Ru3iYvLaY8tc3bo";
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

			nextPageToken = ref new Platform::String(pt.find(L"nextPageToken")->second.get_value<std::wstring>().c_str());

			auto itemsArray = pt.get_child(L"items");
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
	});
}

void SearchPage::gridresult_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e)
{
	YoutubeItem^ youtubeItem = safe_cast<YoutubeItem^> (e->ClickedItem);

	YoutubeExtractor^ youtubeExtractor = ref new YoutubeExtractor(youtubeItem->VideoId);
	concurrency::create_task(youtubeExtractor->getVideoUrlByItagAsync(L"22")).then([this](Platform::String^ urlToPlay)
	{
		if (!urlToPlay->IsEmpty())
		{
			musicPlayer->Source = ref new Uri(urlToPlay);
			musicPlayer->Play();
		}
	});
}

void youtube_backgrounder::SearchPage::ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	ItemsWrapGrid^ itemWrapGrid = safe_cast<ItemsWrapGrid^>(sender);
	
	for (double d = 360.0; d > 200.0; d -= 0.1)
	{
		double ratio = e->NewSize.Width / d;
		if ((ratio - (int)ratio) < 0.001)
		{
			itemWrapGrid->ItemWidth = d;
			break;
		}
	}
}

void youtube_backgrounder::SearchPage::scrollResult_ViewChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::ScrollViewerViewChangedEventArgs^ e)
{
	if (!e->IsIntermediate)
	{
		auto verticalOffset = scrollResult->VerticalOffset;
		auto maxVerticalOffset = scrollResult->ScrollableHeight;

		if (maxVerticalOffset < 0 || verticalOffset == maxVerticalOffset)
		{
			verticalOffset = 0;
			loadYoutubeItems();
		}
	}
}

void SearchPage::InitializeTransportControls()
{
	// Hook up app to system transport controls.
	systemControls = SystemMediaTransportControls::GetForCurrentView();
	systemControls->ButtonPressed += ref new TypedEventHandler<SystemMediaTransportControls^, SystemMediaTransportControlsButtonPressedEventArgs^>(this, &SearchPage::SystemControls_ButtonPressed);

	// Register to handle the following system transpot control buttons.
	systemControls->IsPlayEnabled = true;
	systemControls->IsPauseEnabled = true;
}

void SearchPage::SystemControls_ButtonPressed(SystemMediaTransportControls^ sender, SystemMediaTransportControlsButtonPressedEventArgs^ args)
{
	switch (args->Button)
	{
	case SystemMediaTransportControlsButton::Play:
		PlayMedia();
		break;
	case SystemMediaTransportControlsButton::Pause:
		PauseMedia();
		break;
	default:
		break;
	}
}

void SearchPage::PlayMedia()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		musicPlayer->Play();
	}));
}

void SearchPage::PauseMedia()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		musicPlayer->Pause();
	}));
}

void youtube_backgrounder::SearchPage::musicPlayer_CurrentStateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	switch (musicPlayer->CurrentState)
	{
	case MediaElementState::Playing:
		systemControls->PlaybackStatus = MediaPlaybackStatus::Playing;
		break;
	case MediaElementState::Paused:
		systemControls->PlaybackStatus = MediaPlaybackStatus::Paused;
		break;
	case MediaElementState::Stopped:
		systemControls->PlaybackStatus = MediaPlaybackStatus::Stopped;
		break;
	case MediaElementState::Closed:
		systemControls->PlaybackStatus = MediaPlaybackStatus::Closed;
		break;
	default:
		break;
	}
}


Platform::Object^ youtube_backgrounder::ItemWidthStateConverter::Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	bool active = false;
	double width = safe_cast<double> (value);

	if (parameter != nullptr)
	{
		Platform::String^ sName = parameter->ToString();

		/*if (sName == "BigFont")
		{
			if (width > 400)
				active = true;
		}
		else if (sName == "MediumFont")
		{
			if (width > 320 && width <= 400)
				active = true;
		}
		else if (sName == "SmallFont")
		{
			if (width <= 320)
				active = true;
		}*/
	}

	return active;
}

Platform::Object^ youtube_backgrounder::ItemWidthStateConverter::ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	throw ref new Platform::NotImplementedException();
}

Windows::Foundation::EventRegistrationToken youtube_backgrounder::SizeChange::eventToken;
DependencyProperty^ youtube_backgrounder::SizeChange::IsEnabledProperty = DependencyProperty::RegisterAttached("IsEnabled", bool::typeid, SizeChange::typeid, ref new PropertyMetadata(nullptr, ref new PropertyChangedCallback(&youtube_backgrounder::SizeChange::OnIsEnabledChanged)));

DependencyProperty^ youtube_backgrounder::SizeChange::ActualWidthProperty = DependencyProperty::RegisterAttached("ActualWidth", double::typeid, SizeChange::typeid, nullptr);