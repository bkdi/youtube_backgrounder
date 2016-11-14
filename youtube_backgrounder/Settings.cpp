#include "pch.h"
#include "Settings.h"

using namespace Windows::Foundation;
using namespace Windows::Storage;

namespace youtube_backgrounder
{
	namespace Settings
	{
		Platform::String^ const MATERIAL = L"Material";
		namespace Material
		{
			Platform::String^ const PREFEREDQUALITY = L"PreferedQuality";
		}
	}

	namespace SettingsHelper
	{
		bool existValue(Platform::String^ compositeName, Platform::String^ propertyName)
		{
			bool exist = false;

			auto localSettings = ApplicationData::Current->LocalSettings;
			ApplicationDataCompositeValue^ composite = safe_cast<ApplicationDataCompositeValue^>(localSettings->Values->Lookup(compositeName));
			if (composite != nullptr)
			{
				auto compositeProperty = composite->Lookup(propertyName);
				if (compositeProperty != nullptr)
					exist = true;
			}

			return exist;
		}

		void setPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName, unsigned int value)
		{
			auto localSettings = ApplicationData::Current->LocalSettings;
			ApplicationDataCompositeValue^ composite = safe_cast<ApplicationDataCompositeValue^>(localSettings->Values->Lookup(compositeName));
			if (composite == nullptr)
				composite = ref new ApplicationDataCompositeValue();

			composite->Insert(propertyName, dynamic_cast<PropertyValue^>(PropertyValue::CreateUInt32(value)));
			localSettings->Values->Insert(compositeName, composite);
		}

		unsigned int getPropertyUInt32(Platform::String^ compositeName, Platform::String^ propertyName)
		{
			auto localSettings = ApplicationData::Current->LocalSettings;
			ApplicationDataCompositeValue^ composite = safe_cast<ApplicationDataCompositeValue^>(localSettings->Values->Lookup(compositeName));

			if (composite != nullptr)
			{
				auto property = composite->Lookup(propertyName);
				if (composite != nullptr)
					return safe_cast<IPropertyValue^>(property)->GetUInt32();
				else
					throw ref new Platform::NullReferenceException();
			}
			else
				throw ref new Platform::NullReferenceException();
		}
	}
}