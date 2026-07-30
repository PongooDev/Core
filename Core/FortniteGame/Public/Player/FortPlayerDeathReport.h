#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class AFortPlayerState;
class AFortPawn;
class AFortPlayerStateAthena;

struct FFortPlayerDeathReport {
public:
	DefineUnrealStruct(FFortPlayerDeathReport);

	DefineStructProperty(float, ServerTimeForRespawn);
	DefineStructProperty(float, ServerTimeForResurrect);
	DefineStructProperty(float, LethalDamage);
	DefineStructProperty(AFortPlayerState*, KillerPlayerState);
	DefineStructProperty(AFortPawn*, KillerPawn);
	DefineStructProperty(AActor*, DamageCauser);
	DefineBitfieldStructProperty(bDroppedBackpack);
	DefineBitfieldStructProperty(bNotifyUI);
	DefineStructProperty(FGameplayTagContainer, Tags);
public:
	uint8 Padding[0x50];
};