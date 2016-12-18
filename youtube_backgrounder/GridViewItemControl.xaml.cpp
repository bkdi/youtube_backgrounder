//
// GridViewItemControl.xaml.cpp
// Implementation of the GridViewItemControl class
//

#include "pch.h"
#include "GridViewItemControl.xaml.h"
#include "ContentDialogAddToPlaylist.xaml.h"

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

DependencyProperty^ GridViewItemControl::_ItemTitleProperty = DependencyProperty::Register("ItemTitle", Platform::String::typeid, GridViewItemControl::typeid, ref new PropertyMetadata(""));
DependencyProperty^ GridViewItemControl::_ThumbnailSourceProperty = DependencyProperty::Register("ThumbnailSource", Platform::String::typeid, GridViewItemControl::typeid, ref new PropertyMetadata(""));
DependencyProperty^ GridViewItemControl::_PlaylistsProperty = DependencyProperty::Register("Playlists", YoutubePlaylistsCollection::typeid, GridViewItemControl::typeid, nullptr);

GridViewItemControl::GridViewItemControl()
{
	InitializeComponent();
}


void GridViewItemControl::ItemArea_PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if(Playlists != nullptr && Playlists->PlaylistItems->Size > 0)
		AddToPlaylistButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
}


void GridViewItemControl::ItemArea_PointerExited(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	AddToPlaylistButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void GridViewItemControl::AddToPlaylistButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto element = safe_cast<FrameworkElement^> (this);
	auto item = (safe_cast<YoutubeItem^> (element->DataContext));

	auto dialogAddToPlaylist = ref new ContentDialogAddToPlaylist(Playlists, item);
	concurrency::create_task(dialogAddToPlaylist->ShowAsync());
}

Platform::Object^ ItemWidthToFontSizeConverter::Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	double fontSize = 1.0;
	if (value != nullptr)
	{
		double width = safe_cast<double> (value);
		if (width >= 17.0)
			fontSize = width / 17.0; // przy takim stosunku w obszarze textboxa mieszczą się 3 wiersze
	}
	return fontSize;
}

Platform::Object^ ItemWidthToFontSizeConverter::ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language)
{
	throw ref new Platform::NotImplementedException();
}
