//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "SearchPage.xaml.h"
#include "PlayerPage.xaml.h"

namespace youtube_backgrounder
{
	public ref class NavigationProperties sealed
	{
	public:
		property MainPage^ page;
		property Platform::String^ param;
	};

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void SetPage(Windows::UI::Xaml::Interop::TypeName type, Platform::String^ s);

	protected:
		void SearchSugesstion(Windows::UI::Xaml::Controls::SearchBox^ sender, Windows::UI::Xaml::Controls::SearchBoxQuerySubmittedEventArgs^ args);

	private:
		void MenuButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SearchButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void PlayerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
