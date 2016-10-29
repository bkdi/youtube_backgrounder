#include "pch.h"
#include "YoutubeExtractor.h"
#include "tools_http.h"
#include "YoutubeSignatureDecrypt.h"

YoutubeExtractor::YoutubeExtractor(Platform::String^ youtubeVideoId) : videoId(youtubeVideoId)
{
}

IAsyncOperation<Platform::String^>^ YoutubeExtractor::getVideoUrlByItagAsync(Platform::String^ itag)
{
	return concurrency::create_async([this, itag]()
	{
		std::wstring itagValue = itag->Data();
		downloadVideoWebpage();
		getVideoConfiguration();

		Url url;
		getUrlByItag(itagValue, url);
		Platform::String^ urlToPlay = ref new Platform::String(url.url.c_str());
		if (url.isEncreptedSignature)
		{
			YouTubeSignatureDecrypt^ youtubeSignatureDecrypt = ref new YouTubeSignatureDecrypt(playerUrl, ref new Platform::String(url.signature.c_str()));
			concurrency::create_task(youtubeSignatureDecrypt->decryptAsync()).then([this, &urlToPlay](Platform::String^ decryptedSignature)
			{
				urlToPlay += L"&signature=" + decryptedSignature;
			}).wait();
		}

		return urlToPlay;
	});
}

void YoutubeExtractor::getVideoConfiguration()
{
	size_t startVideoUrlsIndex = videoWebpageFile.find(L"ytplayer.config = {");
	size_t endVideoUrlsIndex = videoWebpageFile.find(L"};", startVideoUrlsIndex);

	if (startVideoUrlsIndex != std::wstring::npos && endVideoUrlsIndex != std::wstring::npos)
	{
		std::wstringstream playerConfigJsonStream(std::wstring(videoWebpageFile.cbegin() + startVideoUrlsIndex + 18, videoWebpageFile.cbegin() + endVideoUrlsIndex + 1));
		boost::property_tree::wptree pt;
		read_json(playerConfigJsonStream, pt);

		getPlayerUrl(pt);
		getVideosUrls(pt);
	}
}

void YoutubeExtractor::getPlayerUrl(const boost::property_tree::wptree& pt)
{
	auto item = pt.get_child(L"assets");
	playerUrl = "http:" + ref new Platform::String(item.find(L"js")->second.get_value<std::wstring>().c_str());
}

void YoutubeExtractor::getVideosUrls(const boost::property_tree::wptree& pt)
{
	auto item = pt.get_child(L"args");
	getUrls(item.find(L"url_encoded_fmt_stream_map")->second.get_value<std::wstring>());
}

void YoutubeExtractor::getUrls(const std::wstring& urlsSection)
{
	std::vector<std::wstring> urlsSections;
	boost::split(urlsSections, urlsSection, boost::is_any_of(","), boost::token_compress_on);

	for (auto section : urlsSections)
	{
		std::vector<std::wstring> urlSectionItems;
		boost::split(urlSectionItems, section, boost::is_any_of("&"), boost::token_compress_on);
		Url url;
		for (auto s : urlSectionItems)
		{
			if (s.find(L"url=") == 0)
			{
				url.url = s.substr(4, s.length());
				unescape(url.url);
			}
			else if (s.find(L"itag=") == 0)
				url.itag = s.substr(5, s.length());
			else if (s.find(L"s=") == 0)
				url.signature = s.substr(2, s.length());
			else if (s.find(L"sig=") == 0)
				url.signature = s.substr(4, s.length());
		}
		url.isEncreptedSignature = !url.signature.empty();
		videosUrls.push_back(url);
	}
}

void YoutubeExtractor::getUrlByItag(const std::wstring& itag, Url &urlByItag)
{
	for (auto url : videosUrls)
	{
		if (url.itag == itag)
		{
			urlByItag = url;
			break;
		}
	}
}

void YoutubeExtractor::unescape(std::wstring & escaped)
{
	boost::replace_all(escaped, L"%26", L"&");
	boost::replace_all(escaped, L"%3D", L"=");
	boost::replace_all(escaped, L"%3A", L":");
	boost::replace_all(escaped, L"%2F", L"/");
	boost::replace_all(escaped, L"%3F", L"?");
	boost::replace_all(escaped, L"%2C", L",");
	boost::replace_all(escaped, L"%25", L"%");
}

void YoutubeExtractor::downloadVideoWebpage()
{
	Platform::String^ youtubeVideoWebPage;
	ToolsHttp::getStrData(L"https://www.youtube.com/watch?v=" + videoId + L"&gl=US&hl=en&has_verified=1&bpctr=9999999999", youtubeVideoWebPage);
	videoWebpageFile = youtubeVideoWebPage->Data();
}


/*
void YoutubeExtractor::downloadGetVideoInfoFile()
{
	Platform::String^ youtubeGetVideoInfoFile;
	ToolsHttp::getStrData(L"https://www.youtube.com/get_video_info?&video_id=" + videoId + L"&el=info&ps=default&eurl=&gl=US&hl=en", youtubeGetVideoInfoFile);
	getVideoInfoFile = youtubeGetVideoInfoFile->Data();
}

void YoutubeExtractor::getUrlsSection(std::wstring &urlsSection)
{
	size_t startVideoUrlsIndex = getVideoInfoFile.find(START_URLS_STRING);
	size_t endVideoUrlsIndex = getVideoInfoFile.find(L"&", startVideoUrlsIndex);
	if (endVideoUrlsIndex == std::wstring::npos)
		endVideoUrlsIndex = getVideoInfoFile.length() - 1;

	urlsSection.assign(getVideoInfoFile.cbegin() + startVideoUrlsIndex + START_URLS_STRING.length(), getVideoInfoFile.cbegin() + endVideoUrlsIndex);
	unescape(urlsSection);
}*/