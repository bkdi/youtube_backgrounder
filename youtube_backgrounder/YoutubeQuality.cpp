#include "pch.h"

#include "YoutubeQuality.h"

using namespace youtube_backgrounder;

const std::vector<YoutubeQuality> youtube_backgrounder::sortedQualities = { YoutubeQuality::VeryLow, YoutubeQuality::Low, YoutubeQuality::Medium, YoutubeQuality::High };

const std::vector<YoutubeQualityItag> youtube_backgrounder::sortedNonDASH_2DVideoQualities = 
{ 
	YoutubeQualityItag::Non_DASH_2D_3GP_VeryLow,
	YoutubeQualityItag::Non_DASH_2D_3GP_Low,	
	YoutubeQualityItag::Non_DASH_2D_FLV_Low,	
	YoutubeQualityItag::Non_DASH_2D_MP4_Medium,	
	YoutubeQualityItag::Non_DASH_2D_WebM_Medium,	
	YoutubeQualityItag::Non_DASH_2D_MP4_High		
};

const std::vector<YoutubeQualityItag> youtube_backgrounder::sortedDASH_AudioQualities =
{
	YoutubeQualityItag::DASH_Audio_WebM_VeryLow,
	YoutubeQualityItag::DASH_Audio_WebM_Low,		
	YoutubeQualityItag::DASH_Audio_WebM_Medium,	
	YoutubeQualityItag::DASH_Audio_M4A_Medium,	
	YoutubeQualityItag::DASH_Audio_WebM_High		
};

YoutubeQualityItem::YoutubeQualityItem(YoutubeQuality youtubeQuality)
{
	Quality = youtubeQuality;
	Name = qualityToDescription(youtubeQuality);
}

Platform::String^ YoutubeQualityItem::qualityToDescription(YoutubeQuality youtubeQuality)
{
	switch (youtubeQuality)
	{
	case YoutubeQuality::VeryLow:
		return L"Very low";
	case YoutubeQuality::Low:
		return L"Low";
	case YoutubeQuality::Medium:
		return L"Medium";
	case YoutubeQuality::High:
		return L"High";
	default:
		return L"";
	}
}

YoutubeQualityItag YoutubeQualityItem::qualityTo_Non_DASH_2D_Quality(YoutubeQuality youtubeQuality)
{
	switch (youtubeQuality)
	{
	case YoutubeQuality::VeryLow:
		return YoutubeQualityItag::Non_DASH_2D_3GP_VeryLow;
	case YoutubeQuality::Low:
		return YoutubeQualityItag::Non_DASH_2D_FLV_Low;
	case YoutubeQuality::Medium:
		return YoutubeQualityItag::Non_DASH_2D_WebM_Medium;
	case YoutubeQuality::High:
		return YoutubeQualityItag::Non_DASH_2D_MP4_High;
	default:
		throw ref new Platform::InvalidArgumentException();
	}
}

YoutubeQualityItag YoutubeQualityItem::qualityTo_DASH_Audio_Quality(YoutubeQuality youtubeQuality)
{
	switch (youtubeQuality)
	{
	case YoutubeQuality::VeryLow:
		return YoutubeQualityItag::DASH_Audio_WebM_VeryLow;
	case YoutubeQuality::Low:
		return YoutubeQualityItag::DASH_Audio_WebM_Low;
	case YoutubeQuality::Medium:
		return YoutubeQualityItag::DASH_Audio_M4A_Medium;
	case YoutubeQuality::High:
		return YoutubeQualityItag::DASH_Audio_WebM_High;
	default:
		throw ref new Platform::InvalidArgumentException();
	}
}

YoutubeQualityCollections::YoutubeQualityCollections()
{
	vec = ref new Vector<YoutubeQualityItem^>();
}

void YoutubeQualityCollections::AppendItem(YoutubeQuality youtubeQuality)
{
	vec->Append(ref new YoutubeQualityItem(youtubeQuality));
}
