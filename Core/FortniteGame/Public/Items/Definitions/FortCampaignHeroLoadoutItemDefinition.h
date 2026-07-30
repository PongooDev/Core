#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortProfileItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"

struct FFortCrewSlotInformation {
public:
	DefineUnrealStruct(FFortCrewSlotInformation);

	DefineStructProperty(FText, DisplayName);
	DefineStructProperty(FName, SlotName);
	DefineStructProperty(FGameplayTagContainer, SlotTags);
	DefineStructProperty(float, SlotStatContribution);
public:
	uint8 Padding[0x44];
};

class UFortCampaignHeroLoadoutItemDefinition : public UFortProfileItemDefinition {
public:
	DefineUnrealClass(UFortCampaignHeroLoadoutItemDefinition);

	DefineUProperty(TArray<FFortCrewSlotInformation>, CrewSlots);
	DefineUProperty(FGameplayTagQuery, CommanderSlotQuery);
	DefineUProperty(FGameplayTagQuery, SupportHeroSlotQuery);
	DefineUProperty(int32, GadgetSlotsAllowed);
	DefineUProperty(FName, SlotUnlockSquadName);
};
