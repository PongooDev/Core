#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Customization/FortCharacterType.h"

struct FFortAttributeInitializationKey {
public:
	DefineUnrealStruct(FFortAttributeInitializationKey);

	DefineStructProperty(FName, AttributeInitCategory);
	DefineStructProperty(FName, AttributeInitSubCategory);
public:
	uint8 Padding[0x10];
};

class UFortDefenderItemDefinition : public UFortCharacterType {
public:
	DefineUnrealClass(UFortDefenderItemDefinition);

	DefineUProperty(FName, AppearanceOverrideName);
	DefineUProperty(FFortAttributeInitializationKey, AttributeInitKey);
	DefineUProperty(TSoftClassPtr<UClass>, PawnClass);
};
