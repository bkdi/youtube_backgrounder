#pragma once

namespace youtube_backgrounder
{
	namespace Settings
	{
		extern Platform::String^ const MATERIAL;
		namespace Material
		{
			extern Platform::String^ const PREFEREDQUALITY;
			extern Platform::String^ const ONLYAUDIO;
		}
	}

	namespace SettingsHelper
	{
		bool existValue(Platform::String^ compositeName, Platform::String^ propertyName);

		void setPropertyBoolean(Platform::String^ compositeName, Platform::String^ propertyName, bool value);
		void setPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName, unsigned int value);

		unsigned int getPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName);
		bool getPropertyBoolean(Platform::String^ compositeName, Platform::String^ propertyName);
	}
}