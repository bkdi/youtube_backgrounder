//
// SettingsPage.xaml.h
// Declaration of the SettingsPage class
//

#pragma once

#include "SettingsPage.g.h"
#include "YoutubeQuality.h"

namespace youtube_backgrounder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SettingsPage sealed
	{
	public:
		SettingsPage();
	private:
		void QualityCombo_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
