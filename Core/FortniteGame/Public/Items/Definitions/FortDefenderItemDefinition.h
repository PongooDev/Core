#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Customization/FortCharacterType.h"

class UFortDefenderItemDefinition : public UFortCharacterType {
public:
	DefineUnrealClass(UFortDefenderItemDefinition);

	DefineUProperty(FName, AppearanceOverrideName);
	DefineUProperty(TSoftClassPtr<UClass>, PawnClass);
};
