#include "pch.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylist.h"

#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"

int32& UFortPlaylist::GetPlaylistId() {
	if (UProperty* PlaylistIdProperty = UFortPlaylistAthena::StaticClass()->FindPropertyByName("PlaylistId")) {
		return *(int32*)((uintptr_t)this + PlaylistIdProperty->Offset_Internal);
	}

	return PlaylistId;
}

FName& UFortPlaylist::GetPlaylistName() {
	if (UProperty* PlaylistNameProperty = UFortPlaylistAthena::StaticClass()->FindPropertyByName("PlaylistName")) {
		return *(FName*)((uintptr_t)this + PlaylistNameProperty->Offset_Internal);
	}

	return PlaylistName;
}