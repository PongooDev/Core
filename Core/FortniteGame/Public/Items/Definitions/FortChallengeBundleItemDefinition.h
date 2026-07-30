#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

class UFortItemDefinition;
class UObject;

struct FFortGiftBoxFortmatData {
public:
	DefineUnrealStruct(FFortGiftBoxFortmatData);

	DefineStructProperty(FString, StringAssetType);
	DefineStructProperty(FString, StringData);
public:
	uint8 Padding[0x20];
};

struct FChallengeGiftBoxData {
public:
	DefineUnrealStruct(FChallengeGiftBoxData);

	DefineStructProperty(TArray<FFortGiftBoxFortmatData>, GiftBoxFormatData);
public:
	uint8 Padding[0x38];
};

class EAthenaRewardItemType {
public:
	DefineUnrealEnum(EAthenaRewardItemType);

	DefineEnumProperty(Normal);
	DefineEnumProperty(HiddenReward);
	DefineEnumProperty(NonExportedFakeReward);
};

struct FAthenaRewardItemReference {
public:
	DefineUnrealStruct(FAthenaRewardItemReference);

	DefineStructProperty(TSoftObjectPtr<UFortItemDefinition>, ItemDefinition);
	DefineStructProperty(FString, TemplateId);
	DefineStructProperty(int32, Quantity);
	DefineStructProperty(FChallengeGiftBoxData, RewardGiftBox);
	DefineStructProperty(bool, IsChaseReward);
	DefineStructProperty(EAthenaRewardItemType, RewardType);
public:
	uint8 Padding[0x7A];
};

struct FFortChallengeBundleRewards {
public:
	DefineUnrealStruct(FFortChallengeBundleRewards);

	DefineStructProperty(int32, CompletionCount);
	DefineStructProperty(TArray<FAthenaRewardItemReference>, Rewards);
public:
	uint8 Padding[0x18];
};

struct FFortChallengeSetStyle {
public:
	DefineUnrealStruct(FFortChallengeSetStyle);

	DefineStructProperty(FLinearColor, PrimaryColor);
	DefineStructProperty(FLinearColor, SecondaryColor);
	DefineStructProperty(FLinearColor, AccentColor);
	DefineStructProperty(TSoftObjectPtr<UObject>, DisplayImage);
	DefineStructProperty(TSoftObjectPtr<UObject>, CustomBackground);
public:
	uint8 Padding[0x80];
};

class UFortChallengeBundleItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortChallengeBundleItemDefinition);

	DefineUProperty(FString, CalendarEventTag);
	DefineUProperty(FString, CalendarEventName);
	DefineUProperty(TArray<FFortChallengeBundleRewards>, BundleCompletionRewards);
	DefineUProperty(FFortChallengeSetStyle, DisplayStyle);
	DefineUProperty(FAthenaRewardItemReference, OverrideRewardItem);
	DefineUProperty(int32, MaxChainDepth);
};
