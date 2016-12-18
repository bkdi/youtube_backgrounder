//
// GridViewItemControl.xaml.h
// Declaration of the GridViewItemControl class
//

#pragma once

#include "GridViewItemControl.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::UI::Xaml;

namespace youtube_backgrounder
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class GridViewItemControl sealed
	{
	public:
		GridViewItemControl();

		static property DependencyProperty^ ItemTitleProperty
		{
			DependencyProperty^ get() { return _ItemTitleProperty; }
		}

		static property DependencyProperty^ ThumbnailSourceProperty
		{
			DependencyProperty^ get() { return _ThumbnailSourceProperty; }
		}

		static property DependencyProperty^ PlaylistsProperty
		{
			DependencyProperty^ get() { return _PlaylistsProperty; }
		}

		property Platform::String^ ItemTitle
		{
			Platform::String^ get() { return (Platform::String^)GetValue(ItemTitleProperty);}
			void set(Platform::String^ value) {SetValue(ItemTitleProperty, value);}
		}

		property Platform::String^ ThumbnailSource
		{
			Platform::String^ get() {return (Platform::String^)GetValue(ThumbnailSourceProperty);}
			void set(Platform::String^ value) {SetValue(ThumbnailSourceProperty, value);}
		}

		property YoutubePlaylistsCollection^ Playlists
		{
			YoutubePlaylistsCollection^ get() { return (YoutubePlaylistsCollection^)GetValue(PlaylistsProperty); }
			void set(YoutubePlaylistsCollection^ value) { SetValue(PlaylistsProperty, value); }
		}

	private:
		static DependencyProperty^ _ItemTitleProperty;
		static DependencyProperty^ _ThumbnailSourceProperty;
		static DependencyProperty^ _PlaylistsProperty;

		void ItemArea_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ItemArea_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void AddToPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};

	public ref class ItemWidthToFontSizeConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:

		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};
}
