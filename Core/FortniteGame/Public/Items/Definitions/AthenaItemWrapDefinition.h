#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UMaterialInterface;

class UAthenaItemWrapDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaItemWrapDefinition);

	DefineUProperty(TSoftClassPtr<UClass>, ItemWrapModifierBlueprint);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, ItemWrapMaterial);
};
