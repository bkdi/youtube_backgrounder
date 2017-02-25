#pragma once

namespace ToolsHttp
{
	void getStrData(Platform::String^ url, Platform::String^& data);
	concurrency::task<Platform::String^ > getDownloadingTask(Platform::String^ url);
}