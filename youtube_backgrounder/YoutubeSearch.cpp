#include "pch.h"
#include "YoutubeSearch.h"
#include "tools_http.h"


YoutubeSearch::YoutubeSearch(Platform::String^ title) : searchedTitle(title)
{
	youtubeItems = ref new YoutubeItemsCollections;
}

void YoutubeSearch::getItemsCollection(YoutubeItemsCollections^* items)
{
	items = &youtubeItems;
}

IAsyncAction^ YoutubeSearch::getItemsAsync()
{
	return concurrency::create_async([this]()
	{
		std::wstringstream jsonItemsStream;
		downloadItemsJsonStyleFile(jsonItemsStream);
		getItemsFromJsonFile(jsonItemsStream);

		//return youtubeItems;
	});
}

void YoutubeSearch::downloadItemsJsonStyleFile(std::wstringstream& jsonItemsStream)
{
	Platform::String^ youtubeItemsJsonFile;
	Platform::String^ url = L"https://www.googleapis.com/youtube/v3/search?part=snippet&maxResults=16&q=" + searchedTitle + L"&type=video&key=AIzaSyCIM4EzNqi1in22f4Z3Ru3iYvLaY8tc3bo";
	if (!nextPageToken->IsEmpty())
		url += L"&pageToken=" + nextPageToken;

	ToolsHttp::getStrData(url, youtubeItemsJsonFile);
	jsonItemsStream << youtubeItemsJsonFile->Data();
}

void YoutubeSearch::getItemsFromJsonFile(std::wstringstream& jsonItemsStream)
{
	boost::property_tree::wptree pt;

	try
	{
		read_json(jsonItemsStream, pt);

		nextPageToken = ref new Platform::String(pt.find(L"nextPageToken")->second.get_value<std::wstring>().c_str());

		auto itemsArray = pt.get_child(L"items");
		for (const boost::property_tree::wptree::value_type& item : pt.get_child(L"items"))
		{
			Platform::String^ videoId = ref new Platform::String(item.second.find(L"id")->second.find(L"videoId")->second.get_value<std::wstring>().c_str());
			Platform::String^ title = ref new Platform::String(item.second.find(L"snippet")->second.find(L"title")->second.get_value<std::wstring>().c_str());
			Platform::String^ smallThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"default")->second.find(L"url")->second.get_value<std::wstring>().c_str());
			Platform::String^ largeThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"high")->second.find(L"url")->second.get_value<std::wstring>().c_str());

			youtubeItems->AppendItem(videoId, title, smallThumbnail, largeThumbnail);
		}
	}
	catch (const boost::property_tree::json_parser_error &)
	{
		//TODO:
	}
}