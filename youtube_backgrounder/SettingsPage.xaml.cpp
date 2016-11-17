//
// SettingsPage.xaml.cpp
// Implementation of the SettingsPage class
//

#include "pch.h"
#include "SettingsPage.xaml.h"
#include "Settings.h"

using namespace youtube_backgrounder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

SettingsPage::SettingsPage()
{
	InitializeComponent();

	YoutubeQualityCollections^ itemsCollection = ref new YoutubeQualityCollections;
	for(auto quality : sortedQualities)
		itemsCollection->AppendItem(quality);
	MaterialQualityCombo->ItemsSource = itemsCollection->YoutubeQualities;

	if (!SettingsHelper::existValue(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY))
	{
		MaterialQualityCombo->SelectedIndex = 3;
		SettingsHelper::setPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY, safe_cast<unsigned int> (YoutubeQuality::High));
	}
	else
	{
		auto preferedQuality = safe_cast<YoutubeQuality> (SettingsHelper::getPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY));

		unsigned index = 0;
		for (auto item : itemsCollection->YoutubeQualities)
		{
			if (item->Quality == preferedQuality)
			{
				MaterialQualityCombo->SelectedIndex = index;
				break;
			}
			++index;
		}
	}

	if (!SettingsHelper::existValue(Settings::MATERIAL, Settings::Material::ONLYAUDIO))
	{
		OnlyAutoTypeSwitch->IsOn = false;
		SettingsHelper::setPropertyBoolean(Settings::MATERIAL, Settings::Material::ONLYAUDIO, false);
	}
	else
		OnlyAutoTypeSwitch->IsOn = SettingsHelper::getPropertyBoolean(Settings::MATERIAL, Settings::Material::ONLYAUDIO);
}

void SettingsPage::OnlyAutoTypeSwitch_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SettingsHelper::setPropertyBoolean(Settings::MATERIAL, Settings::Material::ONLYAUDIO, OnlyAutoTypeSwitch->IsOn);
}

void SettingsPage::MaterialQualityCombo_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	auto item = safe_cast<YoutubeQualityItem^> (MaterialQualityCombo->Items->GetAt(MaterialQualityCombo->SelectedIndex));
	SettingsHelper::setPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY, safe_cast<unsigned int> (item->Quality));
}
