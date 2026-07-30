#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Weapons/FortWeapon.h"

class UBuildingEditModeMetadata;
class UFortWeaponItemDefinition;
class ABuildingSMActor;


class AFortWeap_BuildingToolBase : public AFortWeapon {
public:
	DefineUnrealClass(AFortWeap_BuildingToolBase);
};
