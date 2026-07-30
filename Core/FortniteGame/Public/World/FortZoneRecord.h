#pragma once
#include "pch.h"

#include "FortniteGame/Public/World/FortGenericRecord.h"

class UFortZoneRecord : public UFortGenericRecord {
public:
	DefineUnrealClass(UFortZoneRecord);

	DefineUProperty(int32, WorldSaveCount);
	DefineUProperty(int32, ZoneIndex);
	DefineUProperty(FName, PersistentLevelName);
	DefineUProperty(int32, NumSavedLevels);
	DefineUProperty(int32, ZoneTileSeed);
};
