#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/PrimaryAssetId.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "FortniteGame/Public/Items/Definitions/FortChallengeBundleItemDefinition.h"

class UDataTable;

class UMaterialInterface;
class UTexture2D;

struct FAthenaRewardScheduleLevel {
public:
	DefineUnrealStruct(FAthenaRewardScheduleLevel);

	DefineStructProperty(TArray<FAthenaRewardItemReference>, Rewards);
public:
	uint8 Padding[0x10];
};

struct FAthenaRewardSchedule {
public:
	DefineUnrealStruct(FAthenaRewardSchedule);

	DefineStructProperty(TArray<FAthenaRewardScheduleLevel>, Levels);
public:
	uint8 Padding[0x10];
};

struct FAthenaSeasonBannerLevel {
public:
	DefineUnrealStruct(FAthenaSeasonBannerLevel);

	DefineStructProperty(TSoftObjectPtr<UTexture2D>, SurroundImage);
	DefineStructProperty(TSoftObjectPtr<UMaterialInterface>, BannerMaterial);
public:
	uint8 Padding[0x50];
};

struct FAthenaSeasonBannerLevelSchedule {
public:
	DefineUnrealStruct(FAthenaSeasonBannerLevelSchedule);

	DefineStructProperty(TArray<FAthenaSeasonBannerLevel>, Levels);
public:
	uint8 Padding[0x10];
};

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
	DefineUProperty(FPrimaryAssetId, FreeTokenItemPrimaryAssetId);
	DefineUProperty(TArray<int32>, FreeLevelsThatNavigateToBattlePass);
	DefineUProperty(TArray<int32>, FreeLevelsThatAutoOpenTheAboutScreen);
	DefineUProperty(FAthenaRewardSchedule, SeasonXpScheduleFree);
	DefineUProperty(FGameplayTag, FreeSeasonItemContentTag);
	DefineUProperty(FAthenaRewardSchedule, BookXpScheduleFree);
	DefineUProperty(FGameplayTag, BattlePassFreeItemContentTag);
	DefineUProperty(FAthenaRewardSchedule, BookXpSchedulePaid);
	DefineUProperty(FGameplayTag, BattlePassPaidItemContentTag);
	DefineUProperty(FAthenaSeasonBannerLevelSchedule, SeasonBannerSchedule);
	DefineUProperty(FAthenaRewardScheduleLevel, SeasonGrantsToEveryone);
	DefineUProperty(FGameplayTag, SeasonGrantsToEveryoneItemContentTag);
	DefineUProperty(FAthenaRewardScheduleLevel, SeasonFirstWinRewards);
	DefineUProperty(FGameplayTag, SeasonFirstWinItemContentTag);
	DefineUProperty(FAthenaRewardScheduleLevel, BattleStarSubstitutionReward);
};
