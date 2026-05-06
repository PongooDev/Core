#include "pch.h"
#include "FortniteGame/Public/FortLoot/FortLootTierData.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

bool FFortLootTierData::IsValid() {
	if (!this) {
		return false;
	}

	if (NumLootPackageDrops <= 0) {
		return false;
	}

	if (LootPackageCategoryMinArray.Num() != LootPackageCategoryWeightArray.Num() ||
		LootPackageCategoryMinArray.Num() != LootPackageCategoryMaxArray.Num()) {
		return false;
	}

	return true;
}

FFortLootTierData* FFortLootTierData::ChooseLootTierData(TArray<FFortLootTierData*>& LootTierData) {
	if (LootTierData.Num() == 0) {
		return nullptr;
	}
	float TotalWeight = 0;

	for (auto Item : LootTierData)
		TotalWeight += Item->Weight;

	float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);

	FFortLootTierData* SelectedItem = nullptr;

	if (LootTierData.Num() == 1) {
		SelectedItem = LootTierData[0];
		return SelectedItem;
	}
	else {
		for (auto Item : LootTierData)
		{
			if (Item->Weight == 0) continue;
			if (RandomNumber <= Item->Weight)
			{
				SelectedItem = Item;
				break;
			}

			RandomNumber -= Item->Weight;
		}
	}

	if (!SelectedItem) {
		Log("FFortLootTierData::ChooseLootTierData: No item was selected, repicking...");
		return ChooseLootTierData(LootTierData);
	}

	return SelectedItem;
}

FFortLootTierData* FFortLootTierData::ChooseLootTierData(TArray<UDataTable*> LootTierDataTables, FName TierGroupName, int32 WorldLevel, int32 ForcedLootTier) {
	if (LootTierDataTables.Num() == 0) {
		Log("FFortLootTierData::ChooseLootTierData: No LootTierDataTable(s) in array!");
		return nullptr;
	}

	TArray<FFortLootTierData*> TierGroupRows;
	for (int i = 0; i < LootTierDataTables.Num(); i++) {
		UDataTable* DataTable = LootTierDataTables[i];
		if (DataTable) {
			TArray<FFortLootTierData*> Rows;
			DataTable->GetAllRows<FFortLootTierData>(Rows);
			for (int j = 0; j < Rows.Num(); j++) {
				FFortLootTierData* Row = Rows[j];
				if (Row
					&& Row->TierGroup == TierGroupName
					/*&& Row->Weight != 0*/
					&& (
						(Row->MinWorldLevel == -1 || Row->MaxWorldLevel == -1)
						|| (Row->MinWorldLevel <= WorldLevel && Row->MaxWorldLevel >= WorldLevel)
						)
					&& (Row->LootTier == ForcedLootTier || ForcedLootTier == -1)) {
					TierGroupRows.Add(Row);
				}
			}
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
	int32 RecursionDepth,
	int32 WorldLevel,
	int32 ForcedLootTier) {
	if (RecursionDepth >= 5)
	{
		return nullptr;
	}

	FFortLootTierData* LootTierData = FFortLootTierData::ChooseLootTierData(LootTierDataTables, TierGroupName, WorldLevel, ForcedLootTier);
	if (!LootTierData->IsValid()) {
		return PickLootTierData(LootTierDataTables, TierGroupName, ++RecursionDepth, WorldLevel, ForcedLootTier);
	}

	int32 TotalCategoryWeight = LootTierData->GetTotalCategoryWeight();
	int32 TotalCategoryMin = LootTierData->GetTotalCategoryMin();
	if (TotalCategoryWeight > TotalCategoryMin) {
		Log("FFortLootTierData::PickLootTierData: TotalCategoryWeight (" + std::to_string(TotalCategoryWeight) + ") is greater than TotalCategoryMin (" + std::to_string(TotalCategoryMin) + ") for picked LootTierData, repicking...");
		return PickLootTierData(LootTierDataTables, TierGroupName, ++RecursionDepth, WorldLevel, ForcedLootTier);
	}

	return LootTierData;
}