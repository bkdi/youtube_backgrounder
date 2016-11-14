#pragma once

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

namespace youtube_backgrounder
{
	public enum class YoutubeQualityItag
	{
		Low_240p = 5,
		Medium_360p = 18,
		High_720p_HD = 22
	};

	extern const std::vector<YoutubeQualityItag> vecSortedQualities;

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeQualityItem sealed
	{
	public:
		YoutubeQualityItem(YoutubeQualityItag youtubeQuality);

		property Platform::String^ Name;
		property YoutubeQualityItag Quality;

		static Platform::String^ qualityToDescription(YoutubeQualityItag youtubeQuality);
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubeQualityCollections sealed
	{
	private:
		Vector<YoutubeQualityItem^>^ vec;

	public:
		YoutubeQualityCollections();
		void AppendItem(YoutubeQualityItag youtubeQuality);

		property IVector<YoutubeQualityItem^>^ YoutubeQualities
		{
			IVector<YoutubeQualityItem^>^ get()
			{
				return vec;
			}
		}
	};
}