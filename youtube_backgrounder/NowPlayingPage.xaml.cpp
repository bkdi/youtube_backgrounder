//
// NowPlayingPage.xaml.cpp
// Implementation of the NowPlayingPage class
//

#include "pch.h"
#include "NowPlayingPage.xaml.h"

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
using namespace Windows::Web::Http;
using namespace Windows::UI::Xaml::Interop;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

NowPlayingPage::NowPlayingPage()
{
	InitializeComponent();
}

void NowPlayingPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e)
{
	nowPlayingPlaylist = (safe_cast<YoutubePlaylist^> (e->Parameter));
	DataContext = nowPlayingPlaylist;
	//NowPlayingListView->ItemsSource = nowPlayingPlaylist->Items;
}