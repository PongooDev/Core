#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortCloudSaveItemDefinition.h"

class UGameplayEffect;

class UFortOutpostItemDefinition : public UFortCloudSaveItemDefinition {
public:
	DefineUnrealClass(UFortOutpostItemDefinition);

	DefineUProperty(TArray<TSubclassOf<UGameplayEffect>>, StaticGameplayEffectsByLevel);
	DefineUProperty(int32, TheaterIndex);
	DefineUProperty(FText, LongDescription);
};
