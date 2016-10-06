#include "pch.h"
#include "YoutubeExtractor.h"


const std::wstring YoutubeExtractor::START_URL_STRING = L"url%3D";

YoutubeExtractor::YoutubeExtractor(const std::wstring& youtubeGetVideoInfoFile) : getVideoInfoFile(youtubeGetVideoInfoFile)
{
}

void YoutubeExtractor::getVideosUrls()
{
	std::wstring urlsSection;
	getUrlsSection(urlsSection);
	getRawUrls(urlsSection);
}

void YoutubeExtractor::getUrlsSection(std::wstring &urlsSection)
{
	std::vector<std::wstring> rawUrls;
	size_t startVideoUrlsIndex = getVideoInfoFile.find(L"url_encoded_fmt_stream_map");
	size_t endVideoUrlsIndex = getVideoInfoFile.find(L"&", startVideoUrlsIndex);
	if (endVideoUrlsIndex == std::wstring::npos)
		endVideoUrlsIndex = getVideoInfoFile.length() - 1;

	urlsSection.assign(getVideoInfoFile.cbegin() + startVideoUrlsIndex, getVideoInfoFile.cbegin() + endVideoUrlsIndex);
}

void YoutubeExtractor::getRawUrls(std::wstring &urlsSection)
{
	size_t startUrlIndex = 0;
	do
	{
		startUrlIndex = urlsSection.find(START_URL_STRING, startUrlIndex);

		if (startUrlIndex != std::wstring::npos)
			startUrlIndex += START_URL_STRING.length();
		else
			break;

		size_t endUrlIndex = urlsSection.find(L"%26", startUrlIndex);
		if (endUrlIndex == std::wstring::npos)
			endUrlIndex = urlsSection.length() - 1;

		rawVideosUrls.push_back(std::wstring(urlsSection.cbegin() + startUrlIndex, urlsSection.cbegin() + endUrlIndex));
		
		if(rawVideosUrls.rbegin()->find(L"%2C") != std::wstring::npos)
			rawVideosUrls.rbegin()->erase(rawVideosUrls.rbegin()->find(L"%2C"), rawVideosUrls.rbegin()->length());

	} while (startUrlIndex != std::wstring::npos);
}

void YoutubeExtractor::getVideoUrlByItag(const std::wstring& itag, std::wstring &urlToPlay)
{
	std::wstring itagValue = L"itag%253D" + itag;

	for (auto s : rawVideosUrls)
	{
		if (s.find(itagValue) != std::wstring::npos)
		{
			urlToPlay = s;
			prepareUrl(urlToPlay);
		}
	}
}

void YoutubeExtractor::prepareUrl(std::wstring & urltoPlay)
{
	boost::replace_all(urltoPlay, L"%2526", L"&");
	boost::replace_all(urltoPlay, L"%253D", L"=");
	boost::replace_all(urltoPlay, L"%253A", L":");
	boost::replace_all(urltoPlay, L"%252F", L"/");
	boost::replace_all(urltoPlay, L"%2525", L"%");
	boost::replace_all(urltoPlay, L"%253F", L"?");
}

