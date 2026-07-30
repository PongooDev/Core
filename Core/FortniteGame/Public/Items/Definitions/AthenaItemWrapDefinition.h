#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UMaterialInterface;

class UAthenaItemWrapDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaItemWrapDefinition);

	DefineUProperty(TSoftClassPtr<UClass>, ItemWrapModifierBlueprint);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, ItemWrapMaterial);
};
