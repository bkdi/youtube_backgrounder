﻿//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BINDING_DEBUG_OUTPUT
extern "C" __declspec(dllimport) int __stdcall IsDebuggerPresent();
#endif

#include "MediaPlayer.xaml.h"

void ::youtube_backgrounder::MediaPlayer::InitializeComponent()
{
    if (_contentLoaded)
    {
        return;
    }
    _contentLoaded = true;
    ::Windows::Foundation::Uri^ resourceLocator = ref new ::Windows::Foundation::Uri(L"ms-appx:///MediaPlayer.xaml");
    ::Windows::UI::Xaml::Application::LoadComponent(this, resourceLocator, ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);
}

void ::youtube_backgrounder::MediaPlayer::Connect(int __connectionId, ::Platform::Object^ __target)
{
    switch (__connectionId)
    {
        case 1:
            {
                this->musicPlayer = safe_cast<::Windows::UI::Xaml::Controls::MediaElement^>(__target);
                (safe_cast<::Windows::UI::Xaml::Controls::MediaElement^>(this->musicPlayer))->CurrentStateChanged += ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::youtube_backgrounder::MediaPlayer::*)
                    (::Platform::Object^, ::Windows::UI::Xaml::RoutedEventArgs^))&MediaPlayer::MusicPlayer_CurrentStateChanged);
            }
            break;
    }
    _contentLoaded = true;
}

::Windows::UI::Xaml::Markup::IComponentConnector^ ::youtube_backgrounder::MediaPlayer::GetBindingConnector(int __connectionId, ::Platform::Object^ __target)
{
    __connectionId;         // unreferenced
    __target;               // unreferenced
    return nullptr;
}


