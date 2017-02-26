#include "pch.h"
#include "YoutubeVideoDetails.h"
#include "YoutubeAPI_const.h"
#include "tools_http.h"

namespace youtube_backgrounder
{
	YoutubeVideoDetails::YoutubeVideoDetails()
	{
	}

	IAsyncAction^ YoutubeVideoDetails::fillVideosDetails(YoutubeItemsCollections^ youtubeItems)
	{
		return concurrency::create_async([this, youtubeItems]()
		{
			if (youtubeItems->YoutubeItems->Size > 0)
			{
				Platform::String^ url = L"https://www.googleapis.com/youtube/v3/videos?part=statistics" + L"&key=" + YoutubeAPI::YOUTUBE_API_KEY + "&id=";
				for (auto youtubeItem : youtubeItems->YoutubeItems)
					url += youtubeItem->VideoId + ",";

				auto t = ToolsHttp::getDownloadingTask(url);
				t.then([this, youtubeItems](Platform::String^ youtubeVideoStatistic)
				{
					std::wstringstream jsonStream(youtubeVideoStatistic->Data());
					boost::property_tree::wptree pt;

					try
					{
						read_json(jsonStream, pt);
						for (const boost::property_tree::wptree::value_type& item : pt.get_child(L"items"))
						{
							auto videoId = ref new Platform::String(item.second.find(L"id")->second.get_value<std::wstring>().c_str());
							for (auto youtubeItem : youtubeItems->YoutubeItems)
							{
								if (youtubeItem->VideoId == videoId)
								{
									youtubeItem->ViewCount = item.second.find(L"statistics")->second.find(L"viewCount")->second.get_value<unsigned int>();
									break;
								}
							}
						}
					}
					catch (const boost::property_tree::json_parser_error &)
					{
						//TODO:
					}
				}).wait();
			}
		});
	}
}
