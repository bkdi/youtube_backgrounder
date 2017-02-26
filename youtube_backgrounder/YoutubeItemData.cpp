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

	void YoutubeItemsCollections::AppendNonexistent(YoutubeItemsCollections^ collection, unsigned int itemsCount)
	{
		for (auto it = collection->YoutubeItems->First(); it->HasCurrent && itemsCount > 0; it->MoveNext())
		{
			if (std::find(begin(vec), end(vec), it->Current) == end(vec))
			{
				vec->Append(it->Current);
				--itemsCount;
			}
		}
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