#pragma once

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

namespace youtube_backgrounder
{
	public enum class YoutubeQuality
	{
		VeryLow,
		Low,
		Medium,
		High
	};

	extern const std::vector<YoutubeQuality> sortedQualities;

	public enum class YoutubeQualityItag
	{
		Non_DASH_2D_3GP_VeryLow = 17,	//3GP Video: MPEG-4 144p 0.05 Mbit/s Audio: AAC 24 kbit/s
		Non_DASH_2D_3GP_Low = 36,		//3GP Video: MPEG-4 240p 0.175 Mbit/s Audio: AAC 32 kbit/s
		Non_DASH_2D_FLV_Low = 5,		//FLV Video: H.263 240p 0.25 Mbit/s Audio: MP3 64 kbit/s
		Non_DASH_2D_MP4_Medium = 18,	//MP4 Video: H.264 360p 0.5 Mbit/s Audio: AAC 96 kbit/s
		Non_DASH_2D_WebM_Medium = 43,	//WebM Video: Vp8 360p 0.5 Mbit/s Audio: Vorbis 128 kbit/s
		Non_DASH_2D_MP4_High = 22,		//MP4 Video: H.264 720p 2-3 Mbit/s Audio: AAC 192 kbit/s
		DASH_Audio_WebM_VeryLow = 249,	//WebM Audio: Opus 48 kbit/s
		DASH_Audio_WebM_Low = 250,		//WebM Audio: Opus 64 kbit/s	
		DASH_Audio_WebM_Medium = 171,	//WebM Audio: Vorbis 128 kbit/s
		DASH_Audio_M4A_Medium = 140,	//M4A Audio: AAC 128 kbit/s 
		DASH_Audio_WebM_High = 251		//WebM Audio: Opus 160 kbit/s
	};

	extern const std::vector<YoutubeQualityItag> sortedNonDASH_2DVideoQualities;
	extern const std::vector<YoutubeQualityItag> sortedDASH_AudioQualities;

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeQualityItem sealed
	{
	public:
		YoutubeQualityItem(YoutubeQuality youtubeQuality);

		property Platform::String^ Name;
		property YoutubeQuality Quality;

		static Platform::String^ qualityToDescription(YoutubeQuality youtubeQuality);
		static YoutubeQualityItag qualityTo_Non_DASH_2D_Quality(YoutubeQuality youtubeQuality);
		static YoutubeQualityItag qualityTo_DASH_Audio_Quality(YoutubeQuality youtubeQuality);
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeQualityCollections sealed
	{
	private:
		Vector<YoutubeQualityItem^>^ vec;

	public:
		YoutubeQualityCollections();
		void AppendItem(YoutubeQuality youtubeQuality);

		property IVector<YoutubeQualityItem^>^ YoutubeQualities
		{
			IVector<YoutubeQualityItem^>^ get()
			{
				return vec;
			}
		}
	};
}