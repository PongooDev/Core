#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class AFortPlayerState;
class AFortPawn;
class AFortPlayerStateAthena;

struct FDeathInfo {
public:
	DefineUnrealStruct(FDeathInfo);

	DefineStructProperty(AFortPlayerStateAthena*, FinisherOrDowner);
	DefineStructProperty(bool, bDBNO);
	DefineStructProperty(uint8, DeathCause);
	DefineStructProperty(float, Distance);
	DefineStructProperty(FVector, DeathLocation);
	DefineStructProperty(bool, bInitialized);
	DefineStructProperty(int32, DeathCircumstance);
public:
	uint8 Padding[0x50];
};