#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortWorldItemDefinition.h"

class UBuildingEditModeMetadata;
class FFortRangedWeaponStats;

class UFortWeaponItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortWeaponItemDefinition);

	DefineUProperty(FDataTableRowHandle, WeaponStatHandle);
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

	FFortRangedWeaponStats* GetRangedWeaponStats() const;

	int32 GetClipSize() const;
};