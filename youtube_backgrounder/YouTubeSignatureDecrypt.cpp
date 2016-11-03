#include "pch.h"
#include "YouTubeSignatureDecrypt.h"
#include "tools_http.h"
#include "v7.h"

using namespace Windows::UI::Xaml::Controls;

/*YouTubeSignatureDecrypt::YouTubeSignatureDecrypt(Platform::String^ youtubeVideoId, Platform::String^ youtubeEncryptedSignature) 
	: videoId(youtubeVideoId), encryptedSignature(youtubeEncryptedSignature)
{

}*/

YouTubeSignatureDecrypt::YouTubeSignatureDecrypt(Platform::String^ youtubePlayerUrl, Platform::String^ youtubeEncryptedSignature)
	: playerUrl(youtubePlayerUrl), encryptedSignature(youtubeEncryptedSignature)
{

}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

IAsyncOperation<Platform::String^>^ YouTubeSignatureDecrypt::decryptAsync()
{
	return concurrency::create_async([this]()
	{
		/*downloadEmbedFile();
		std::wstring urlToPlayer;
		getPlayerUrlFromEmbedFile(urlToPlayer);*/
		downloadPlayerFile();

		//szukanie nazwy funkcji dekryptuj¹cej
		std::wregex regex(L"\\.sig\\|\\|([a-zA-Z0-9$]+)\\(");
		std::wsmatch matched;
		std::wstring funcName;
		if (std::regex_search(playerFile, matched, regex))
			funcName = matched[1];	
		
		
		int funcDefStartIndx = playerFile.find(std::wstring(funcName + L"=function("));
		int funcDefEndIndx = playerFile.find(L"};", funcDefStartIndx);
		std::wstring funcDef(playerFile.begin() + funcDefStartIndx, playerFile.begin() + funcDefEndIndx + 2);
		std::wstring varName = funcDef.substr(funcDef.find(L";") + 1, 2);
		int varDefStartIndx = playerFile.find(std::wstring(L"var " + varName + L"={"));
		int varDefEndIndx = playerFile.find(L"};", varDefStartIndx);
		std::wstring varDef(playerFile.begin() + varDefStartIndx, playerFile.begin() + varDefEndIndx + 2);
		std::string javaScipts = "var " + ws2s(funcDef) + ws2s(varDef);

		enum v7_err rcode = V7_OK;
		v7_val_t result;
		struct v7 *v7 = v7_create();
		rcode = v7_exec(v7, javaScipts.c_str(), &result);
		v7_val_t func, args;
		func = v7_get(v7, v7_get_global(v7), ws2s(funcName).c_str(), funcName.length());
		args = v7_mk_array(v7);
		std::string encSig = ws2s(encryptedSignature->Data());
		v7_array_push(v7, args, v7_mk_string(v7, encSig.c_str(), encSig.length(), 0));
		std::string decSig;
		if (v7_apply(v7, func, V7_UNDEFINED, args, &result) == V7_OK)
			decSig = v7_get_string(v7, &result, NULL);
		v7_destroy(v7);

		return ref new Platform::String(std::wstring(decSig.begin(), decSig.end()).c_str());
	});
}

/*void YouTubeSignatureDecrypt::downloadEmbedFile()
{
	Platform::String^ youtubeEmbedFile;
	ToolsHttp::getStrData(L"https://www.youtube.com/embed/" + videoId, youtubeEmbedFile);
	embedFile = youtubeEmbedFile->Data();
}

void YouTubeSignatureDecrypt::getPlayerUrlFromEmbedFile(std::wstring &urlToPlayer)
{
	std::wregex regex(L"\"assets\":.+?\"js\":\\s*\"([^\"]+)\"");
	std::wsmatch matched;
	if (std::regex_search(embedFile, matched, regex))
	{
		urlToPlayer = L"http:";
		urlToPlayer += matched[1];
		boost::replace_all(urlToPlayer, L"\\/", L"/");
	}
}*/

void YouTubeSignatureDecrypt::downloadPlayerFile()
{
	Platform::String^ youtubePlayerFile;
	ToolsHttp::getStrData(playerUrl, youtubePlayerFile);
	playerFile = youtubePlayerFile->Data();
}

