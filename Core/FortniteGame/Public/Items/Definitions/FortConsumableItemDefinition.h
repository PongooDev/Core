#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

class UAnimMontage;

class UFortConsumableItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortConsumableItemDefinition);

	DefineUProperty(TSoftObjectPtr<UAnimMontage>, UseAnimation);
	DefineUProperty(float, UseTime);
	DefineUProperty(bool, bRequiresMissingHealth);
};
