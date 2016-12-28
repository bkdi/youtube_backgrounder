//
// NowPlayingPage.xaml.cpp
// Implementation of the NowPlayingPage class
//

#include "pch.h"
#include "NowPlayingPage.xaml.h"

#include "YoutubePlaylist.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

NowPlayingPage::NowPlayingPage()
{
	InitializeComponent();
}

void NowPlayingPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	NowPlayingListView->ItemsSource = (safe_cast<YoutubePlaylist^> (e->Parameter))->Items;
}

Platform::Object^ NowPlayingBackgroundConverter::Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	if (safe_cast<bool> (value))
		return ref new SolidColorBrush(Windows::UI::Colors::Blue);
	else
		return ref new SolidColorBrush(Windows::UI::Colors::Black);
}

Platform::Object^ NowPlayingBackgroundConverter::ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	throw ref new Platform::NotImplementedException();
}

