#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

enum class EFortPickupSourceTypeFlag : uint8
{
	Other = 0,
	Player = 1,
	Destruction = 2,
	Container = 4,
	AI = 8,
	Tossed = 16,
	FloorLoot = 32,
	Fishing = 64,
	NPCService = 128,
};

enum class EFortPickupSpawnSource : uint8
{
	Unset = 0,
	PlayerElimination = 1,
	Chest = 2,
	SupplyDrop = 3,
	AmmoBox = 4,
	Drone = 5,
	ItemSpawner = 6,
	BotElimination = 7,
	NPCElimination = 8,
	LootDrop = 9,
	TossedByPlayer = 10,
	NPC = 11,
	NPCGift = 12,
	CraftingBench = 13,
	VendingMachine = 14,
};

enum class EFortPickupTossState : uint8
{
	NotTossed = 0,
	InProgress = 1,
	AtRest = 2,
	EFortPickupTossState_MAX = 3,
};

enum class ESpecialActorStatType : uint8
{
	NumEliminationsNearby = 0,
	TimeInWorld = 1,
	PickupNumSpawns = 2,
	PickupNumDespawns = 3,
	PickupNumDropped = 4,
	PickupNumTaken = 5,
	PlayerWon = 6,
	PlayerNumEliminations = 7,
	PlayerNum = 8,
	TotalStats = 9,
	ESpecialActorStatType_MAX = 10,
};

class EFortQuickBars {
public:
	DefineUnrealEnum(EFortQuickBars);

	DefineEnumProperty(Primary);
	DefineEnumProperty(Secondary);
	DefineEnumProperty(Creative);

	DefineEnumProperty(Max_None);
};

class EFortStructuralGridEditMode {
public:
	DefineUnrealEnum(EFortStructuralGridEditMode);

	DefineEnumProperty(CanAdd);
	DefineEnumProperty(ExistingActor);
	DefineEnumProperty(NoStructuralSupport);
	DefineEnumProperty(InvalidActor);
	DefineEnumProperty(ReachedLimit);
	DefineEnumProperty(NoEditPermission);
	DefineEnumProperty(PatternNotPermittedByLayoutRequirement);
	DefineEnumProperty(ResourceTypeNotPermittedByLayoutRequirement);
	DefineEnumProperty(BuildingAtRequirementsDisabled);
	DefineEnumProperty(BuildingOtherThanRequirementsDisabled);
	DefineEnumProperty(EFortStructuralGridQueryResults_MAX);
};

class EFortBuildPreviewMarkerOptionalAdjustment {
public:
	DefineUnrealEnum(EFortBuildPreviewMarkerOptionalAdjustment);

	DefineEnumProperty(None);
	DefineEnumProperty(FreeWallPieceOnTop);
	DefineEnumProperty(FreeWallPieceOnBottom);
	DefineEnumProperty(EFortBuildPreviewMarkerOptionalAdjustment_MAX);
};

class EFortItemEntryState {
public:
	DefineUnrealEnum(EFortItemEntryState);

	DefineEnumProperty(NoneState);
	DefineEnumProperty(NewItemCount);
	DefineEnumProperty(ShouldShowItemToast);
};

enum class ELootQuotaLevel : uint8
{
	Unlimited = 0,
	Level1 = 1,
	Level2 = 2,
	Level3 = 3,
	Level4 = 4,
	Level5 = 5,
	Level6 = 6,
	Level7 = 7,
	Level8 = 8,
	Level9 = 9,
	Level10 = 10,
	Level11 = 11,
	Level12 = 12,
	Level13 = 13,
	Level14 = 14,
	Level15 = 15,
	Level16 = 16,
	Level17 = 17,
	NumLevels = 18,
};

enum class EFortInventoryType : uint8
{
	World = 0,
	Account = 1,
	Outpost = 2,
	MAX = 3,
};

enum EFortPlayerValidationType : uint8
{
	NotValidated = 0,
	ValidatedPlayer = 1,
	ValidatedSpectator = 2,
};

enum EContextCredentials : int32
{
	CXC_Client = 0,
	CXC_DedicatedServer = 1,
	CXC_Cheater = 2,
	CXC_Public = 3,
};

enum class EFortCustomPartType : uint8
{
	Head = 0,
	Body = 1,
	Hat = 2,
	Backpack = 3,
	Charm = 4,
	Face = 5,
	NumTypes = 6,
	EFortCustomPartType_MAX = 7,
};

class EBuildingReplacementType {
public:
	DefineUnrealEnum(EBuildingReplacementType);

	DefineEnumProperty(BRT_None);
	DefineEnumProperty(BRT_Edited);
	DefineEnumProperty(BRT_Conversion);
	DefineEnumProperty(BRT_MAX);
};

class EFortItemType
{
public:
	DefineUnrealEnum(EFortItemType);

	DefineEnumProperty(WeaponHarvest);
	DefineEnumProperty(WorldResource);
	DefineEnumProperty(EditTool);
	DefineEnumProperty(Trap);
	DefineEnumProperty(Ammo);
	DefineEnumProperty(BuildingPiece);
	DefineEnumProperty(Ingredient);
};

enum EAnalyticsProviderType : int32
{
	EPT_Client = 0,
    EPT_Server = 1,
    EPT_ServerGameInstance = 2,
    EPT_EsportsServerGameInstance = 3,
    EPT_MMRServerGameInstance = 4,
};