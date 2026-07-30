#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UDataTable;

class EAthenaChallengeTabVisibility {
public:
	DefineUnrealEnum(EAthenaChallengeTabVisibility);

	DefineEnumProperty(Hide);
	DefineEnumProperty(ShowAlways);
};

class EAthenaSeasonShopVisibility {
public:
	DefineUnrealEnum(EAthenaSeasonShopVisibility);

	DefineEnumProperty(Hide);
	DefineEnumProperty(ShowIfOffersAvailable);
	DefineEnumProperty(ShowAlways);
};

class UAthenaSeasonItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UAthenaSeasonItemDefinition);

	DefineUProperty(int32, SeasonNumber);
	DefineUProperty(int32, NumSeasonLevels);
	DefineUProperty(int32, NumBookLevels);
	DefineUProperty(EAthenaSeasonShopVisibility, SeasonShopVisibility);
	DefineUProperty(EAthenaChallengeTabVisibility, ChallengesVisibility);
	DefineUProperty(UDataTable*, SeasonXpCurve);
	DefineUProperty(UDataTable*, BookXpCurve);
	DefineUProperty(FString, SeasonStorefront);
	DefineUProperty(FString, BattlePassLevelOfferID);
	DefineUProperty(TArray<int32>, FreeLevelsThatNavigateToBattlePass);
	DefineUProperty(TArray<int32>, FreeLevelsThatAutoOpenTheAboutScreen);
	DefineUProperty(FGameplayTag, FreeSeasonItemContentTag);
	DefineUProperty(FGameplayTag, BattlePassFreeItemContentTag);
	DefineUProperty(FGameplayTag, BattlePassPaidItemContentTag);
	DefineUProperty(FGameplayTag, SeasonGrantsToEveryoneItemContentTag);
	DefineUProperty(FGameplayTag, SeasonFirstWinItemContentTag);
};
