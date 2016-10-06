#pragma once

class YoutubeExtractor
{

public:
	YoutubeExtractor(const std::wstring& youtubeGetVideoInfoFile);

	void getVideosUrls();
	void getVideoUrlByItag(const std::wstring& itag, std::wstring &urlToPlay);

private:
	static const std::wstring START_URL_STRING;

	const std::wstring getVideoInfoFile;

	std::vector<std::wstring> rawVideosUrls;

	void getUrlsSection(std::wstring &urlsSection);
	void getRawUrls(std::wstring &urlsSection);
	void prepareUrl(std::wstring & rawUrltoPlay);
};

