//
// PlayerPage.xaml.h
// Declaration of the PlayerPage class
//

#pragma once

#include "PlayerPage2.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::Media;
using namespace Windows::Media::Playback;

namespace youtube_backgrounder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlayerPage2 sealed
	{
	public:
		PlayerPage2();

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		YoutubePlaylist^ nowPlayingPlaylist;
		MediaPlayer^ player;

		void MediaEndedHandler(MediaPlayer^ sender, Platform::Object^ e);

		void PlayItem(Platform::Object^ sender, PropertyChangedEventArgs^ e);
		void playItem(YoutubeItem^ item);
	};
}
