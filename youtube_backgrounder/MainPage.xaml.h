//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "YoutubePlaylist.h"

namespace youtube_backgrounder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		property bool Internet
		{
			bool get() { return internet; }
			void set(bool value)
			{
				internet = value;
				OnPropertyChanged(L"Internet");
			}
		}

	protected:
		void OnPropertyChanged(Platform::String^ name)
		{
			PropertyChanged(this, ref new PropertyChangedEventArgs(name));
		}

	private:
		YoutubePlaylist^ nowPlayingPlaylist;
		YoutubePlaylistsCollection^ playlists;
		bool querySubmitted;
		bool internet;

		void MenuButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SearchButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void PlaylistsButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void AutoSuggestBox_QuerySubmitted(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs^ args);
		void AutoSuggestBox_TextChanged(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs^ args);
		void AutoSuggestBox_SuggestionChosen(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs^ args);
		void NowPlayingButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void WindowProportionButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void InternetChanged(Platform::Object^ sender, PropertyChangedEventArgs^ e);
		void NetworkInformation_NetworkStatusChanged(Platform::Object^ sender);
		virtual event PropertyChangedEventHandler^ PropertyChanged;
	};
}
