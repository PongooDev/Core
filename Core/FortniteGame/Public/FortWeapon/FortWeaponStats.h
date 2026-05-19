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
public:
	uint8 Padding[0x118];
};

struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
public:
	DefineUnrealStruct(FFortRangedWeaponStats);
};