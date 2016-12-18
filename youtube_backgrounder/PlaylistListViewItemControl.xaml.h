//
// PlaylistListViewItemControl.xaml.h
// Declaration of the PlaylistListViewItemControl class
//

#pragma once

#include "PlaylistListViewItemControl.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::UI::Xaml;

namespace youtube_backgrounder
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlaylistListViewItemControl sealed
	{
	public:
		PlaylistListViewItemControl();

		static property DependencyProperty^ PlaylistNameProperty
		{
			DependencyProperty^ get() { return _PlaylistNameProperty; }
		}

		static property DependencyProperty^ TracksCountProperty
		{
			DependencyProperty^ get() { return _TracksCountProperty; }
		}

		static property DependencyProperty^ PlayerFrameProperty
		{
			DependencyProperty^ get() { return _PlayerFrameProperty; }
		}

		property Platform::String^ PlaylistName
		{
			Platform::String^ get() { return (Platform::String^)GetValue(PlaylistNameProperty); }
			void set(Platform::String^ value) { SetValue(PlaylistNameProperty, value); }
		}

		property unsigned int TracksCount
		{
			unsigned int get() { return (unsigned int)GetValue(TracksCountProperty); }
			void set(unsigned int value) { SetValue(TracksCountProperty, value); }
		}

		property Controls::Frame^ PlayerFrame
		{
			Controls::Frame^ get() { return (Controls::Frame^)GetValue(PlayerFrameProperty); }
			void set(Controls::Frame^ value) { SetValue(PlayerFrameProperty, value); }
		}

	private:
		static DependencyProperty^ _PlaylistNameProperty;
		static DependencyProperty^ _TracksCountProperty;
		static DependencyProperty^ _PlayerFrameProperty;

		void PlaylistPlayButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};

	public ref class PlaylistItemsCountToStrConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:

		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};
}
