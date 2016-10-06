#pragma once
class YouTubeSignatureDecrypt
{
public:
	YouTubeSignatureDecrypt(const std::wstring& youtubeEmbedVideoFile);
	~YouTubeSignatureDecrypt();

	void decrypt(std::wstring & urlWithSignature);

private:
	const std::wstring embedVideoFile;

	bool checkAndGetSignatureFromUrl(std::wstring & urlWithSignature, std::wstring &signature);
};

