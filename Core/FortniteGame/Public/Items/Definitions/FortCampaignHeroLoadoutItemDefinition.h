#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortProfileItemDefinition.h"

class UFortCampaignHeroLoadoutItemDefinition : public UFortProfileItemDefinition {
public:
	DefineUnrealClass(UFortCampaignHeroLoadoutItemDefinition);

	DefineUProperty(FGameplayTagQuery, CommanderSlotQuery);
	DefineUProperty(FGameplayTagQuery, SupportHeroSlotQuery);
	DefineUProperty(int32, GadgetSlotsAllowed);
	DefineUProperty(FName, SlotUnlockSquadName);
};
