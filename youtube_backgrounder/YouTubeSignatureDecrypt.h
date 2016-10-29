#pragma once

using namespace Windows::Foundation;

ref class YouTubeSignatureDecrypt sealed
{
public:
	//YouTubeSignatureDecrypt(Platform::String^ youtubeVideoId, Platform::String^ youtubeEncryptedSignature);
	YouTubeSignatureDecrypt(Platform::String^ youtubePlayerUrl, Platform::String^ youtubeEncryptedSignature);

	IAsyncOperation<Platform::String^>^ decryptAsync();

private:
	Platform::String^ videoId;
	Platform::String^ encryptedSignature;
	Platform::String^ playerUrl;

	std::wstring embedFile;
	std::wstring playerFile;

	/*void downloadEmbedFile();
	void getPlayerUrlFromEmbedFile(std::wstring &urlToPlayer);*/
	void downloadPlayerFile();
};

