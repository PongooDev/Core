#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FAthenaMatchStats {
public:
	DefineUnrealStruct(FAthenaMatchStats);

	DefineStructProperty(int32, Place);
	DefineStructProperty(int32, TotalPlayers);
	DefineStructProperty(int32, SecondsAlive);
	DefineStructProperty(int32, Kills);
	DefineStructProperty(int32, Downs);
	DefineStructProperty(int32, Assists);
	DefineStructProperty(int32, Revives);
	DefineStructProperty(int32, DamageDealtToHostiles);
	DefineStructProperty(int32, DamageDealtToFriends);
	DefineStructProperty(int32, DamageDealtToStructures);
	DefineStructProperty(int32, DamageTaken);
	DefineStructProperty(int32, RangedHit);
	DefineStructProperty(int32, RangedMiss);
	DefineStructProperty(float, Accuracy);
	DefineStructProperty(int32, TravelDistanceGround);
	DefineStructProperty(int32, MaterialsGathered);
	DefineStructProperty(int32, MaterialsUsed);
	DefineStructProperty(int32, CriticalShots);
public:
	uint8 Padding[0x74];
};