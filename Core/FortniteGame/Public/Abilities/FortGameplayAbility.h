#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EGameplayAbilityInstancingPolicy {
public:
	DefineUnrealEnum(EGameplayAbilityInstancingPolicy);

	DefineEnumProperty(NonInstanced);
	DefineEnumProperty(InstancedPerActor);
	DefineEnumProperty(InstancedPerExecution);
};

class UFortGameplayAbility : public UGameplayAbility {
public:
	DefineUnrealClass(UFortGameplayAbility);
};

class UGAB_Emote_Generic_C : public UFortGameplayAbility {
public:
	DefineUnrealClass(UGAB_Emote_Generic_C);
};
