#include "pch.h"
#include "YoutubeSearch.h"
#include "YoutubeAPI_const.h"
#include "tools_http.h"

namespace youtube_backgrounder
{
	Platform::String^ convertOrderToStr(YoutubeSearchedResultsOrder order)
	{
		switch (order)
		{
		case YoutubeSearchedResultsOrder::Relevance:
			return L"relevance";
		case YoutubeSearchedResultsOrder::ViewCount:
			return L"viewCount";
		case YoutubeSearchedResultsOrder::Rating:
			return L"rating";
		case YoutubeSearchedResultsOrder::Date:
			return L"date";
		default: return L"";
		}
	}

	Platform::String^ convertOrderToDescription(YoutubeSearchedResultsOrder order)
	{
		switch (order)
		{
		case YoutubeSearchedResultsOrder::Relevance:
			return L"Relevance";
		case YoutubeSearchedResultsOrder::ViewCount:
			return L"View count";
		case YoutubeSearchedResultsOrder::Rating:
			return L"Rating";
		case YoutubeSearchedResultsOrder::Date:
			return L"Date";
		default: return L"";
		}
	}
	YoutubeSearchedResultsOrder convertDescriptionToOrder(Platform::String^ description)
	{
		if (description == L"Relevance")
			return YoutubeSearchedResultsOrder::Relevance;
		else if (description == L"View count")
			return YoutubeSearchedResultsOrder::ViewCount;
		else if (description == L"Rating")
			return YoutubeSearchedResultsOrder::Rating;
		else if (description == L"Date")
			return YoutubeSearchedResultsOrder::Date;
		else
			return YoutubeSearchedResultsOrder::Relevance;
	}

	YoutubeSearch::YoutubeSearch() : searchingMode(SearchingMode::Null), nextPageToken(L"")
	{

	}

	IAsyncOperation<YoutubeItemsCollections^>^ YoutubeSearch::search(Platform::String^ title_, YoutubeSearchedResultsOrder order_, unsigned int resultsCount)
	{
		title = title_;
		order = order_;
		searchingMode = SearchingMode::Search;
		
		return getResults(resultsCount);
	}

	IAsyncOperation<YoutubeItemsCollections^>^ YoutubeSearch::searchMore(unsigned int resultsCount)
	{
		searchingMode = SearchingMode::SearchNextPage;
		
		return getResults(resultsCount);
	}

	IAsyncOperation<YoutubeItemsCollections^>^ YoutubeSearch::searchRelated(Platform::String^ videoId_, unsigned int resultsCount)
	{
		videoId = videoId_;
		searchingMode = SearchingMode::SearchRelated;
		return getResults(resultsCount);
	}

	IAsyncOperation<YoutubeItemsCollections^>^ YoutubeSearch::getResults(unsigned int resultsCount)
	{
		return concurrency::create_async([this, resultsCount]()
		{
			void validateParams();

			auto searchedResults = ref new YoutubeItemsCollections;

			if (searchingMode != SearchingMode::SearchNextPage || (searchingMode == SearchingMode::SearchNextPage && !nextPageToken->IsEmpty()))
			{
				auto url = prepareReq(resultsCount);

				auto t = ToolsHttp::getDownloadingTask(url);
				t.then([this, &searchedResults](Platform::String^ youtubeSearchedResults)
				{
					searchedResults = parseJSON(youtubeSearchedResults, searchedResults);
				}).then([](concurrency::task<void> t)
				{
					try
					{
						t.get();
					}
					catch (Platform::COMException^ e)
					{
						throw e;
					}
				}).wait();
			}
			return searchedResults;
		});
	}


	Platform::String^ YoutubeSearch::prepareReq(unsigned int resultsCount)
	{
		Platform::String^ url = L"https://www.googleapis.com/youtube/v3/search?part=snippet&type=video&maxResults=" + resultsCount.ToString();
		
		if (searchingMode == SearchingMode::Search || searchingMode == SearchingMode::SearchNextPage)
			url += L"&q=" + title + L"&order=" + convertOrderToStr(order);
		
		if (searchingMode == SearchingMode::SearchNextPage && !nextPageToken->IsEmpty())
			url += L"&pageToken=" + nextPageToken;

		if (searchingMode == SearchingMode::SearchRelated)
			url += L"&relatedToVideoId=" + videoId;

		url += L"&key=" + YoutubeAPI::YOUTUBE_API_KEY;

		return url;
	}

	YoutubeItemsCollections^ YoutubeSearch::parseJSON(Platform::String^ searchedResultJSON, YoutubeItemsCollections^& searchedResults)
	{
		std::wstringstream jsonStream(searchedResultJSON->Data());
		boost::property_tree::wptree pt;

		try
		{
			read_json(jsonStream, pt);

			auto it = pt.find(L"nextPageToken");
			if (it != pt.not_found())
				nextPageToken = ref new Platform::String(it->second.get_value<std::wstring>().c_str());
			else
				nextPageToken = L"";

			for (const boost::property_tree::wptree::value_type& item : pt.get_child(L"items"))
			{
				auto videoId = ref new Platform::String(item.second.find(L"id")->second.find(L"videoId")->second.get_value<std::wstring>().c_str());
				auto title = ref new Platform::String(item.second.find(L"snippet")->second.find(L"title")->second.get_value<std::wstring>().c_str());
				auto smallThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"default")->second.find(L"url")->second.get_value<std::wstring>().c_str());
				auto largeThumbnail = ref new Platform::String(item.second.find(L"snippet")->second.find(L"thumbnails")->second.find(L"high")->second.find(L"url")->second.get_value<std::wstring>().c_str());

				searchedResults->AppendItem(videoId, title, smallThumbnail, largeThumbnail);
			}
			return searchedResults;
		}
		catch (const boost::property_tree::json_parser_error &)
		{
			//TODO:
		}

		return nullptr;
	}

	void YoutubeSearch::validateParams()
	{
		switch (searchingMode)
		{
		case SearchingMode::Search:
		{
			if (title->IsEmpty())
				throw ref new Platform::InvalidArgumentException(L"Title for searching is empty.");

			break;
		}
		case SearchingMode::SearchNextPage:
			break;
		case SearchingMode::SearchRelated:
			break;
		default: 
			throw ref new Platform::InvalidArgumentException(L"Unknown searching mode.");
		}
	}
}