#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Abilities/FortAttributeInitializationKey.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AttributeSet.h"

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

	// What a gadget's HUD readout actually comes from - the glider's charges and the jetpack's
	// fuel are gameplay attributes on AttributeSet, seeded from AttributeInitKey, and listed in
	// TrackedAttributes for the widget to read. Nothing in Core applies them yet.
	DefineUProperty(TSoftClassPtr<UClass>, AttributeSet);
	DefineUProperty(FFortAttributeInitializationKey, AttributeInitKey);
	DefineUProperty(TArray<FGameplayAttribute>, TrackedAttributes);
	DefineUProperty(TSoftObjectPtr<UFortDecoItemDefinition>, DecoItemDefinition);
	DefineUProperty(TSoftObjectPtr<UFortWeaponItemDefinition>, WeaponItemDefinition);

public:
	UFortWeaponItemDefinition* GetGadgetWeapon();
};
