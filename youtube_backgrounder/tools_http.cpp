#include "pch.h"
#include "tools_http.h"

using namespace Windows::Web::Http;
using namespace Windows::Foundation;

namespace ToolsHttp
{
	void getStrData(Platform::String^ url, Platform::String^& data)
	{
		auto httpClient = ref new HttpClient();
		auto uri = ref new Uri(url);

		concurrency::create_task(httpClient->GetStringAsync(uri)).then([&data](Platform::String^ youtubeGetVideoInfoFile)
		{
			data = youtubeGetVideoInfoFile;
		}).wait();
	}

	concurrency::task<Platform::String^ > getDownloadingTask(Platform::String^ url)
	{
		auto httpClient = ref new HttpClient();
		auto uri = ref new Uri(url);

		return concurrency::create_task(httpClient->GetStringAsync(uri));
	}
}