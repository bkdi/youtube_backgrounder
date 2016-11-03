#pragma once

#include "YoutubeItemData.h"

using namespace youtube_backgrounder;
using namespace Windows::Foundation;

ref class YoutubeSearch sealed
{
public:
	YoutubeSearch(Platform::String^ title);

	void getItemsCollection(YoutubeItemsCollections^* items);
	IAsyncAction^ getItemsAsync();

private:
	Platform::String^ searchedTitle;
	Platform::String^ nextPageToken;

	YoutubeItemsCollections^ youtubeItems;

	void downloadItemsJsonStyleFile(std::wstringstream& jsonItemsStream);
	void getItemsFromJsonFile(std::wstringstream& jsonItemsStream);
};

