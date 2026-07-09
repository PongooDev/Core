#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortPlaylistAthena;

class UFortPlaylistManager : public UObject {
public:
	DefineUnrealClass(UFortPlaylistManager);

	DefineUProperty(TArray<UFortPlaylistAthena*>, AthenaPlaylists);
public:
	UFortPlaylistAthena* GetPlaylist(int32 PlaylistId);
	UFortPlaylistAthena* GetPlaylist(FName PlaylistName);

	static UFortPlaylistManager* Get();
};