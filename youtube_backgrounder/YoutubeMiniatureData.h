#pragma once
#include "pch.h"
#include <collection.h>
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

namespace youtube_backgrounder
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeMiniatureData sealed
	{
	public:
		YoutubeMiniatureData(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr)
		{
			VideoId = VideoIdStr;
			Title = TitleStr;
			SmallThumbnail = SmallThumbnailStr;
			LargeThumbnail = LargeThumbnailStr;
		}

		property Platform::String^ VideoId;
		property Platform::String^ Title;
		property Platform::String^ SmallThumbnail;
		property Platform::String^ LargeThumbnail;
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class DataSource sealed
	{
	private:
		Vector<YoutubeMiniatureData^>^ vec;

	public:
		DataSource() 
		{
			vec = ref new Vector<YoutubeMiniatureData^>();
		}

		void AppendItem(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr)
		{
			vec->Append(ref new YoutubeMiniatureData(VideoIdStr, TitleStr, SmallThumbnailStr, LargeThumbnailStr));
		}

		property IVector<YoutubeMiniatureData^>^ YoutubeMiniatures
		{
			IVector<YoutubeMiniatureData^>^ get()
			{
				return vec;
			}
		}
	};
}