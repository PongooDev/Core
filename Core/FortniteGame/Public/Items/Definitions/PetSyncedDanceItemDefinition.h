#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaDanceItemDefinition.h"

class UAnimMontage;

class UPetSyncedDanceItemDefinition : public UAthenaDanceItemDefinition {
public:
	DefineUnrealClass(UPetSyncedDanceItemDefinition);

	DefineUProperty(TSoftObjectPtr<UAnimMontage>, DefaultPetAnimation);
};
