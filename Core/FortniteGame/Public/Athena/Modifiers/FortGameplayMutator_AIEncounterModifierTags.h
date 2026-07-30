#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Athena/Modifiers/FortGameplayMutator.h"

class AFortGameplayMutator_AIEncounterModifierTags : public AFortGameplayMutator {
public:
	DefineUnrealClass(AFortGameplayMutator_AIEncounterModifierTags);

	DefineUProperty(FGameplayTagContainer, GameplayTags);
};
