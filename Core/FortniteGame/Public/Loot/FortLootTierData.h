#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"


class UFortItemDefinition;
struct FFortItemEntry;

enum class ELootQuotaLevel : uint8
{
	Unlimited = 0,
	Level1 = 1,
	Level2 = 2,
	Level3 = 3,
	Level4 = 4,
	Level5 = 5,
	Level6 = 6,
	Level7 = 7,
	Level8 = 8,
	Level9 = 9,
	Level10 = 10,
	Level11 = 11,
	Level12 = 12,
	Level13 = 13,
	Level14 = 14,
	Level15 = 15,
	Level16 = 16,
	Level17 = 17,
	NumLevels = 18,
};

struct FFortLootTierData : public FTableRowBase {
public:
	DefineUnrealStruct(FFortLootTierData);

	DefineStructProperty(FName, TierGroup);
	DefineStructProperty(float, Weight);
	DefineStructProperty(ELootQuotaLevel, QuotaLevel);
	DefineStructProperty(int32, LootTier);
	DefineStructProperty(int32, MinWorldLevel);
	DefineStructProperty(int32, MaxWorldLevel);
	DefineStructProperty(FName, LootPackage);
	DefineStructProperty(FName, LootPreviewPackage);
	DefineStructProperty(float, NumLootPackageDrops);
	DefineStructProperty(TArray<int32>, LootPackageCategoryWeightArray);
	DefineStructProperty(TArray<int32>, LootPackageCategoryMinArray);
	DefineStructProperty(TArray<int32>, LootPackageCategoryMaxArray);
public:
	uint8 Padding[0xE0];
public:
	bool IsValid();

	static FFortLootTierData* ChooseLootTierData(TArray<FFortLootTierData*>& LootTierData);
	static FFortLootTierData* ChooseLootTierData(
		TArray<UDataTable*> LootTierDataTables,
		FName TierGroupName,
		int32 WorldLevel = 0,
		int32 ForcedLootTier = -1
	);

	int32 GetMinimumLootDrops();
	int32 GetTotalCategoryWeight();
	int32 GetTotalCategoryMin();

	static FFortLootTierData* PickLootTierData(
		TArray<UDataTable*> LootTierDataTables,
		FName TierGroupName,
		int32 WorldLevel = -1,
		int32 ForcedLootTier = -1,
		int32 RecursionDepth = 0
	);
};
