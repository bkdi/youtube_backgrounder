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
#include "PlayerPage2.xaml.h"
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
using namespace Windows::Networking::Connectivity;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	NetworkInformation::NetworkStatusChanged += ref new NetworkStatusChangedEventHandler(this, &MainPage::NetworkInformation_NetworkStatusChanged);
	PropertyChanged += ref new PropertyChangedEventHandler(this, &MainPage::InternetChanged);

	Internet = true;
	auto connections = NetworkInformation::GetInternetConnectionProfile();
	Internet = connections != nullptr && connections->GetNetworkConnectivityLevel() == NetworkConnectivityLevel::InternetAccess;

	querySubmitted = false;
	playlists = ref new YoutubePlaylistsCollection;
	nowPlayingPlaylist = ref new YoutubePlaylist(L"");

	auto playlistLoader = ref new PlaylistIO;
	playlistLoader->Read(&playlists);

	SearchPageNavParam^ navParam = ref new SearchPageNavParam(L"", PlayerFrame, playlists, nowPlayingPlaylist);
	SearchFrame->Navigate(TypeName(SearchPage::typeid), navParam);

	PlayerFrame->Navigate(TypeName(PlayerPage::typeid));
}

void MainPage::NetworkInformation_NetworkStatusChanged(Platform::Object^ sender)
{
	auto connections = NetworkInformation::GetInternetConnectionProfile();
	this->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this, connections]()
	{
		Internet = connections != nullptr && connections->GetNetworkConnectivityLevel() == NetworkConnectivityLevel::InternetAccess;
	}));
}

void MainPage::InternetChanged(Platform::Object^ sender, PropertyChangedEventArgs^ e)
{
	if (!Internet)
	{
		auto dialog = ref new Windows::UI::Popups::MessageDialog("Error internet connection. Internet was lost.");
		dialog->ShowAsync();
	}
}

void MainPage::MenuButton_Click(Platform::Object^ sender, RoutedEventArgs^ e)
{
	MenuSplitView->IsPaneOpen = !MenuSplitView->IsPaneOpen;
}

void MainPage::SearchButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MainPage::PlaylistsButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	PlaylistsPageNavParam^ navParam = ref new PlaylistsPageNavParam(playlists, nowPlayingPlaylist, PlayerFrame);
	PlaylistsFrame->Navigate(TypeName(PlaylistsPage::typeid), navParam);

	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MainPage::NowPlayingButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	NowPlayingFrame->Navigate(TypeName(NowPlayingPage::typeid), nowPlayingPlaylist);

	NowPlayingFrame->Visibility = Windows::UI::Xaml::Visibility::Visible;

	SearchFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	PlaylistsFrame->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MainPage::AutoSuggestBox_QuerySubmitted(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs^ args)
{
	if (!sender->Text->IsEmpty())
	{
		Platform::String^ s = sender->Text;
		sender->Text = L"";
		sender->Text = s;

		querySubmitted = true;
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

		auto t = concurrency::create_task(httpClient->GetStringAsync(uri));
		auto continuation = t.then([this, sender](Platform::String^ youtubeSuggestQueriesFile)
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
		}).then([](concurrency::task<void> t)
		{
			try
			{
				t.get();
			}
			catch (Platform::COMException^ e) 
			{

			}
		});
	}
	else
		sender->IsSuggestionListOpen = false;
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