#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

class UBuildingEditModeMetadata;
class FFortRangedWeaponStats;
class FFortBaseWeaponStats;
class AFortWeapon;

class UFortWeaponItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortWeaponItemDefinition);

	DefineUProperty(FDataTableRowHandle, WeaponStatHandle);
public:
	FFortBaseWeaponStats* GetWeaponStats() const;

	FFortRangedWeaponStats* GetRangedWeaponStats() const;

	int32 GetClipSize(int32 Level = 0) const;

	float GetDurability(int32 Level = 0) const;

	TSubclassOf<AFortWeapon> GetWeaponActorClass() const;
};
