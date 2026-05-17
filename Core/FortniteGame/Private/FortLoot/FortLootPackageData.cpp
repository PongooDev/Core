#include "pch.h"
#include "FortniteGame/Public/FortLoot/FortLootPackageData.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/FortLoot/FortLootTierData.h"

bool FFortLootPackageData::DoesLootPackageMatchWorldLevel(int32 WorldLevel)
{
	if (WorldLevel == -1)
		return true;

	return
		(MinWorldLevel == -1 || MinWorldLevel <= WorldLevel) &&
		(MaxWorldLevel == -1 || MaxWorldLevel >= WorldLevel);
}

FFortLootPackageData* FFortLootPackageData::GetLootPackage(TArray<FFortLootPackageData*> LootPackages)
{
	if (LootPackages.Num() == 0)
		return nullptr;

	float TotalWeight = 0.0f;

	for (auto* Item : LootPackages)
	{
		if (Item && Item->Weight > 0.0f)
			TotalWeight += Item->Weight;
	}

	if (TotalWeight <= 0.0f)
		return nullptr;

	float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

	for (auto* Item : LootPackages)
	{
		if (!Item || Item->Weight <= 0.0f)
			continue;

		if (RandomNumber <= Item->Weight)
			return Item;

		RandomNumber -= Item->Weight;
	}

	for (int i = LootPackages.Num() - 1; i >= 0; --i)
	{
		if (LootPackages[i] && LootPackages[i]->Weight > 0.0f)
			return LootPackages[i];
	}

	return nullptr;
}

TArray<FFortLootPackageData*> FFortLootPackageData::GetLootPackages(
	TArray<UDataTable*> LootPackageDataTables,
	FFortLootTierData* LootTierData,
	int32 WorldLevel)
{
	if (!LootTierData) {
		Log("FFortLootPackageData::GetLootPackages: LootTierData is null!");
		return TArray<FFortLootPackageData*>();
	}

	TArray<FFortLootPackageData*> TierGroupLootPackages;

	for (int p = 0; p < LootPackageDataTables.Num(); p++)
	{
		auto LP = LootPackageDataTables[p];
		if (!LP)
			continue;

		auto& LPRowMap = LP->RowMap;

		for (int i = 0; i < LPRowMap.Elements.Num(); i++)
		{
			auto& CurrentLP = LPRowMap.Elements[i];
			auto LootPackage = (FFortLootPackageData*)CurrentLP.Value();

			if (!LootPackage)
				continue;

			if (LootPackage->Weight == 0)
				continue;

			if (LootPackage->LootPackageID != LootTierData->LootPackage)
				continue;

			if (!LootPackage->DoesLootPackageMatchWorldLevel(WorldLevel))
				continue;

			TierGroupLootPackages.Add(LootPackage);
		}
	}

	return TierGroupLootPackages;
}

TArray<FFortLootPackageData*> FFortLootPackageData::GetCandidateLootPackages(
	TArray<UDataTable*>& LootPackageDataTables,
	TArray<FFortLootPackageData*> LootPackages,
	FString LootPackageID,
	FFortLootTierData* LootTierData,
	int32 WorldLevel)
{
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
			auto* CurrentLP = LootPackages[i];
			if (!CurrentLP || CurrentLP->Weight == 0)
				continue;

			if (!CurrentLP->DoesLootPackageMatchWorldLevel(WorldLevel))
				continue;

			if (CurrentLP->LootPackageID.ToString().ToString() == LootPackageID.ToString())
			{
				if (CurrentLP->ItemDefinition.Get()) {
					CandidateLootPackages.Add(CurrentLP);
				}
				else if (!CurrentLP->LootPackageCall.ToString().empty()) {
					TArray<FFortLootPackageData*> SubCandidateLootPackages =
						GetCandidateLootPackages(LootPackageDataTables, LootPackages, CurrentLP->LootPackageCall.ToString(), LootTierData, WorldLevel);

					for (int j = 0; j < SubCandidateLootPackages.Num(); j++)
					{
						auto* SubCandidateLP = SubCandidateLootPackages[j];
						if (SubCandidateLP && SubCandidateLP->Weight != 0)
							CandidateLootPackages.Add(SubCandidateLP);
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < LootPackageDataTables.Num(); i++)
		{
			if (!LootPackageDataTables[i])
				continue;

			auto& LPRowMap = LootPackageDataTables[i]->RowMap;

			for (int j = 0; j < LPRowMap.Elements.Num(); j++)
			{
				auto& CurrentLP = LPRowMap.Elements[j];
				auto* LootPackage = (FFortLootPackageData*)CurrentLP.Value();

				if (!LootPackage || LootPackage->Weight == 0)
					continue;

				if (!LootPackage->DoesLootPackageMatchWorldLevel(WorldLevel))
					continue;

				if (LootPackage->LootPackageID.ToString().ToString() == LootPackageID.ToString())
				{
					if (LootPackage->ItemDefinition.Get()) {
						CandidateLootPackages.Add(LootPackage);
					}
					else if (!LootPackage->LootPackageCall.ToString().empty()) {
						TArray<FFortLootPackageData*> SubCandidateLootPackages =
							GetCandidateLootPackages(LootPackageDataTables, LootPackages, LootPackage->LootPackageCall.ToString(), LootTierData, WorldLevel);

						for (int z = 0; z < SubCandidateLootPackages.Num(); z++)
						{
							auto* SubCandidateLP = SubCandidateLootPackages[z];
							if (SubCandidateLP && SubCandidateLP->Weight != 0)
								CandidateLootPackages.Add(SubCandidateLP);
						}
					}
				}
			}
		}
	}

	return CandidateLootPackages;
}

TArray<FFortItemEntry> FFortLootPackageData::GetLootItems(
	TArray<UDataTable*> LootPackageDataTables,
	FFortLootTierData* LootTierData,
	int32 WorldLevel)
{
	if (!LootTierData) {
		Log("FFortLootPackageData::PickLootPackageData: LootTierData is null!");
		return TArray<FFortItemEntry>();
	}

	TArray<FFortItemEntry> LootItems;

	int32 MinimumLootDrops = LootTierData->GetMinimumLootDrops();

	float RawDrops = LootTierData->NumLootPackageDrops;
	int32 NumLootPackageDrops = (int32)std::floor(RawDrops);
	float FractionalPart = RawDrops - NumLootPackageDrops;

	if (UKismetMathLibrary::RandomFloat() < FractionalPart)
	{
		NumLootPackageDrops++;
	}

	TArray<FFortLootPackageData*> LootPackages = GetLootPackages(LootPackageDataTables, LootTierData, WorldLevel);

	LootItems.Reserve(NumLootPackageDrops);
	for (int32 i = 0; i < NumLootPackageDrops; i++)
	{
		if (i >= LootPackages.Num())
			break;

		FFortLootPackageData* LootPackage = LootPackages[i];
		if (!LootPackage)
			continue;

		FString LookupPackageID = LootPackage->LootPackageID.ToString();
		if (!LootPackage->LootPackageCall.ToString().empty())
		{
			LookupPackageID = LootPackage->LootPackageCall;
		}

		TArray<FFortLootPackageData*> CandidateLootPackages =
			GetCandidateLootPackages(LootPackageDataTables, LootPackages, LookupPackageID, LootTierData, WorldLevel);

		if (CandidateLootPackages.Num() == 0) {
			NumLootPackageDrops++;
			continue;
		}

		FFortLootPackageData* PickedLootPackage = GetLootPackage(CandidateLootPackages);
		if (!PickedLootPackage) {
			Log("FFortLootPackageData::GetLootItems: Failed to pick loot package for loot package call '" + LootPackage->LootPackageCall.ToString() + "'!");
			continue;
		}

		if (PickedLootPackage->Count <= 0) {
			NumLootPackageDrops++;
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
	}

	return LootItems;
}