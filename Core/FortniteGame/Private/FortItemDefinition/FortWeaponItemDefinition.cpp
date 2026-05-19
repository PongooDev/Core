#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

FFortBaseWeaponStats* UFortWeaponItemDefinition::GetWeaponStats() const {
	FFortBaseWeaponStats WeaponStats;
	if (UFortKismetLibrary::GetWeaponStatsRow(WeaponStatHandle, &WeaponStats)) {
		return &WeaponStats;
	}

	Log("UFortWeaponItemDefinition::GetWeaponStats: GetWeaponStatsRow Failed!");
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
	return 1.f;
}