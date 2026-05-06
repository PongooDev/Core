#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
struct FFortItemEntry;
class FFortLootTierData;

struct FFortLootPackageData : public FTableRowBase {
public:
	DefineUnrealStruct(FFortLootPackageData);

	DefineStructProperty(FName, LootPackageID);
	DefineStructProperty(float, Weight);
	DefineStructProperty(FName, NamedWeightMult);
	DefineStructProperty(TArray<FName>, PotentialNamedWeights);
	DefineStructProperty(int32, Count);
	DefineStructProperty(int32, LootPackageCategory);
	DefineStructProperty(FGameplayTagContainer, GameplayTags);
	DefineStructProperty(FName, RequiredTag);
	DefineStructProperty(FString, LootPackageCall);
	DefineStructProperty(TSoftObjectPtr<UFortItemDefinition>, ItemDefinition);
	DefineStructProperty(FString, PersistentLevel);
	DefineStructProperty(int32, MinWorldLevel);
	DefineStructProperty(int32, MaxWorldLevel);
	DefineStructProperty(bool, bAllowBonusDrops);
	DefineStructProperty(FString, Annotation);
public:
	static FFortLootPackageData* GetLootPackage(
		TArray<FFortLootPackageData*> LootPackages
	);
	static TArray<FFortLootPackageData*> GetLootPackages(
		TArray<UDataTable*> LootPackageDataTables,
		FFortLootTierData* LootTierData
	);

	static TArray<FFortLootPackageData*> GetCandidateLootPackages(
		TArray<UDataTable*>& LootPackageDataTables,
		TArray<FFortLootPackageData*> LootPackages,
		FString LootPackageID,
		FFortLootTierData* LootTierData
	);

	static TArray<FFortItemEntry> GetLootItems(
		TArray<UDataTable*> LootPackageDataTables,
		FFortLootTierData* LootTierData
	);
};