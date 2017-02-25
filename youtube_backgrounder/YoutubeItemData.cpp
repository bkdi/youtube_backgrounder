#include "pch.h"

#include "YoutubeItemData.h"

namespace youtube_backgrounder
{
	YoutubeItemsCollections::YoutubeItemsCollections()
	{
		vec = ref new Vector<YoutubeItem^>();
	}

	void YoutubeItemsCollections::Append(YoutubeItemsCollections^ collection)
	{
		for (auto item : collection->YoutubeItems)
			vec->Append(item);
	}

	void YoutubeItemsCollections::AppendItem(Platform::String^ VideoIdStr, Platform::String^ TitleStr, Platform::String^ SmallThumbnailStr, Platform::String^ LargeThumbnailStr)
	{
		vec->Append(ref new YoutubeItem(VideoIdStr, TitleStr, SmallThumbnailStr, LargeThumbnailStr));
	}

	void YoutubeItemsCollections::AppendItem(YoutubeItem^ item)
	{
		vec->Append(item);
	}

	void YoutubeItemsCollections::Clear()
	{
		vec->Clear();
	}
}