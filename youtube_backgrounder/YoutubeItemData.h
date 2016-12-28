#pragma once
#include "pch.h"

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace Windows::UI::Xaml::Data;

namespace youtube_backgrounder
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeItem sealed : public INotifyPropertyChanged
	{
	public:
		YoutubeItem(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr)
		{
			VideoId = VideoIdStr;
			Title = TitleStr;
			SmallThumbnail = SmallThumbnailStr;
			LargeThumbnail = LargeThumbnailStr;
			NowPlaying = false;
		}

		property Platform::String^ VideoId;
		property Platform::String^ Title;
		property Platform::String^ SmallThumbnail;
		property Platform::String^ LargeThumbnail;
		property bool NowPlaying
		{
			bool get() { return nowPlaying;}
			void set(bool value) 
			{
				nowPlaying = value;
				OnPropertyChanged(L"NowPlaying");
			}
		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;

	protected:
		void OnPropertyChanged(Platform::String^ name)
		{
			PropertyChanged(this, ref new PropertyChangedEventArgs(name));
		}

	private:
		bool nowPlaying;
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeItemsCollections sealed
	{
	private:
		Vector<YoutubeItem^>^ vec;

	public:
		YoutubeItemsCollections();
		void AppendItem(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr);
		void AppendItem(YoutubeItem^ item);
		void Clear();

		property IVector<YoutubeItem^>^ YoutubeItems
		{
			IVector<YoutubeItem^>^ get()
			{
				return vec;
			}
		}
	};
}