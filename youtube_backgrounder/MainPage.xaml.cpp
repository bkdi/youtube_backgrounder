//
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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

void MainPage::SearchSugesstion(SearchBox^ sender, SearchBoxQuerySubmittedEventArgs^ args)
{
	if (sender->QueryText != "")
	{
		SearchPageNavParam^ navParam = ref new SearchPageNavParam(sender->QueryText, PlayerFrame);
		SearchFrame->Navigate(TypeName(SearchPage::typeid), navParam);
	}
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
