// 
// PlaylistListViewItemControl.xaml.cpp
// Implementation of the PlaylistListViewItemControl class
//

#include "pch.h"
#include "PlaylistListViewItemControl.xaml.h"

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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

typedef unsigned int uint;

DependencyProperty^ PlaylistListViewItemControl::_PlaylistNameProperty = DependencyProperty::Register(L"PlaylistName", Platform::String::typeid, PlaylistListViewItemControl::typeid, ref new PropertyMetadata(""));
DependencyProperty^ PlaylistListViewItemControl::_TracksCountProperty = DependencyProperty::Register(L"TracksCount", uint::typeid, PlaylistListViewItemControl::typeid, ref new PropertyMetadata(0));

PlaylistListViewItemControl::PlaylistListViewItemControl()
{
	InitializeComponent();
}

void PlaylistListViewItemControl::PlaylistPlayButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	PlayButtonClick(this, e);
}


void youtube_backgrounder::PlaylistListViewItemControl::PlaylistListViewItem_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (TracksCount > 0)
		PlaylistPlayButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void youtube_backgrounder::PlaylistListViewItemControl::PlaylistListViewItem_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	PlaylistPlayButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}
