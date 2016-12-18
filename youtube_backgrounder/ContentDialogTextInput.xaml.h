//
// ContentDialogTextInput.xaml.h
// Declaration of the ContentDialogTextInput class
//

#pragma once

#include "ContentDialogTextInput.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::UI::Xaml;

namespace youtube_backgrounder
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContentDialogTextInput sealed
	{
	public:
		ContentDialogTextInput(YoutubePlaylistsCollection^ playlists);

		property Platform::String^ Text
		{
			Platform::String^ get()
			{
				return (Platform::String^)GetValue(TextProperty);
			}

			void set(Platform::String^ value)
			{
				DependencyObject::SetValue(TextProperty, value);
			}
		}

	private:
		static DependencyProperty^ TextProperty;

		YoutubePlaylistsCollection^ Playlists;

		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void TextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
	};
}