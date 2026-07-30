#pragma once
#include "pch.h"

#include "FortniteGame/Public/McpItemDefinitionBase.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/Quests/FortMcpQuestObjectiveInfo.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortWeaponMeleeItemDefinition;
class USkeletalMesh;
class UAnimInstance;
class USoundBase;

class EFortItemType
{
public:
	DefineUnrealEnum(EFortItemType);

	DefineEnumProperty(WorldItem);
	DefineEnumProperty(Ammo);
	DefineEnumProperty(Badge);
	DefineEnumProperty(BackpackPickup);
	DefineEnumProperty(BuildingPiece);
	DefineEnumProperty(CharacterPart);
	DefineEnumProperty(Consumable);
	DefineEnumProperty(Deco);
	DefineEnumProperty(EditTool);
	DefineEnumProperty(Ingredient);
	DefineEnumProperty(Gadget);
	DefineEnumProperty(HomebaseGadget);
	DefineEnumProperty(HeroAbility);
	DefineEnumProperty(MissionItem);
	DefineEnumProperty(Trap);
	DefineEnumProperty(Weapon);
	DefineEnumProperty(WeaponMelee);
	DefineEnumProperty(WeaponRanged);
	DefineEnumProperty(WeaponHarvest);
	DefineEnumProperty(WorldResource);
	DefineEnumProperty(AccountItem);
	DefineEnumProperty(AccountResource);
	DefineEnumProperty(CollectedResource);
	DefineEnumProperty(Alteration);
	DefineEnumProperty(CardPack);
	DefineEnumProperty(CharacterCosmetic);
	DefineEnumProperty(Currency);
	DefineEnumProperty(Hero);
	DefineEnumProperty(Schematic);
	DefineEnumProperty(Worker);
	DefineEnumProperty(Token);
	DefineEnumProperty(DailyRewardScheduleToken);
	DefineEnumProperty(CodeToken);
	DefineEnumProperty(Buff);
	DefineEnumProperty(BuffCredit);
	DefineEnumProperty(Quest);
	DefineEnumProperty(Compendium);
	DefineEnumProperty(CompendiumBundle);
	DefineEnumProperty(GameplayModifier);
	DefineEnumProperty(Outpost);
	DefineEnumProperty(HomebaseNode);
	DefineEnumProperty(Defender);
	DefineEnumProperty(ConversionControl);
	DefineEnumProperty(DeployableBaseCloudSave);
	DefineEnumProperty(ConsumableAccountItem);
	DefineEnumProperty(Quota);
	DefineEnumProperty(Expedition);
	DefineEnumProperty(HomebaseBannerIcon);
	DefineEnumProperty(HomebaseBannerColor);
	DefineEnumProperty(AthenaGlider);
	DefineEnumProperty(AthenaPickaxe);
	DefineEnumProperty(AthenaHat);
	DefineEnumProperty(AthenaBackpack);
	DefineEnumProperty(AthenaCharacter);
	DefineEnumProperty(AthenaDance);
	DefineEnumProperty(AthenaConsumableEmote);
	DefineEnumProperty(AthenaLoadingScreen);
	DefineEnumProperty(AthenaBattleBus);
	DefineEnumProperty(AthenaVictoryPose);
	DefineEnumProperty(AthenaMedal);
	DefineEnumProperty(SpecialItem);
	DefineEnumProperty(Emote);
	DefineEnumProperty(Stack);
	DefineEnumProperty(CollectionBookPage);
	DefineEnumProperty(Profile);
	DefineEnumProperty(Max_None);
};

class UFortItemDefinition : public UMcpItemDefinitionBase {
public:
	DefineUnrealClass(UFortItemDefinition);

	DefineUProperty(FScalableFloat, MaxStackSize);

	DefineUProperty(EFortItemType, ItemType);
	DefineUProperty(uint8, Rarity);

	DefineBitfieldUProperty(bForceAutoPickup);
public:
	UFortItem* CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const;

	int32 GetMaxStackSize() const;

	bool IsStackable() const;

	uint8 GetQuickBarForItem();
};
