//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "SearchPage.xaml.h"
#include "SettingsPage.xaml.h"
#include "PlaylistsPage.xaml.h"
#include "PlayerPage.xaml.h"
#include "NowPlayingPage.xaml.h"
#include "PlaylistIO.h"
#include "ContentDialogTextInput.xaml.h"

using namespace youtube_backgrounder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Web::Http;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	playlists = ref new YoutubePlaylistsCollection;
	nowPlayingPlaylist = ref new YoutubePlaylist(L"");

	auto playlistLoader = ref new PlaylistIO;
	playlistLoader->Read(&playlists);
	PlaylistsListView->ItemsSource = playlists->PlaylistItems;

	SearchPageNavParam^ navParam = ref new SearchPageNavParam(L"", PlayerFrame, playlists, nowPlayingPlaylist);
	SearchFrame->Navigate(TypeName(SearchPage::typeid), navParam);
}

void MainPage::MenuButton_Click(Platform::Object^ sender, RoutedEventArgs^ e)
{
	MenuSplitView->OpenPaneLength = 150;
	MenuSplitView->IsPaneOpen = !MenuSplitView->IsPaneOpen;
}

void MainPage::SearchButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	(safe_cast<RadioButton^> (sender))->IsChecked = true;
	MenuSplitView->OpenPaneLength = 150;

	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MainPage::PlaylistsButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	MenuSplitView->OpenPaneLength = 500;
	MenuSplitView->IsPaneOpen = true;
	/*(safe_cast<RadioButton^> (sender))->IsChecked = true;

	PlaylistsPageNavParam^ navParam = ref new PlaylistsPageNavParam(playlists, nowPlayingPlaylist, PlayerFrame);
	PlaylistsFrame->Navigate(TypeName(PlaylistsPage::typeid), navParam);

	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;*/
}

void MainPage::NowPlayingButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	(safe_cast<RadioButton^> (sender))->IsChecked = true;
	MenuSplitView->OpenPaneLength = 150;

	NowPlayingFrame->Navigate(TypeName(NowPlayingPage::typeid), nowPlayingPlaylist);

	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MainPage::AutoSuggestBox_QuerySubmitted(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs^ args)
{
	if (!sender->Text->IsEmpty())
	{
		sender->IsSuggestionListOpen = false;

		SearchPageNavParam^ navParam = ref new SearchPageNavParam(sender->Text, PlayerFrame, playlists, nowPlayingPlaylist);
		SearchFrame->Navigate(TypeName(SearchPage::typeid), navParam);

		SearchButton->IsChecked = true;

		SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

		PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}

void MainPage::AutoSuggestBox_TextChanged(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs^ args)
{
	if (args->Reason == AutoSuggestionBoxTextChangeReason::UserInput && !sender->Text->IsEmpty())
	{
		auto httpClient = ref new HttpClient();
		auto uri = ref new Uri(L"http://suggestqueries.google.com/complete/search?client=firefox&ds=yt&q=" + sender->Text);

		concurrency::create_task(httpClient->GetStringAsync(uri)).then([this, sender](Platform::String^ youtubeSuggestQueriesFile)
		{
			auto suggestions = ref new Platform::Collections::Vector<Platform::String^>;
			std::wstringstream jsonStream(youtubeSuggestQueriesFile->Data());
			
			boost::property_tree::wptree pt;
			read_json(jsonStream, pt);
			try
			{
				//podpowiedzi występują w drugim elemencie bez nazwy
				for (auto item : pt.get_child(L"").rbegin()->second)
					suggestions->Append(ref new Platform::String(item.second.get_value<std::wstring>().c_str()));
			}
			catch (const boost::property_tree::json_parser_error &)
			{
				//TODO:
			}
			sender->ItemsSource = suggestions;
		});
	}
}

void MainPage::AutoSuggestBox_SuggestionChosen(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs^ args)
{
	sender->Text = args->SelectedItem->ToString();
}

void MainPage::WindowProportionButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	static int state = 1;

	MenuSplitView->OpenPaneLength = 150;

	++state %= 3;

	auto button = safe_cast<Button^> (sender);
	switch (state)
	{
	/*case 0: 
		button->Tag = L"0"; 

		MainGrid->RowDefinitions->GetAt(1)->Height = GridLength(0, GridUnitType::Star);
		MainGrid->RowDefinitions->GetAt(2)->Height = GridLength(1, GridUnitType::Star);
		break;*/
	case 0: 
		button->Tag = L"\u00BC"; 
		MainGrid->RowDefinitions->GetAt(1)->Height = GridLength(1, GridUnitType::Star);
		MainGrid->RowDefinitions->GetAt(2)->Height = GridLength(3, GridUnitType::Star);
		break;
	case 1: 
		button->Tag = L"\u00BD"; 
		MainGrid->RowDefinitions->GetAt(1)->Height = GridLength(1, GridUnitType::Star);
		MainGrid->RowDefinitions->GetAt(2)->Height = GridLength(1, GridUnitType::Star);
		break;
	case 2: 
		button->Tag = L"\u00BE"; 
		MainGrid->RowDefinitions->GetAt(1)->Height = GridLength(3, GridUnitType::Star);
		MainGrid->RowDefinitions->GetAt(2)->Height = GridLength(1, GridUnitType::Star);
		break;
	/*case 4: 
		button->Tag = L"1"; 

		MainGrid->RowDefinitions->GetAt(1)->Height = GridLength(1, GridUnitType::Star);
		MainGrid->RowDefinitions->GetAt(2)->Height = GridLength(0, GridUnitType::Star);
		
		break;*/
	}
}

void youtube_backgrounder::MainPage::AddPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto dialogPlaylistNameInput = ref new ContentDialogTextInput(playlists);

	concurrency::create_task(dialogPlaylistNameInput->ShowAsync()).then([this, dialogPlaylistNameInput](Controls::ContentDialogResult result)
	{
		if (result == Controls::ContentDialogResult::Primary)
		{
			auto playlist = ref new YoutubePlaylist(dialogPlaylistNameInput->Text);
			playlists->AppendPlaylist(playlist);
			auto playlistLoader = ref new PlaylistIO;
			playlistLoader->Write(playlists);
		}
	});
}


void youtube_backgrounder::MainPage::DeletePlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		auto playlist = safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem);
		playlist->clear();

		playlists->DeletePlaylist(playlist);
	}
}


void youtube_backgrounder::MainPage::PlaylistListViewItemControl_PlayButtonClick(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto element = safe_cast<FrameworkElement^> (sender);
	auto playlist = (safe_cast<YoutubePlaylist^> (element->DataContext));

	nowPlayingPlaylist->reset();
	nowPlayingPlaylist->Name = playlist->Name;
	for (auto item : playlist->Items)
		nowPlayingPlaylist->add(item);

	MenuSplitView->IsPaneOpen = false;
	PlayerFrame->Navigate(TypeName(PlayerPage::typeid), nowPlayingPlaylist);
}

void youtube_backgrounder::MainPage::PlaylistsListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	if (PlaylistsListView->SelectedItem != nullptr)
	{
		PlaylistsPageNavParam^ navParam = ref new PlaylistsPageNavParam(safe_cast<YoutubePlaylist^> (PlaylistsListView->SelectedItem));
		PlaylistsFrame->Navigate(TypeName(PlaylistsPage::typeid), navParam);

		PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

		SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
}