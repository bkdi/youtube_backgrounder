#include "pch.h"
#include "PlaylistIO.h"

using namespace Windows::Storage;
using namespace Windows::Foundation;

namespace youtube_backgrounder
{
	PlaylistIO::PlaylistIO()
	{

	}

	void PlaylistIO::Read(YoutubePlaylistsCollection^* playlists)
	{
		auto storageFolder = ApplicationData::Current->LocalFolder;

		auto t = concurrency::create_task(storageFolder->GetFileAsync("playlists.txt"));

		auto continuation = t.then([this, playlists](StorageFile^ playlistsFile)->IAsyncOperation<Platform::String^>^
		{
			return FileIO::ReadTextAsync(playlistsFile);
		}).then([this, playlists](Platform::String^ playlistsJSON)
		{
			parseJSON(playlistsJSON, playlists);
		}).then([](concurrency::task<void> t)
		{
			try
			{
				t.get();
			}
			catch (Platform::COMException^)
			{

			}
		});
	}

	void PlaylistIO::Write(YoutubePlaylistsCollection^ playlists)
	{
		Platform::String^ playlistsJSON = generateJSON(playlists);

		auto storageFolder = ApplicationData::Current->LocalFolder;

		concurrency::create_task(storageFolder->CreateFileAsync("playlists.txt", CreationCollisionOption::ReplaceExisting)).then([playlistsJSON](StorageFile^ playlistsFile)->IAsyncAction^
		{
			return FileIO::WriteTextAsync(playlistsFile, playlistsJSON); 
		});
	}

	Platform::String^ PlaylistIO::generateJSON(YoutubePlaylistsCollection^ playlists)
	{
		boost::property_tree::wptree pt;

		boost::property_tree::wptree playlistsArrayJSON;
		for (auto playlist : playlists->PlaylistItems)
		{
			boost::property_tree::wptree itemsArrayJSON;
			boost::property_tree::wptree itemJSON;
			for (auto item : playlist->Items)
			{
				itemJSON.put(L"VideoID", item->VideoId->Data());
				itemJSON.put(L"SmallThumbnail", item->SmallThumbnail->Data());
				itemJSON.put(L"LargeThumbnail", item->LargeThumbnail->Data());
				itemJSON.put(L"Title", item->Title->Data());
				itemsArrayJSON.push_back(std::make_pair(L"", itemJSON));
			}
			
			playlistsArrayJSON.put_child(playlist->Name->Data(), itemsArrayJSON);
		}

		pt.put_child(L"Playlists", playlistsArrayJSON);

		std::wstringstream ss;
		write_json(ss, pt);

		return ref new Platform::String(ss.str().c_str());
	}

	void PlaylistIO::parseJSON(Platform::String^ playlistsJSON, YoutubePlaylistsCollection^* playlists)
	{
		boost::property_tree::wptree pt;
		std::wstringstream jsonStream(playlistsJSON->Data());

		read_json(jsonStream, pt);

		for (auto playlistJSON : pt.get_child(L"Playlists"))
		{
			Platform::String^ name = ref new Platform::String(playlistJSON.first.c_str());
			auto playlist = ref new YoutubePlaylist(name);
			for (auto item : playlistJSON.second)
			{
				Platform::String^ videoId, ^title, ^smallThumbnail, ^largeThumbnail;

				auto it = item.second.find(L"VideoID");
				if (it != item.second.not_found())
					videoId = ref new Platform::String(it->second.get_value<std::wstring>().c_str());
				else
					continue;

				it = item.second.find(L"Title");
				if (it != item.second.not_found())
					title = ref new Platform::String(it->second.get_value<std::wstring>().c_str());
				else
					continue;

				it = item.second.find(L"SmallThumbnail");
				if (it != item.second.not_found())
					smallThumbnail = ref new Platform::String(it->second.get_value<std::wstring>().c_str());
				else
					continue;

				it = item.second.find(L"LargeThumbnail");
				if (it != item.second.not_found())
					largeThumbnail = ref new Platform::String(it->second.get_value<std::wstring>().c_str());
				else
					continue;

				playlist->add(ref new YoutubeItem(videoId, title, smallThumbnail, largeThumbnail));
			}

			(*playlists)->AppendPlaylist(playlist);
		}
	}
}

