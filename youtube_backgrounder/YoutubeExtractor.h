#pragma once
#include "YoutubeQuality.h"

using namespace youtube_backgrounder;

using namespace Windows::Foundation;

ref class YoutubeExtractor sealed
{

public:
	YoutubeExtractor(Platform::String^ youtubeVideoId);
	IAsyncOperation<Platform::String^>^ getVideoUrlByItagAsync(YoutubeQualityItag itag);

private:
	struct Url
	{
		bool isEncreptedSignature;
		std::wstring url;
		std::wstring signature;
		std::wstring itag;

		Url() : isEncreptedSignature(false) {}
	};
	
	Platform::String^ videoId;
	Platform::String^ playerUrl;

	std::wstring getVideoInfoFile;
	std::wstring videoWebpageFile;

	std::vector<Url> videosUrls;

	void downloadVideoWebpage();
	void getVideoConfiguration();
	void getPlayerUrl(const boost::property_tree::wptree& pt);
	void getVideosUrls(const boost::property_tree::wptree& pt);
	void getUrls(const std::wstring& urlsSection);
	void getUrlByItag(const std::wstring& itag, Url &urlByItag);
	void unescape(std::wstring & escaped);
};

