#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortAbilitySet;

class EFortAlteration {
public:
	DefineUnrealEnum(EFortAlteration);

	DefineEnumProperty(AttributeSlot);
	DefineEnumProperty(GameplaySlot);
	DefineEnumProperty(ComplexCosmeticSlot);
	DefineEnumProperty(UserPickedCosmeticSlot);
	DefineEnumProperty(ColorSlot);
	DefineEnumProperty(HeroSpecializationTier1Slot);
	DefineEnumProperty(HeroSpecializationTier2Slot);
	DefineEnumProperty(HeroSpecializationTier3Slot);
	DefineEnumProperty(HeroSpecializationTier4Slot);
	DefineEnumProperty(HeroSpecializationTier5Slot);
};

class UFortAlterationItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAlterationItemDefinition);

	DefineUProperty(EFortAlteration, AlterationType);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, AlterationAbilitySet);
};
