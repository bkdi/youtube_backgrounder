#include "pch.h"
#include "tools_http.h"

using namespace Windows::Web::Http;
using namespace Windows::Foundation;

namespace ToolsHttp
{
	auto getStrFileAsync(Platform::String^ url)
	{
		auto httpClient = ref new HttpClient();

		auto uri = ref new Uri(url);
		return httpClient->GetStringAsync(uri);
	}
}