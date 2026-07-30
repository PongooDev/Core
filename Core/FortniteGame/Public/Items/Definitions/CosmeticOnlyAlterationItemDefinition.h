#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAlterationItemDefinition.h"

class UMaterialInterface;

class UCosmeticOnlyAlterationItemDefinition : public UFortAlterationItemDefinition {
public:
	DefineUnrealClass(UCosmeticOnlyAlterationItemDefinition);

	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, VehicleWrapMaterial);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, VehicleWrapMaterial_Masked);
};
