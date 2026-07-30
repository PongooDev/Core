#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"

class UBuildingEditModeMetadata;
class FFortRangedWeaponStats;
class FFortBaseWeaponStats;
class AFortWeapon;


class UFortWeaponRangedItemDefinition : public UFortWeaponItemDefinition {
public:
	DefineUnrealClass(UFortWeaponRangedItemDefinition);
};
