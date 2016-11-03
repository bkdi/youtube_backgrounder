//
// PlayerPage.xaml.cpp
// Implementation of the PlayerPage class
//

#include "pch.h"
#include "PlayerPage.xaml.h"

using namespace youtube_backgrounder;

using namespace Windows::Media::Core;
using namespace Windows::Foundation;
using namespace Windows::Storage;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

PlayerPage::PlayerPage()
{
	InitializeComponent();
	player = ref new MediaPlayer;
	player->AudioCategory = MediaPlayerAudioCategory::Movie;
	player->AudioDeviceType = MediaPlayerAudioDeviceType::Multimedia;

	playerElement->SetMediaPlayer(player);
}

void PlayerPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	Platform::String^ url = safe_cast<Platform::String^> (e->Parameter);

	if (!url->IsEmpty())
		player->Source = MediaSource::CreateFromUri(ref new Uri(url));

	/*concurrency::create_task(StorageFile::GetFileFromPathAsync(L"C:\\Users\\Bartłomiej\\Videos\\videoplayback.mp4")).then([this](IStorageFile^ file)
	{
		player->Source = MediaSource::CreateFromStorageFile(file);
	});*/
	
}