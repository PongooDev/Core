#pragma once
#include "pch.h"

#include "FortPlaylist.h"

class UFortPlaylistAthena : public UFortPlaylist {
public:
	DefineUnrealClass(UFortPlaylistAthena);

	DefineUProperty(uint8, SafeZoneStartUp);
	DefineUProperty(bool, bIsLargeTeamGame);
};