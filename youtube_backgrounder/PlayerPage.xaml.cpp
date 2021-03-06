﻿//
// PlayerPage.xaml.cpp
// Implementation of the PlayerPage class
//

#include "pch.h"
#include "PlayerPage.xaml.h"

#include "YoutubeExtractor.h"
#include "YouTubeSignatureDecrypt.h"
#include "Settings.h"
#include "YoutubeSearch.h"
#include "YoutubeVideoDetails.h"

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

PlayerPage::PlayerPage()
{
	InitializeComponent();
	InitializeTransportControls();
}

void PlayerPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	if (e->Parameter != nullptr)
	{
		nowPlayingPlaylist = (safe_cast<YoutubePlaylist^> (e->Parameter));
		nowPlayingPlaylist->eventToken = nowPlayingPlaylist->PropertyChanged += ref new PropertyChangedEventHandler(this, &PlayerPage::PlayItem);

		nowPlayingPlaylist->NowPlayingIndex = 0;
	}
}

void PlayerPage::InitializeTransportControls()
{
	// Hook up app to system transport controls.
	systemControls = SystemMediaTransportControls::GetForCurrentView();
	systemControls->ButtonPressed += ref new TypedEventHandler<SystemMediaTransportControls^, SystemMediaTransportControlsButtonPressedEventArgs^>(this, &PlayerPage::SystemControls_ButtonPressed);

	// Register to handle the following system transpot control buttons.
	systemControls->IsPlayEnabled = true;
	systemControls->IsPauseEnabled = true;
	systemControls->IsPreviousEnabled = true;
	systemControls->IsNextEnabled = true;
}

void PlayerPage::PlayItem(Platform::Object^ sender, PropertyChangedEventArgs^ e)
{
	if (e->PropertyName == L"NowPlayingIndex")
	{
		if (nowPlayingPlaylist->NowPlayingIndex >= 0 && nowPlayingPlaylist->NowPlayingIndex < nowPlayingPlaylist->TracksCount)
			playItem(nowPlayingPlaylist->Items->GetAt(nowPlayingPlaylist->NowPlayingIndex));

		auto controls = safe_cast<CustomMediaTransportControls^> (musicPlayer->TransportControls);
		if (controls->IsSearchRelated && nowPlayingPlaylist->TracksCount > 0 && ((nowPlayingPlaylist->TracksCount - 1) - nowPlayingPlaylist->NowPlayingIndex) < 3)
		{
			auto youtubeSearch = ref new YoutubeSearch;

			concurrency::create_task(youtubeSearch->searchRelated(nowPlayingPlaylist->getItems()->GetAt(nowPlayingPlaylist->TracksCount - 1)->VideoId, 20)).then([this](YoutubeItemsCollections^ searchedResults)
			{
				auto youtubeVideoDetails = ref new YoutubeVideoDetails;
				concurrency::create_task(youtubeVideoDetails->fillVideosDetails(searchedResults)).then([this, searchedResults]()
				{
					std::sort(begin(searchedResults->YoutubeItems), end(searchedResults->YoutubeItems));
					nowPlayingPlaylist->appendNonexistent(searchedResults, 3);
				}).then([this](concurrency::task<void> t)
				{
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
			}).then([this](concurrency::task<void> t)
			{
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
	}
}

void PlayerPage::SystemControls_ButtonPressed(SystemMediaTransportControls^ sender, SystemMediaTransportControlsButtonPressedEventArgs^ args)
{
	switch (args->Button)
	{
	case SystemMediaTransportControlsButton::Play:
		PlayMedia();
		break;
	case SystemMediaTransportControlsButton::Pause:
		PauseMedia();
		break;
	case SystemMediaTransportControlsButton::Next:
		NextItem();
		break;
	case SystemMediaTransportControlsButton::Previous:
		PreviousItem();
		break;
	default:
		break;
	}
}

void PlayerPage::PlayMedia()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		musicPlayer->Play();
	}));
}

void PlayerPage::PauseMedia()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		musicPlayer->Pause();
	}));
}

void PlayerPage::NextItem()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		nextTrack();
	}));
}

void PlayerPage::PreviousItem()
{
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this]()
	{
		if (nowPlayingPlaylist->NowPlayingIndex > 0)
			--nowPlayingPlaylist->NowPlayingIndex;
	}));
}

void PlayerPage::MusicPlayer_CurrentStateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
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

void youtube_backgrounder::PlayerPage::musicPlayer_MediaEnded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{	
	nextTrack();
}

void PlayerPage::playItem(YoutubeItem^ item)
{
	loadingProgress->Visibility = Windows::UI::Xaml::Visibility::Visible;
	loadingProgress->IsActive = true;

	auto preferedQuality = safe_cast<YoutubeQuality> (SettingsHelper::getPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY));
	YoutubeQualityItag preferedItag;
	IVector<YoutubeQualityItag>^ sortedQualities;

	auto onlyAuto = SettingsHelper::getPropertyBoolean(Settings::MATERIAL, Settings::Material::ONLYAUDIO);
	if (onlyAuto)
		preferedItag = YoutubeQualityItem::qualityTo_DASH_Audio_Quality(preferedQuality);
	else
		preferedItag = YoutubeQualityItem::qualityTo_Non_DASH_2D_Quality(preferedQuality);

	auto youtubeExtractor = ref new YoutubeExtractor(item->VideoId, preferedItag, onlyAuto);
	concurrency::create_task(youtubeExtractor->getVideoUrlByItagAsync()).then([this](Platform::String^ urlToPlay)
	{
		if (!urlToPlay->IsEmpty())
			musicPlayer->Source = ref new Uri(urlToPlay);

		loadingProgress->IsActive = false;
		loadingProgress->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	});
}

void PlayerPage::nextTrack()
{
	auto controls = safe_cast<CustomMediaTransportControls^> (musicPlayer->TransportControls);
	
	try
	{
		if (nowPlayingPlaylist->NowPlayingIndex < nowPlayingPlaylist->TracksCount - 1)
			++nowPlayingPlaylist->NowPlayingIndex;
		else if (controls->IsRepeat)
			nowPlayingPlaylist->NowPlayingIndex = 0;
	}
	catch (Platform::ChangedStateException^)
	{

	}
}

void PlayerPage::previousTrack()
{
	try
	{
		if (nowPlayingPlaylist->NowPlayingIndex > 0)
			--nowPlayingPlaylist->NowPlayingIndex;
	}
	catch (Platform::ChangedStateException^)
	{

	}
}

void PlayerPage::CustomMediaTransportControls_NextTrackButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	nextTrack();
}


void PlayerPage::CustomMediaTransportControls_PreviousTrackButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	previousTrack();
}
