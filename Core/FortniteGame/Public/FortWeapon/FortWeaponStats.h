#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

class ABuildingSMActor;
class UDataTable;

struct FFortBaseWeaponStats : public FTableRowBase {
public:
	DefineUnrealStruct(FFortBaseWeaponStats);

	DefineStructProperty(int32, BaseLevel);
	DefineStructProperty(int32, ClipSize);
	DefineStructProperty(float, ClipScale);
	DefineStructProperty(UDataTable*, Durability);
	DefineStructProperty(FName, DurabilityRowName);
public:
	uint8 Padding[0x130];
public:
	int32 GetClipSize(int32 WeaponLevel);

	float GetDurability(int32 WeaponLevel, uint8 Rarity);
};

struct FFortRangedWeaponStats : public FFortBaseWeaponStats {
public:
	DefineUnrealStruct(FFortRangedWeaponStats);
};