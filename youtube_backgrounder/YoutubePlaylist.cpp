#include "pch.h"
#include "YoutubePlaylist.h"

namespace youtube_backgrounder
{
	YoutubePlaylist::YoutubePlaylist()
	{
		itemsCollection = ref new YoutubeItemsCollections;
	}

	YoutubePlaylist::YoutubePlaylist(Platform::String^ playlistName)
	{
		itemsCollection = ref new YoutubeItemsCollections;
		Name = playlistName;
	}

	void YoutubePlaylist::add(YoutubeItem^ item)
	{
		itemsCollection->AppendItem(item);
		OnPropertyChanged("TracksCount");
	}

	void YoutubePlaylist::clear()
	{
		itemsCollection->Clear();
		NowPlayingIndex = -1;
		OnPropertyChanged("TracksCount");
	}

	void YoutubePlaylist::reset()
	{
		PropertyChanged -= eventToken;
		clear();
		Name = L"";
	}

	void YoutubePlaylist::deleteItem(YoutubeItem^ item)
	{
		if (item != nullptr)
		{
			auto vec = getItems();

			unsigned int index = 0;
			if (vec->IndexOf(item, &index))
			{
				vec->RemoveAt(index);
				OnPropertyChanged("TracksCount");
			}
		}
	}

	IVector<YoutubeItem^>^ YoutubePlaylist::getItems()
	{
		return itemsCollection->YoutubeItems;
	}

	YoutubePlaylistsCollection::YoutubePlaylistsCollection()
	{
		vec = ref new Vector<YoutubePlaylist^>;
	}

	void YoutubePlaylistsCollection::AppendPlaylist(YoutubePlaylist^ item)
	{
		if (item != nullptr)
			vec->Append(item);
	}

	bool YoutubePlaylistsCollection::FindByName(Platform::String^ name)
	{
		bool bFound = false;

		for (auto item : vec)
		{
			if (item->Name == name)
			{
				bFound = true;
				break;
			}
		}

		return bFound;
	}

	void YoutubePlaylistsCollection::DeletePlaylist(YoutubePlaylist^ item)
	{
		if (item != nullptr)
		{
			unsigned int index = 0;
			if (vec->IndexOf(item, &index))
				vec->RemoveAt(index);
		}
	}
}
