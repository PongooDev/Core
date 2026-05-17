#include "pch.h"
#include "FortniteGame/Public/FortLoot/FortLootTierData.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

bool FFortLootTierData::IsValid() {
	if (NumLootPackageDrops <= 0.0f) {
		return false;
	}

	if (LootPackageCategoryMinArray.Num() != LootPackageCategoryWeightArray.Num() ||
		LootPackageCategoryMinArray.Num() != LootPackageCategoryMaxArray.Num()) {
		return false;
	}

	return true;
}

FFortLootTierData* FFortLootTierData::ChooseLootTierData(TArray<FFortLootTierData*>& LootTierData)
{
	if (LootTierData.Num() == 0) {
		return nullptr;
	}

	float TotalWeight = 0.0f;

	for (auto Item : LootTierData)
	{
		if (!Item)
			continue;

		if (Item->Weight > 0.0f)
			TotalWeight += Item->Weight;
	}

	if (TotalWeight <= 0.0f)
	{
		Log("FFortLootTierData::ChooseLootTierData: TotalWeight <= 0!");
		return nullptr;
	}

	float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

	for (auto Item : LootTierData)
	{
		if (!Item || Item->Weight <= 0.0f)
			continue;

		if (RandomNumber <= Item->Weight)
		{
			return Item;
		}

		RandomNumber -= Item->Weight;
	}

	for (int i = LootTierData.Num() - 1; i >= 0; --i)
	{
		if (LootTierData[i] && LootTierData[i]->Weight > 0.0f)
			return LootTierData[i];
	}

	return nullptr;
}

FFortLootTierData* FFortLootTierData::ChooseLootTierData(
	TArray<UDataTable*> LootTierDataTables,
	FName TierGroupName,
	int32 WorldLevel,
	int32 ForcedLootTier)
{
	if (LootTierDataTables.Num() == 0) {
		Log("FFortLootTierData::ChooseLootTierData: No LootTierDataTable(s) in array!");
		return nullptr;
	}

	TArray<FFortLootTierData*> TierGroupRows;

	for (int i = 0; i < LootTierDataTables.Num(); i++) {
		UDataTable* DataTable = LootTierDataTables[i];
		if (!DataTable)
			continue;

		TArray<FFortLootTierData*> Rows;
		DataTable->GetAllRows<FFortLootTierData>(Rows);

		for (int j = 0; j < Rows.Num(); j++) {
			FFortLootTierData* Row = Rows[j];
			if (!Row)
				continue;

			if (Row->TierGroup != TierGroupName)
				continue;

			if (Row->Weight <= 0)
				continue;

			const bool bMatchesWorldLevel =
				(WorldLevel == -1) ||
				((Row->MinWorldLevel == -1 || Row->MinWorldLevel <= WorldLevel) &&
					(Row->MaxWorldLevel == -1 || Row->MaxWorldLevel >= WorldLevel));

			if (!bMatchesWorldLevel)
				continue;

			if (ForcedLootTier != -1 && Row->LootTier != ForcedLootTier)
				continue;

			TierGroupRows.Add(Row);
		}
	}

	return ChooseLootTierData(TierGroupRows);
}

int32 FFortLootTierData::GetMinimumLootDrops() {
	int32 MinimumLootDrops = 0;
	if (LootPackageCategoryMinArray.Num())
	{
		for (int i = 0; i < LootPackageCategoryMinArray.Num(); i++)
		{
			if (LootPackageCategoryMinArray[i] > 0)
			{
				MinimumLootDrops += LootPackageCategoryMinArray[i];
			}
		}
	}

	return MinimumLootDrops;
}

int32 FFortLootTierData::GetTotalCategoryWeight() {
	int32 TotalCategoryWeight = 0;

	for (int i = 0; i < LootPackageCategoryWeightArray.Num(); i++)
	{
		auto CategoryWeight = LootPackageCategoryWeightArray[i];

		if (CategoryWeight > 0)
		{
			auto CategoryMax = LootPackageCategoryMaxArray[i];

			if (CategoryMax < 0)
			{
				TotalCategoryWeight += CategoryWeight;
			}
		}
	}

	return TotalCategoryWeight;
}

int32 FFortLootTierData::GetTotalCategoryMin() {
	int32 TotalCategoryMin = 0;
	for (int i = 0; i < LootPackageCategoryMinArray.Num(); i++)
	{
		auto CategoryMin = LootPackageCategoryMinArray[i];

		if (CategoryMin > 0)
		{
			auto CategoryMax = LootPackageCategoryMaxArray[i];

			if (CategoryMax < 0)
			{
				TotalCategoryMin += CategoryMin;
			}
		}
	}

	return TotalCategoryMin;
}

FFortLootTierData* FFortLootTierData::PickLootTierData(
	TArray<UDataTable*> LootTierDataTables,
	FName TierGroupName,
	int32 WorldLevel,
	int32 ForcedLootTier,
	int32 RecursionDepth)
{
	if (RecursionDepth >= 5)
	{
		return nullptr;
	}

	FFortLootTierData* LootTierData = FFortLootTierData::ChooseLootTierData(LootTierDataTables, TierGroupName, WorldLevel, ForcedLootTier);

	if (!LootTierData || !LootTierData->IsValid()) {
		return PickLootTierData(LootTierDataTables, TierGroupName, WorldLevel, ForcedLootTier, RecursionDepth + 1);
	}

	/*int32 TotalCategoryWeight = LootTierData->GetTotalCategoryWeight();
	int32 TotalCategoryMin = LootTierData->GetTotalCategoryMin();

	if (TotalCategoryWeight > TotalCategoryMin) {
		return PickLootTierData(LootTierDataTables, TierGroupName, WorldLevel, ForcedLootTier, RecursionDepth + 1);
	}*/

	return LootTierData;
}