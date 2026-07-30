#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UBuildingEditModeMetadata;
class UFortWeaponItemDefinition;
class ABuildingSMActor;

class AFortWeapon : public AActor {
public:
	DefineUnrealClass(AFortWeapon);

	DefineUProperty(UFortWeaponItemDefinition*, WeaponData);
	DefineUProperty(FGuid, ItemEntryGuid);
	DefineUProperty(int32, WeaponLevel);
};
