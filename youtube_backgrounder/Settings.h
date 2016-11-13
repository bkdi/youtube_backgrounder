#pragma once

namespace youtube_backgrounder
{
	namespace Settings
	{
		static Platform::String^ MATERIAL = L"Material";
		namespace Material
		{
			static Platform::String^ PREFEREDQUALITY = L"PreferedQuality";
		}
	}

	namespace SettingsHelper
	{
		bool existValue(Platform::String^ compositeName, Platform::String^ propertyName);

		void setPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName, unsigned int value);

		unsigned int getPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName);
	}
}