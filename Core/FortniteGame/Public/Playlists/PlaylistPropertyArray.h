#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class UFortPlaylistAthena;

struct FPlaylistPropertyArray : public FFastArraySerializer
{
public:
	DefineUnrealStruct(FPlaylistPropertyArray);

	DefineStructProperty(int32, PlaylistReplicationKey);
	DefineStructProperty(UFortPlaylistAthena*, BasePlaylist);
	DefineStructProperty(UFortPlaylistAthena*, OverridePlaylist);
public:
	void SetBasePlaylist(UFortPlaylistAthena* Playlist)
	{
		PlaylistReplicationKey++;
		BasePlaylist = Playlist;

		if (PlaylistReplicationKey == -1)
			PlaylistReplicationKey = 0;
	}
public:
	uint8 Padding[0xE8];
};