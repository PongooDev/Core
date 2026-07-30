#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

#include "FortniteGame/Public/AI/FortRiftReservationHandle.h"

class ABuildingRift;

struct FFortAIEncounterRift {
public:
	DefineUnrealStruct(FFortAIEncounterRift);

	DefineStructProperty(int32, QueryID);
	DefineStructProperty(FVector, RiftLocation);
	DefineStructProperty(ABuildingRift*, RiftActor);
	DefineStructProperty(FFortRiftReservationHandle, RiftReservationHandle);
public:
	uint8 Padding[0x20];
};
