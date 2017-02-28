//
// PlayerPage.xaml.h
// Declaration of the PlayerPage class
//

#pragma once

#include "PlayerPage.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::Media;

namespace youtube_backgrounder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlayerPage sealed
	{
	public:
		PlayerPage();

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		SystemMediaTransportControls^ systemControls;
		YoutubePlaylist^ nowPlayingPlaylist;

		void InitializeTransportControls();
		void SystemControls_ButtonPressed(SystemMediaTransportControls^ sender, SystemMediaTransportControlsButtonPressedEventArgs^ args);
		void PlayMedia();
		void PauseMedia();
		void NextItem();
		void nextTrack();
		void PreviousItem();
		void previousTrack();
		void MusicPlayer_CurrentStateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void musicPlayer_MediaEnded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void PlayItem(Platform::Object^ sender, PropertyChangedEventArgs^ e);
		void playItem(YoutubeItem^ item);
		void CustomMediaTransportControls_NextTrackButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CustomMediaTransportControls_PreviousTrackButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
