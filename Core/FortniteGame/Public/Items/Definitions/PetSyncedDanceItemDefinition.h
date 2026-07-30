#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaDanceItemDefinition.h"

class UAnimMontage;

class UPetSyncedDanceItemDefinition : public UAthenaDanceItemDefinition {
public:
	DefineUnrealClass(UPetSyncedDanceItemDefinition);

	DefineUProperty(TSoftObjectPtr<UAnimMontage>, DefaultPetAnimation);
};
