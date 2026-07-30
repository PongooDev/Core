#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Items/Definitions/FortProfileItemDefinition.h"

class UFortAbilityKit;

class EFortItemTier {
public:
	DefineUnrealEnum(EFortItemTier);

	DefineEnumProperty(No_Tier);
	DefineEnumProperty(I);
	DefineEnumProperty(II);
	DefineEnumProperty(III);
	DefineEnumProperty(IV);
	DefineEnumProperty(V);
	DefineEnumProperty(VI);
	DefineEnumProperty(VII);
	DefineEnumProperty(VIII);
	DefineEnumProperty(IX);
	DefineEnumProperty(X);
	DefineEnumProperty(NumItemTierValues);
};

class EFortRarity {
public:
	DefineUnrealEnum(EFortRarity);

	DefineEnumProperty(Handmade);
	DefineEnumProperty(Ordinary);
	DefineEnumProperty(Sturdy);
	DefineEnumProperty(Quality);
	DefineEnumProperty(Fine);
	DefineEnumProperty(Elegant);
	DefineEnumProperty(Masterwork);
	DefineEnumProperty(Epic);
	DefineEnumProperty(Badass);
	DefineEnumProperty(Legendary);
	DefineEnumProperty(NumRarityValues);
};

struct FFortTeamPerkLoadoutCondition {
public:
	DefineUnrealStruct(FFortTeamPerkLoadoutCondition);

	DefineStructProperty(int32, NumTimesSatisfiable);
	DefineStructProperty(FGameplayTagQuery, RequiredTagQuery);
	DefineStructProperty(EFortItemTier, MinimumHeroTier);
	DefineStructProperty(EFortItemTier, MaximumHeroTier);
	DefineStructProperty(int32, MinimumHeroLevel);
	DefineStructProperty(int32, MaximumHeroLevel);
	DefineStructProperty(EFortRarity, MinimumHeroRarity);
	DefineStructProperty(EFortRarity, MaximumHeroRarity);
public:
	uint8 Padding[0x62];
};

class UFortTeamPerkItemDefinition : public UFortProfileItemDefinition {
public:
	DefineUnrealClass(UFortTeamPerkItemDefinition);

	DefineUProperty(TSoftObjectPtr<UFortAbilityKit>, GrantedAbilityKit);
	DefineUProperty(bool, bProgressiveBonus);
	DefineUProperty(FGameplayTagQuery, RequiredCommanderTagQuery);
	DefineUProperty(FText, CommanderRequirementsText);
	DefineUProperty(TArray<FFortTeamPerkLoadoutCondition>, TeamPerkLoadoutConditions);
};
