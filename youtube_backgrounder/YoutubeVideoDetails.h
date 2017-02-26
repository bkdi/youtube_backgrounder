#pragma once

#include "YoutubeItemData.h"

using namespace youtube_backgrounder;
using namespace Windows::Foundation;

namespace youtube_backgrounder
{
	ref class YoutubeVideoDetails sealed
	{
	public:
		YoutubeVideoDetails();

		IAsyncAction^ fillVideosDetails(YoutubeItemsCollections^ youtubeItems);
	};
}

