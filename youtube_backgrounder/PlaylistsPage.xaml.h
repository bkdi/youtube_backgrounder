﻿//
// PlaylistsPage.xaml.h
// Declaration of the PlaylistsPage class
//

#pragma once

#include "PlaylistsPage.g.h"
#include "YoutubePlaylist.h"

using namespace Windows::UI::Xaml;

namespace youtube_backgrounder
{
	public ref class PlaylistsPageNavParam sealed
	{
	public:
		PlaylistsPageNavParam(YoutubePlaylistsCollection^ playlists, Controls::Frame^ frame)
		{
			Playlists = playlists;
			PlayerFrame = frame;
		}

		property YoutubePlaylistsCollection^ Playlists;
		property Controls::Frame^ PlayerFrame;
	};

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class PlaylistsPage sealed
	{
	public:
		PlaylistsPage();

		property Controls::Frame^ PlayerFrame
		{
			Controls::Frame^ get() { return (Controls::Frame^)GetValue(_PlayerFrameProperty); }
			void set(Controls::Frame^ value) { SetValue(_PlayerFrameProperty, value); }
		}

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		const double minItemWidth = 200.0;
		const double maxItemWidth = 360.0;

		PlaylistsPageNavParam^ inputParams;

		static DependencyProperty^ _PlayerFrameProperty;

		void ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void AddNewPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void PlaylistsListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void DeletePlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
