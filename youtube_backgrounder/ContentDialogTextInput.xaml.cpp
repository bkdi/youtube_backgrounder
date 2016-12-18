//
// ContentDialogTextInput.xaml.cpp
// Implementation of the ContentDialogTextInput class
//

#include "pch.h"
#include "ContentDialogTextInput.xaml.h"

using namespace youtube_backgrounder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Content Dialog item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

DependencyProperty^ youtube_backgrounder::ContentDialogTextInput::TextProperty = DependencyProperty::RegisterAttached("Text", Platform::String::typeid, ContentDialogTextInput::typeid, nullptr);

youtube_backgrounder::ContentDialogTextInput::ContentDialogTextInput(YoutubePlaylistsCollection^ playlists) : Playlists(playlists)
{
	InitializeComponent();
}

void youtube_backgrounder::ContentDialogTextInput::ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{
	if (Playlists->FindByName(Text))
	{
		args->Cancel = true;
		WarningTextBlock->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}
}

void youtube_backgrounder::ContentDialogTextInput::ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args)
{

}

void youtube_backgrounder::ContentDialogTextInput::TextBox_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
{
	auto input = safe_cast<TextBox^> (sender);
	IsPrimaryButtonEnabled = !input->Text->IsEmpty();
	WarningTextBlock->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}
