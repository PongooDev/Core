#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortProfileItemDefinition.h"

class UFortAbilityKit;

class UFortTeamPerkItemDefinition : public UFortProfileItemDefinition {
public:
	DefineUnrealClass(UFortTeamPerkItemDefinition);

	DefineUProperty(TSoftObjectPtr<UFortAbilityKit>, GrantedAbilityKit);
	DefineUProperty(bool, bProgressiveBonus);
	DefineUProperty(FGameplayTagQuery, RequiredCommanderTagQuery);
	DefineUProperty(FText, CommanderRequirementsText);
};
