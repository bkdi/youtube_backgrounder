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
		IAsyncOperation<YoutubeItemsCollections^>^ searchRelated(Platform::String^ videoId, unsigned int resultsCount);

	private:
		enum class SearchingMode
		{
			Null,
			Search,
			SearchNextPage,
			SearchRelated
		};

		Platform::String^ title;
		Platform::String^ videoId;
		YoutubeSearchedResultsOrder order;
		Platform::String^ nextPageToken;

		SearchingMode searchingMode;

		IAsyncOperation<YoutubeItemsCollections^>^ getResults(unsigned int resultsCount);
		Platform::String^ prepareReq(unsigned int resultsCount);
		YoutubeItemsCollections^ parseJSON(Platform::String^ searchedResultJSON, YoutubeItemsCollections^& searchedResults);
		void validateParams();
	};
}

