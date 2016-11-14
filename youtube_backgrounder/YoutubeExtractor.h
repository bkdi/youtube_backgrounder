#pragma once
#include "YoutubeQuality.h"

using namespace youtube_backgrounder;

using namespace Windows::Foundation;

ref class YoutubeExtractor sealed
{

public:
	YoutubeExtractor(Platform::String^ youtubeVideoId, YoutubeQualityItag youtubePreferedQuality);
	IAsyncOperation<Platform::String^>^ getVideoUrlByItagAsync();

private:
	struct Url
	{
		bool isEncreptedSignature;
		std::wstring url;
		std::wstring signature;
		std::wstring itag;

		Url() : isEncreptedSignature(false) {}
	};
	
	YoutubeQualityItag preferedQuality;
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
	void getUrlByItag(Url &urlByItag);
	void unescape(std::wstring & escaped);
};

