#pragma once
#include "pch.h"

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace Windows::UI::Xaml::Data;

namespace youtube_backgrounder
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeItem sealed
	{
	public:
		YoutubeItem(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr)
		{
			VideoId = VideoIdStr;
			Title = TitleStr;
			SmallThumbnail = SmallThumbnailStr;
			LargeThumbnail = LargeThumbnailStr;
			ViewCount = 0;
		} 

		property Platform::String^ VideoId;
		property Platform::String^ Title;
		property Platform::String^ SmallThumbnail;
		property Platform::String^ LargeThumbnail;
		property unsigned int ViewCount;

	internal:
		bool operator <(YoutubeItem^ item)
		{
			return (item->ViewCount < ViewCount);
		}

		bool operator ==(YoutubeItem^ item)
		{
			return (item->VideoId == VideoId);
		}

		bool operator ==(Platform::String^ videoId)
		{
			return (VideoId == videoId);
		}
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeItemsCollections sealed
	{
	private:
		Vector<YoutubeItem^>^ vec;

	public:
		YoutubeItemsCollections();
		void Append(YoutubeItemsCollections^ collection);
		void AppendNonexistent(YoutubeItemsCollections^ collection, unsigned int itemsCount);
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