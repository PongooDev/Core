#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "FortniteGame/Public/Items/Definitions/FortAlterableItemDefinition.h"

class USoundBase;

class UFortSchematicItemDefinition : public UFortAlterableItemDefinition {
public:
	DefineUnrealClass(UFortSchematicItemDefinition);

	DefineUProperty(FDataTableRowHandle, CraftingRecipe);
	DefineUProperty(TSoftObjectPtr<USoundBase>, CraftingSuccessSound);
	DefineUProperty(FName, CraftingTimeRowName);
	DefineUProperty(bool, bUseSchematicDisplayName);
};
