﻿//
// NowPlayingPage.xaml.h
// Declaration of the NowPlayingPage class
//

#pragma once

#include "NowPlayingPage.g.h"

#include "YoutubePlaylist.h"

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

	private:
		YoutubePlaylist^ nowPlayingPlaylist;
	};
}