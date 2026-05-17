#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

class ABuildingSMActor;
class UDataTable;

struct FFortBaseWeaponStats : public FTableRowBase {
public:
	DefineUnrealStruct(FFortBaseWeaponStats);

	DefineStructProperty(int32, ClipSize);
	DefineStructProperty(UDataTable*, Durability);
	DefineStructProperty(FName, DurabilityRowName);
};

struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
public:
	DefineUnrealStruct(FFortRangedWeaponStats);
};