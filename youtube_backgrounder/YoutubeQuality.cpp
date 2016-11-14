#include "pch.h"

#include "YoutubeQuality.h"

using namespace youtube_backgrounder;

const std::vector<YoutubeQualityItag> youtube_backgrounder::vecSortedQualities = { YoutubeQualityItag::Low_240p, YoutubeQualityItag::Medium_360p, YoutubeQualityItag::High_720p_HD };

YoutubeQualityItem::YoutubeQualityItem(YoutubeQualityItag youtubeQuality)
{
	Quality = youtubeQuality;
	Name = qualityToDescription(youtubeQuality);
}

Platform::String^ YoutubeQualityItem::qualityToDescription(YoutubeQualityItag youtubeQuality)
{
	switch (youtubeQuality)
	{
	case YoutubeQualityItag::Low_240p:
		return L"Low (240p)";
	case YoutubeQualityItag::Medium_360p:
		return L"Medium (360p)";
	case YoutubeQualityItag::High_720p_HD:
		return L"High (720p HD)";
	default:
		return L"";
	}
}

YoutubeQualityCollections::YoutubeQualityCollections()
{
	vec = ref new Vector<YoutubeQualityItem^>();
}

void YoutubeQualityCollections::AppendItem(YoutubeQualityItag youtubeQuality)
{
	vec->Append(ref new YoutubeQualityItem(youtubeQuality));
}
