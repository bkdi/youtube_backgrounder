﻿//
// SearchPage.xaml.h
// Declaration of the SearchPage class
//

#pragma once

#include "SearchPage.g.h"
#include "MainPage.xaml.h"
#include "YoutubeMiniatureData.h"

using namespace Windows::UI::Xaml;
using namespace Windows::Media;

namespace youtube_backgrounder
{

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SearchPage sealed
	{
	public:
		SearchPage();

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:	
		void gridresult_ItemClick(Platform::Object^ sender, Windows::UI::Xaml::Controls::ItemClickEventArgs^ e);
		void ItemsWrapGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);

		SystemMediaTransportControls^ systemControls;
		void InitializeTransportControls();
		void SystemControls_ButtonPressed(SystemMediaTransportControls^ sender, SystemMediaTransportControlsButtonPressedEventArgs^ args);
		void PlayMedia();
		void PauseMedia();
		void musicPlayer_CurrentStateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};

	public ref class ItemWidthStateConverter sealed : Windows::UI::Xaml::Data::IValueConverter
	{
	public:

		virtual Platform::Object^ Convert(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);

		virtual Platform::Object^ ConvertBack(Platform::Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Platform::Object^ parameter, Platform::String^ language);
	};

	public ref class SizeChange sealed : DependencyObject
	{
	public:

		static bool GetIsEnabled(DependencyObject^ obj)
		{
			return (bool)obj->GetValue(IsEnabledProperty);
		}

		static void SetIsEnabled(DependencyObject^ obj, bool value)
		{
			obj->SetValue(IsEnabledProperty, value);
		}

		static double GetActualWidth(DependencyObject^ obj)
		{
			return (double)obj->GetValue(ActualWidthProperty);
		}


		static void SetActualWidth(DependencyObject^ obj, double value)
		{
			obj->SetValue(ActualWidthProperty, value);
		}

	private:
		static DependencyProperty^ IsEnabledProperty;
		static DependencyProperty^ ActualWidthProperty;

		static void OnIsEnabledChanged(DependencyObject^ obj, DependencyPropertyChangedEventArgs^ args)
		{
			auto element = (FrameworkElement^)obj;

			if (element == nullptr)
				return;

			if (args->NewValue != nullptr && (bool)args->NewValue == true)
				Attach(element);
			else
				Detach(element);
		}

		static Windows::Foundation::EventRegistrationToken eventToken;
		static void Attach(FrameworkElement^ element)
		{
			eventToken = element->SizeChanged += ref new SizeChangedEventHandler(&SizeChange::HandleSizeChanged);
		}

		static void Detach(FrameworkElement^ element)
		{
			element->SizeChanged -= eventToken;
		}

		static void HandleSizeChanged(Platform::Object^ sender, SizeChangedEventArgs^ e)
		{
			auto element = (FrameworkElement^)sender;

			if (element == nullptr)
				return;

			auto width = e->NewSize.Width;
			auto width2 = element->Width;

			SetActualWidth(element, width);
		}
	};

}
