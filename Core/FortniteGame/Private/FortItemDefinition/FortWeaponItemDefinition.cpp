#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

FFortRangedWeaponStats* UFortWeaponRangedItemDefinition::GetRangedWeaponStats() const {
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

	Log("UFortWeaponRangedItemDefinition::GetRangedWeaponStats: Row not found in DataTable or PackageData is null.");
	return nullptr;
}

int32 UFortWeaponRangedItemDefinition::GetClipSize() const {
	FFortRangedWeaponStats* RangedWeaponStats = GetRangedWeaponStats();
	if (!RangedWeaponStats)
		return 1;

	return RangedWeaponStats->ClipSize;
}