#pragma once
#include "pch.h"

#include "McpItemDefinitionBase.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortWeaponMeleeItemDefinition;

class UFortItemDefinition : public UMcpItemDefinitionBase {
public:
	DefineUnrealClass(UFortItemDefinition);

	DefineUProperty(FScalableFloat, MaxStackSize);

	DefineUProperty(uint8, ItemType);
	DefineUProperty(uint8, Rarity);
public:
	UFortItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const;

	int32 GetMaxStackSize() const;

	bool IsStackable() const;

	uint8 GetQuickBarForItem();
};

class UFortAccountItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortAccountItemDefinition);
};

class UAthenaCosmeticItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UAthenaCosmeticItemDefinition);
};

class UAthenaPickaxeItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaPickaxeItemDefinition);

	DefineUProperty(UFortWeaponMeleeItemDefinition*, WeaponDefinition);
};