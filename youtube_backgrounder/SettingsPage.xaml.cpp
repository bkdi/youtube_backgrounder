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
	for(auto quality : vecSortedQualities)
		itemsCollection->AppendItem(quality);
	QualityCombo->ItemsSource = itemsCollection->YoutubeQualities;

	if (!SettingsHelper::existValue(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY))
	{
		QualityCombo->SelectedIndex = 2;
		SettingsHelper::setPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY, safe_cast<unsigned int> (YoutubeQualityItag::High_720p_HD));
	}
	else
	{
		YoutubeQualityItag preferedQuality = safe_cast<YoutubeQualityItag> (SettingsHelper::getPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY));

		unsigned index = 0;
		for (auto item : itemsCollection->YoutubeQualities)
		{
			if (item->Quality == preferedQuality)
			{
				QualityCombo->SelectedIndex = index;
				break;
			}
			++index;
		}
	}
}

void SettingsPage::QualityCombo_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	auto item = safe_cast<YoutubeQualityItem^> (QualityCombo->Items->GetAt(QualityCombo->SelectedIndex));
	SettingsHelper::setPropertyUInt32(Settings::MATERIAL, Settings::Material::PREFEREDQUALITY, safe_cast<unsigned int> (item->Quality));
}