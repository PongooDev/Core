#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

#include "FortWorldItemDefinition.h"

class UBuildingEditModeMetadata;
class FFortRangedWeaponStats;
class FFortBaseWeaponStats;

class UFortWeaponItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortWeaponItemDefinition);

	DefineUProperty(FDataTableRowHandle, WeaponStatHandle);
public:
	FFortBaseWeaponStats* GetWeaponStats() const;

	FFortRangedWeaponStats* GetRangedWeaponStats() const;

	int32 GetClipSize() const;

	float GetDurability() const;
};

class UFortEditToolItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortEditToolItemDefinition);
};

class UFortBuildingItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortBuildingItemDefinition);

	DefineUProperty(TSoftObjectPtr<UBuildingEditModeMetadata>, BuildingMetaData);
};

class UFortDecoItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortDecoItemDefinition);
};

class UFortTrapItemDefinition : public UFortDecoItemDefinition {
public:
	DefineUnrealClass(UFortTrapItemDefinition);
};

class UFortWeaponMeleeItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortWeaponMeleeItemDefinition);
};

class UFortWeaponRangedItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortWeaponRangedItemDefinition);
};