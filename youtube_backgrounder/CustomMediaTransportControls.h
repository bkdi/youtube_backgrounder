//
// CustomMediaTransportControls.h
// Declaration of the CustomMediaTransportControls class.
//

#pragma once

using namespace Windows::UI::Xaml::Controls;

namespace youtube_backgrounder
{
	public ref class CustomMediaTransportControls sealed : public Windows::UI::Xaml::Controls::MediaTransportControls
	{
	public:
		CustomMediaTransportControls();

		event Windows::Foundation::EventHandler<Windows::UI::Xaml::RoutedEventArgs^ >^ NextTrackButtonClick;
		event Windows::Foundation::EventHandler<Windows::UI::Xaml::RoutedEventArgs^ >^ PreviousTrackButtonClick;

		property bool IsRepeat;
		property bool IsSearchRelated;

	protected:
		void OnApplyTemplate() override;

	private:
		

		void OnNextTrackClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnPreviousTrackClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnSearchRelatedVideosClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void OnRepeatClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
