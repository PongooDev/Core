#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class UFortGameplayAbility : public UGameplayAbility {
public:
	DefineUnrealClass(UFortGameplayAbility);
};

class UGAB_Emote_Generic_C : public UFortGameplayAbility {
public:
	DefineUnrealClass(UGAB_Emote_Generic_C);
};