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

DependencyProperty^ PlaylistListViewItemControl::_PlaylistNameProperty = DependencyProperty::Register(L"PlaylistName", Platform::String::typeid, PlaylistListViewItemControl::typeid, ref new PropertyMetadata(""));
DependencyProperty^ PlaylistListViewItemControl::_TracksCountProperty = DependencyProperty::Register(L"TracksCount", unsigned int::typeid, PlaylistListViewItemControl::typeid, nullptr);

PlaylistListViewItemControl::PlaylistListViewItemControl()
{
	InitializeComponent();
}

Platform::Object^ PlaylistItemsCountToStrConverter::Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	auto count = safe_cast<unsigned int> (value);

	Platform::String^ tracks = ref new Platform::String(L"Tracks: ");
	tracks += value->ToString();

	return tracks;
}

Platform::Object^ PlaylistItemsCountToStrConverter::ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	throw ref new Platform::NotImplementedException();
}

void PlaylistListViewItemControl::PlaylistPlayButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	PlayButtonClick(this, e);
}
