//
// CustomMediaTransportControls.cpp
// Implementation of the CustomMediaTransportControls class.
//

#include "pch.h"
#include "CustomMediaTransportControls.h"

using namespace youtube_backgrounder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;

// The Templated Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234235

CustomMediaTransportControls::CustomMediaTransportControls()
{
	DefaultStyleKey = "youtube_backgrounder.CustomMediaTransportControls";

	IsRepeat = false;
	IsSearchRelated = false;
}

void CustomMediaTransportControls::OnApplyTemplate()
{
	// This is where you would get your custom button and create an event handler for its click method.
	auto button = safe_cast<Button^> (GetTemplateChild("NextTrackButton"));
	button->Click += ref new RoutedEventHandler(this, &CustomMediaTransportControls::OnNextTrackClick);

	button = safe_cast<Button^> (GetTemplateChild("PreviousTrackButton"));
	button->Click += ref new RoutedEventHandler(this, &CustomMediaTransportControls::OnPreviousTrackClick);

	auto toggleButton = safe_cast<AppBarToggleButton^> (GetTemplateChild("SearchRelatedVideosButton"));
	toggleButton->Click += ref new RoutedEventHandler(this, &CustomMediaTransportControls::OnSearchRelatedVideosClick);

	toggleButton = safe_cast<AppBarToggleButton^> (GetTemplateChild("RepeatButton"));
	toggleButton->Click += ref new RoutedEventHandler(this, &CustomMediaTransportControls::OnRepeatClick);

	MediaTransportControls::OnApplyTemplate();
}

void CustomMediaTransportControls::OnNextTrackClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	NextTrackButtonClick(this, e);
}

void CustomMediaTransportControls::OnPreviousTrackClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	PreviousTrackButtonClick(this, e);
}

void CustomMediaTransportControls::OnSearchRelatedVideosClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IsSearchRelated = (safe_cast<AppBarToggleButton^> (sender))->IsChecked->Value;
}

void CustomMediaTransportControls::OnRepeatClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	IsRepeat = (safe_cast<AppBarToggleButton^> (sender))->IsChecked->Value;
}
