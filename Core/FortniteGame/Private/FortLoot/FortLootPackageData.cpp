#include "pch.h"
#include "FortniteGame/Public/FortLoot/FortLootPackageData.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/FortLoot/FortLootTierData.h"

FFortLootPackageData* FFortLootPackageData::GetLootPackage(TArray<FFortLootPackageData*> LootPackages) {
	float TotalWeight = 0;

	for (auto Item : LootPackages)
		TotalWeight += Item->Weight;

	float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0, TotalWeight);

	FFortLootPackageData* SelectedItem = nullptr;

	for (auto Item : LootPackages)
	{
		if (RandomNumber <= Item->Weight)
		{
			SelectedItem = Item;
			break;
		}

		RandomNumber -= Item->Weight;
	}

	if (!SelectedItem)
		return GetLootPackage(LootPackages);

	return SelectedItem;
}

TArray<FFortLootPackageData*> FFortLootPackageData::GetLootPackages(TArray<UDataTable*> LootPackageDataTables, FFortLootTierData* LootTierData) {
	if (!LootTierData) {
		Log("FFortLootPackageData::GetLootPackages: LootTierData is null!");
		return TArray<FFortLootPackageData*>();
	}

	TArray<FFortLootPackageData*> TierGroupLootPackages;
	for (int p = 0; p < LootPackageDataTables.Num(); p++)
	{
		auto LP = LootPackageDataTables[p];
		auto& LPRowMap = LP->RowMap;

		for (int i = 0; i < LPRowMap.Elements.Num(); i++)
		{
			auto& CurrentLP = LPRowMap.Elements[i];
			auto LootPackage = (FFortLootPackageData*)CurrentLP.Value();

			if (!LootPackage)
				continue;

			if (LootPackage->LootPackageID == LootTierData->LootPackage && LootPackage->Weight != 0)
			{
				TierGroupLootPackages.Add(LootPackage);
			}
		}
	}

	return TierGroupLootPackages;
}

TArray<FFortLootPackageData*> FFortLootPackageData::GetCandidateLootPackages(TArray<UDataTable*>& LootPackageDataTables, TArray<FFortLootPackageData*> LootPackages, FString LootPackageID, FFortLootTierData* LootTierData) {
	TArray<FFortLootPackageData*> CandidateLootPackages;
	if (!LootTierData) {
		Log("FFortLootPackageData::GetCandidateLootPackages: LootTierData is null!");
		return CandidateLootPackages;
	}
	std::string LootPackageName = LootTierData->LootPackage.ToString().ToString();
	bool bIsWorldList = LootPackageName.contains("WorldList");

	if (bIsWorldList)
	{
		for (int i = 0; i < LootPackages.Num(); i++)
		{
			auto& CurrentLP = LootPackages[i];

			if (CurrentLP->Weight != 0) {
				CandidateLootPackages.Add(CurrentLP);
			}
		}
	}
	else
	{
		for (int i = 0; i < LootPackageDataTables.Num(); i++)
		{
			auto& LPRowMap = LootPackageDataTables[i]->RowMap;

			for (int j = 0; j < LPRowMap.Elements.Num(); j++)
			{
				auto& CurrentLP = LPRowMap.Elements[j];

				auto LootPackage = (FFortLootPackageData*)CurrentLP.Value();

				if (
					LootPackage->LootPackageID.ToString().ToString() == LootPackageID.ToString()
					&& LootPackage->Weight != 0)
				{
					CandidateLootPackages.Add(LootPackage);
				}
			}
		}
	}

	return CandidateLootPackages;
}

TArray<FFortItemEntry> FFortLootPackageData::GetLootItems(TArray<UDataTable*> LootPackageDataTables, FFortLootTierData* LootTierData) {
	if (!LootTierData) {
		Log("FFortLootPackageData::PickLootPackageData: LootTierData is null!");
		return TArray<FFortItemEntry>();
	}
	TArray<FFortItemEntry> LootItems;

	int32 MinimumLootDrops = LootTierData->GetMinimumLootDrops();
	float NumLootPackageDrops = std::floor(LootTierData->NumLootPackageDrops);

	TArray<FFortLootPackageData*> LootPackages = GetLootPackages(LootPackageDataTables, LootTierData);

	LootItems.Reserve(NumLootPackageDrops);
	for (float i = 0; i < NumLootPackageDrops; i++)
	{
		if (i >= LootPackages.Num())
			break;

		FFortLootPackageData* LootPackage = LootPackages[i];
		TArray<FFortLootPackageData*> CandidateLootPackages = GetCandidateLootPackages(LootPackageDataTables, LootPackages, LootPackage->LootPackageCall, LootTierData);
		if (CandidateLootPackages.Num() == 0) {
			NumLootPackageDrops++;
			continue;
		}

		FFortLootPackageData* PickedLootPackage = GetLootPackage(CandidateLootPackages);
		if (!PickedLootPackage) {
			Log("FFortLootPackageData::GetLootItems: Failed to pick loot package for loot package call '" + LootPackage->LootPackageCall.ToString() + "'!");
			continue;
		}

		UFortItemDefinition* ItemDefinition = PickedLootPackage->ItemDefinition.Get();
		if (!ItemDefinition) {
			Log("FFortLootPackageData::GetLootItems: Failed to load item definition for loot package '" + PickedLootPackage->LootPackageID.ToString().ToString() + "'!");
			NumLootPackageDrops++;
			continue;
		}

		FFortItemEntry LootDropEntry{};

		LootDropEntry.ItemDefinition = ItemDefinition;
		LootDropEntry.Count = PickedLootPackage->Count;

		LootItems.Add(LootDropEntry, FFortItemEntry::GetSize());
		Log("FFortLootPackageData::GetLootItems: Added loot drop '" + LootDropEntry.ItemDefinition->GetName().ToString() + "' with count " + std::to_string(LootDropEntry.Count) + " to loot items array!");
	}

	return LootItems;
}