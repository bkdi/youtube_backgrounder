//
// PlayerPage.xaml.h
// Declaration of the PlayerPage class
//

#pragma once

#include "PlayerPage.g.h"

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

		void InitializeTransportControls();
		void SystemControls_ButtonPressed(SystemMediaTransportControls^ sender, SystemMediaTransportControlsButtonPressedEventArgs^ args);
		void PlayMedia();
		void PauseMedia();
		void MusicPlayer_CurrentStateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
