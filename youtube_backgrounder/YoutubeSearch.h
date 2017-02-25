#pragma once

#include "YoutubeItemData.h"

using namespace youtube_backgrounder;
using namespace Windows::Foundation;

namespace youtube_backgrounder
{
	public enum class YoutubeSearchedResultsOrder
	{
		Relevance,
		ViewCount,
		Rating,
		Date
	};

	Platform::String^ convertOrderToStr(YoutubeSearchedResultsOrder order);
	Platform::String^ convertOrderToDescription(YoutubeSearchedResultsOrder order);
	YoutubeSearchedResultsOrder convertDescriptionToOrder(Platform::String^ description);

	ref class YoutubeSearch sealed
	{
	public:
		YoutubeSearch();

		IAsyncOperation<YoutubeItemsCollections^>^ search(Platform::String^ title_, YoutubeSearchedResultsOrder order_, unsigned int resultsCount);
		IAsyncOperation<YoutubeItemsCollections^>^ searchMore(unsigned int resultsCount);

	private:
		Platform::String^ title;
		YoutubeSearchedResultsOrder order;
		Platform::String^ nextPageToken;
		bool useNextPageToken;

		IAsyncOperation<YoutubeItemsCollections^>^ getResults(unsigned int resultsCount);
		Platform::String^ prepareReq(unsigned int resultsCount);
		YoutubeItemsCollections^ parseJSON(Platform::String^ searchedResultJSON);
	};
}

