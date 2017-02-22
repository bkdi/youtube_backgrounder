//
// PlayerPage.xaml.cpp
// Implementation of the PlayerPage class
//

#include "pch.h"
#include "PlayerPage2.xaml.h"

#include "YoutubeExtractor.h"
#include "YouTubeSignatureDecrypt.h"
#include "Settings.h"

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

PlayerPage2::PlayerPage2()
{
	InitializeComponent();

	player = ref new MediaPlayer;
	player->AudioCategory = MediaPlayerAudioCategory::Movie;
	player->AudioDeviceType = MediaPlayerAudioDeviceType::Multimedia;
	player->RealTimePlayback = true;
	player->AutoPlay = true;

	player->MediaEnded += ref new TypedEventHandler<MediaPlayer^, Platform::Object^> (this, &PlayerPage2::MediaEndedHandler);

	playerElement->SetMediaPlayer(player);
}

void PlayerPage2::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	if (e->Parameter != nullptr)
	{
		nowPlayingPlaylist = (safe_cast<YoutubePlaylist^> (e->Parameter));
		nowPlayingPlaylist->eventToken = nowPlayingPlaylist->PropertyChanged += ref new PropertyChangedEventHandler(this, &PlayerPage2::PlayItem);

		nowPlayingPlaylist->NowPlayingIndex = 0;
	}
}

void PlayerPage2::PlayItem(Platform::Object^ sender, PropertyChangedEventArgs^ e)
{
	if (nowPlayingPlaylist->NowPlayingIndex >= 0)
		playItem(nowPlayingPlaylist->Items->GetAt(nowPlayingPlaylist->NowPlayingIndex));
}

void PlayerPage2::MediaEndedHandler(MediaPlayer^ sender, Platform::Object^ e)
{
	try
	{
		if (nowPlayingPlaylist->NowPlayingIndex < nowPlayingPlaylist->Items->Size - 1)
			++nowPlayingPlaylist->NowPlayingIndex;
	}
	catch (Platform::ChangedStateException^)
	{

	}
}

void PlayerPage2::playItem(YoutubeItem^ item)
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
			player->Source = Windows::Media::Core::MediaSource::CreateFromUri(ref new Uri(urlToPlay));

		loadingProgress->IsActive = false;
		loadingProgress->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	});
}
