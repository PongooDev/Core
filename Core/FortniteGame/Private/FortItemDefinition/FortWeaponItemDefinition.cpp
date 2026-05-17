#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

FFortBaseWeaponStats* UFortWeaponItemDefinition::GetWeaponStats() const {
	if (!WeaponStatHandle.DataTable)
		return nullptr;

	auto DataTable = WeaponStatHandle.DataTable;
	auto RowName = WeaponStatHandle.RowName;

	auto& RowMap = DataTable->RowMap;

	for (int i = 0; i < RowMap.Num(); i++)
	{
		auto& Pair = RowMap[i];

		FName CurrentRowName = Pair.Key();
		FFortRangedWeaponStats* PackageData = (FFortRangedWeaponStats*)Pair.Value();

		if (CurrentRowName == RowName && PackageData)
		{
			return PackageData;
		}
	}

	Log("UFortWeaponItemDefinition::GetWeaponStats: Row not found in DataTable or PackageData is null.");
	return nullptr;
}

FFortRangedWeaponStats* UFortWeaponItemDefinition::GetRangedWeaponStats() const {
	if (!IsA(UFortWeaponRangedItemDefinition::StaticClass()))
		return nullptr;

	return (FFortRangedWeaponStats*)GetWeaponStats();
}

int32 UFortWeaponItemDefinition::GetClipSize() const {
	FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
	if (!WeaponStats)
		return 1;

	return WeaponStats->ClipSize;
}

float UFortWeaponItemDefinition::GetDurability() const {
	FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
	if (!WeaponStats)
		return 0.f;

	auto DataTable = WeaponStats->Durability;
	if (!DataTable)
		return 0.f;

	auto RowName = WeaponStats->DurabilityRowName;

	auto& RowMap = DataTable->RowMap;

	for (int i = 0; i < RowMap.Num(); i++)
	{
		auto& Pair = RowMap[i];

		FName CurrentRowName = Pair.Key();
		float* Durability = (float*)Pair.Value();

		if (CurrentRowName == RowName && Durability)
		{
			return *Durability;
		}
	}

	Log("UFortWeaponItemDefinition::GetDurability: Row not found in DataTable or Durability is null.");
	return 0.f;
}