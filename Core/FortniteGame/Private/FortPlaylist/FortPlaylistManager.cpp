#include "pch.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistManager.h"

#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"
#include "FortniteGame/Public/FortGameInstance/FortGameInstance.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

UFortPlaylistAthena* UFortPlaylistManager::GetPlaylist(int32 PlaylistId) {
	for (int i = 0; i < AthenaPlaylists.Num(); i++) {
		UFortPlaylistAthena* Playlist = AthenaPlaylists[i];
		if (Playlist) {
			if (Playlist->GetPlaylistId() == PlaylistId) {
				return Playlist;
			}
		}
	}
	
	static TArray<UObject*> Playlists = FUObjectArray::GetObjectsOfClass(UFortPlaylistAthena::StaticClass());
	for (int i = 0; i < Playlists.Num(); i++) {
		UFortPlaylistAthena* Playlist = Playlists[i]->Cast<UFortPlaylistAthena>();
		if (Playlist) {
			if (Playlist->GetPlaylistId() == PlaylistId) {
				return Playlist;
			}
		}
	}

	Log("UFortPlaylistManager::GetAthenaPlaylist: Unable to find playlist with PlaylistId: " + std::to_string(PlaylistId));
	return nullptr;
}

UFortPlaylistAthena* UFortPlaylistManager::GetPlaylist(FName PlaylistName) {
	for (int i = 0; i < AthenaPlaylists.Num(); i++) {
		UFortPlaylistAthena* Playlist = AthenaPlaylists[i];
		if (Playlist) {
			if (Playlist->GetPlaylistName() == PlaylistName) {
				return Playlist;
			}
		}
	}
	
	static TArray<UObject*> Playlists = FUObjectArray::GetObjectsOfClass(UFortPlaylistAthena::StaticClass());
	for (int i = 0; i < Playlists.Num(); i++) {
		UFortPlaylistAthena* Playlist = Playlists[i]->Cast<UFortPlaylistAthena>();
		if (Playlist) {
			if (Playlist->GetPlaylistName() == PlaylistName) {
				return Playlist;
			}
		}
	}

	Log("UFortPlaylistManager::GetAthenaPlaylist: Unable to find playlist " + PlaylistName.ToString().ToString());
	return nullptr;
}

UFortPlaylistManager* UFortPlaylistManager::Get() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("UFortPlaylistManager::Get: Unable to get FortPlaylistManager, World is nullptr!");
		return nullptr;
	}

	AFortGameMode* FortGameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!FortGameMode) {
		Log("UFortPlaylistManager::Get: Unable to get FortPlaylistManager, AuthorityGameMode is nullptr!");
		return nullptr;
	}

	return FortGameMode->PlaylistManager;
}