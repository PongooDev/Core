#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
struct FFortItemEntry;

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
		int32 RecursionDepth = 0,
		int32 WorldLevel = 0,
		int32 ForcedLootTier = -1
	);
};