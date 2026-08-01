#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

class UFortAbilitySet;
class UCustomCharacterPart;
class UFortWeaponItemDefinition;
class UFortDecoItemDefinition;

class UFortGadgetItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortGadgetItemDefinition);

	DefineBitfieldUProperty(bDropAllOnEquip);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, AbilitySet);
	DefineUProperty(TArray<UCustomCharacterPart*>, CharacterParts);
	DefineUProperty(TSoftClassPtr<UClass>, GameplayAbility);
	DefineUProperty(TSoftObjectPtr<UFortDecoItemDefinition>, DecoItemDefinition);
	DefineUProperty(TSoftObjectPtr<UFortWeaponItemDefinition>, WeaponItemDefinition);

public:
	UFortWeaponItemDefinition* GetGadgetWeapon();
};
