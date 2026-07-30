#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Athena/Modifiers/FortGameplayMutator.h"
#include "FortniteGame/Public/Missions/FortObjectiveBase.h"

class AFortGameplayMutator_AIEncounterModifierTags : public AFortGameplayMutator {
public:
	DefineUnrealClass(AFortGameplayMutator_AIEncounterModifierTags);

	DefineUProperty(FGameplayTagContainer, GameplayTags);
	DefineUProperty(EFortMissionType, RequiredEncounterAssociatedMissionType);
};
