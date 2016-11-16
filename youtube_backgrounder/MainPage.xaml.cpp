﻿//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "SearchPage.xaml.h"
#include "SettingsPage.xaml.h"


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
}

void MainPage::MenuButton_Click(Platform::Object^ sender, RoutedEventArgs^ e)
{
	MenuSplitView->IsPaneOpen = !MenuSplitView->IsPaneOpen;

	(safe_cast<RadioButton^> (sender))->IsChecked = false;
}

void MainPage::SearchButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	(safe_cast<RadioButton^> (sender))->IsChecked = true;
}


void MainPage::PlayerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	(safe_cast<RadioButton^> (sender))->IsChecked = true;
}


void youtube_backgrounder::MainPage::AutoSuggestBox_QuerySubmitted(Windows::UI::Xaml::Controls::AutoSuggestBox^ sender, Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs^ args)
{
	if (!sender->Text->IsEmpty())
	{
		SearchPageNavParam^ navParam = ref new SearchPageNavParam(sender->Text, PlayerFrame);
		SearchFrame->Navigate(TypeName(SearchPage::typeid), navParam);
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
