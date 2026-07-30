#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"

class UFortGameplayModifierItemDefinition;

struct FFortAIPawnUpgradeData {
public:
	DefineUnrealStruct(FFortAIPawnUpgradeData);

	DefineStructProperty(FCurveTableRowHandle, SpawnPointsMultiplierCurve);
	DefineStructProperty(FCurveTableRowHandle, LifespanMultiplierCurve);
	DefineStructProperty(FCurveTableRowHandle, ScoreMultiplierCurve);
	DefineStructProperty(UFortGameplayModifierItemDefinition*, ModifierDefinition);
public:
	uint8 Padding[0x38];
};

struct FFortAIPawnUpgrade {
public:
	DefineUnrealStruct(FFortAIPawnUpgrade);

	DefineStructProperty(FGameplayTagQuery, TagQuery);
	DefineStructProperty(TArray<FFortAIPawnUpgradeData>, PawnUpgradeDataPerPlayerCount);
public:
	uint8 Padding[0x58];
};

struct FFortAISpawnGroupUpgradeUIData {
public:
	DefineUnrealStruct(FFortAISpawnGroupUpgradeUIData);

	DefineStructProperty(bool, bAlwaysDisplayHealthBar);
	DefineStructProperty(bool, bOverrideHealthBarColor);
	DefineStructProperty(FSlateBrush, UpgradeIcon);
	DefineStructProperty(FLinearColor, HealthBarColorOverride);
	DefineStructProperty(FText, UpgradeName);
public:
	uint8 Padding[0xB8];
};

class UFortAISpawnGroupUpgrade : public UObject {
public:
	DefineUnrealClass(UFortAISpawnGroupUpgrade);

	DefineUProperty(FGameplayTagContainer, UpgradeTags);
	DefineUProperty(FGameplayTagQuery, GroupRequiredTagQuery);
	DefineUProperty(TArray<FFortAIPawnUpgrade>, BasePawnUpgrades);
	DefineUProperty(FName, AdditionalModifiersLootTierGroup);
	DefineUProperty(bool, bInvalidForEnemySpawners);
	DefineUProperty(float, SpawnGroupDiscountPercentage);
	DefineUProperty(FFortAISpawnGroupUpgradeUIData, UIData);
};
