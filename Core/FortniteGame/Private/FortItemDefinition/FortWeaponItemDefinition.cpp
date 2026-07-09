#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

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

int32 UFortWeaponItemDefinition::GetClipSize(int32 Level) const {
	FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
	if (!WeaponStats)
		return 1;

	return WeaponStats->GetClipSize(Level);
}

float UFortWeaponItemDefinition::GetDurability(int32 Level) const {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("UFortWeaponItemDefinition::GetDurability: World is null!");
		return 1;
	}
	
	if (AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>()) {
		return FLT_MAX;
	}

	if (Finder::FindUFortWeaponItemDefinition_GetMaxDurabilityVFT()) {
		float (*&GetMaxDurabilityInternal)(const UFortWeaponItemDefinition* This, int32 Level) = decltype(GetMaxDurabilityInternal)(VTable[Finder::FindUFortWeaponItemDefinition_GetMaxDurabilityVFT()]);
		return GetMaxDurabilityInternal(this, Level);
	}
	else {
		FFortBaseWeaponStats* WeaponStats = GetWeaponStats();
		if (!WeaponStats)
			return 1;

		return WeaponStats->GetDurability(Level, Rarity);
	}
}

TSubclassOf<AFortWeapon> UFortWeaponItemDefinition::GetWeaponActorClass() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetWeaponActorClass");

	return const_cast<UFortWeaponItemDefinition*>(this)->Call<TSubclassOf<AFortWeapon>>(Func);
}