#pragma once

#include "YoutubeItemData.h"

namespace youtube_backgrounder
{
	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubePlaylist sealed : public INotifyPropertyChanged
	{
	public:
		YoutubePlaylist();
		YoutubePlaylist(Platform::String^ playlistName);

		property Platform::String^ Name;
		property IVector<YoutubeItem^>^ Items
		{
			IVector<YoutubeItem^>^ get()
			{ 
				return getItems(); 
			}
		};
		
		property unsigned int TracksCount
		{
			unsigned int get()
			{
				return Items->Size;
			}
		};
		
		void add(YoutubeItem^ item);
		void clear();
		void reset();
		void deleteItem(YoutubeItem^ item);
		IVector<YoutubeItem^>^ getItems();

		virtual event PropertyChangedEventHandler^ PropertyChanged;
		property Windows::Foundation::EventRegistrationToken eventToken;

		property int NowPlayingIndex
		{
			int get() { return nowPlayingIndex; }
			void set(int value)
			{
				nowPlayingIndex = value;
				OnPropertyChanged(L"NowPlayingIndex");
			}
		}

	protected:
		void OnPropertyChanged(Platform::String^ name)
		{
			PropertyChanged(this, ref new PropertyChangedEventArgs(name));
		}


	private:
		YoutubeItemsCollections^ itemsCollection;
		int nowPlayingIndex;
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class YoutubePlaylistsCollection sealed
	{
	private:
		Vector<YoutubePlaylist^>^ vec;

	public:
		YoutubePlaylistsCollection();
		void AppendPlaylist(YoutubePlaylist^ item);
		bool FindByName(Platform::String^ name);
		void DeletePlaylist(YoutubePlaylist^ item);

		property IVector<YoutubePlaylist^>^ PlaylistItems
		{
			IVector<YoutubePlaylist^>^ get()
			{
				return vec;
			}
		}
	};
}

