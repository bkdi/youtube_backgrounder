//
// NowPlayingPage.xaml.h
// Declaration of the NowPlayingPage class
//

#pragma once

#include "NowPlayingPage.g.h"

namespace youtube_backgrounder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class NowPlayingPage sealed
	{
	public:
		NowPlayingPage();

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	};

	public ref class NowPlayingBackgroundConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:

		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};
}
