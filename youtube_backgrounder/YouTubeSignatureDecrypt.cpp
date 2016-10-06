#include "pch.h"
#include "YouTubeSignatureDecrypt.h"


YouTubeSignatureDecrypt::YouTubeSignatureDecrypt(const std::wstring& youtubeEmbedVideoFile) : embedVideoFile(youtubeEmbedVideoFile)
{

}


YouTubeSignatureDecrypt::~YouTubeSignatureDecrypt()
{
}

void YouTubeSignatureDecrypt::decrypt(std::wstring & urlWithSignature)
{
	std::wstring signature;
	bool isEncreptedSignature = checkAndGetSignatureFromUrl(urlWithSignature, signature);

	if (isEncreptedSignature)
	{

	}

	urlWithSignature += L"&signature=" + signature;
}

bool YouTubeSignatureDecrypt::checkAndGetSignatureFromUrl(std::wstring & urlWithSignature, std::wstring &signature)
{
	bool isEncreptedSignature = false;
	std::wsmatch matchedSignature;
	std::wregex regex(L"((s)|(sig)|(signature))=([[:alnum:]]+\\.[[:alnum:]]+)"); 
	if (std::regex_search(urlWithSignature, matchedSignature, regex))
	{
		std::wstring signaturePropName = matchedSignature[1];
		if (signaturePropName == L"s" || signaturePropName == L"sig")
			isEncreptedSignature = true;

		signature = matchedSignature[5];
		urlWithSignature.erase(matchedSignature.position(), matchedSignature[0].length() + 1);
	}

	return isEncreptedSignature;
}