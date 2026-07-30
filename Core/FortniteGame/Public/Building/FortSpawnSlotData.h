#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AFortAIPawn;

struct FFortSpawnSlotData {
public:
	DefineUnrealStruct(FFortSpawnSlotData);

	DefineStructProperty(FVector, SpawnSlotLocation);
	DefineStructProperty(AFortAIPawn*, OccupyingAI);
	DefineStructProperty(uint8, SlotStatus);
public:
	uint8 Padding[0x20];
};
