#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

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

enum class EFortQuickBars : uint8
{
	Primary = 0,
	Secondary = 1,
	Creative = 2,
	Max_None = 3,
	EFortQuickBars_MAX = 4,
};

enum class EFortStructuralGridQueryResults : uint8
{
	CanAdd = 0,
	ExistingActor = 1,
	Obstructed = 2,
	NoStructuralSupport = 3,
	InvalidActor = 4,
	ReachedLimit = 5,
	NoEditPermission = 6,
	PatternNotPermittedByLayoutRequirement = 7,
	ResourceTypeNotPermittedByLayoutRequirement = 8,
	BuildingAtRequirementsDisabled = 9,
	BuildingOtherThanRequirementsDisabled = 10,
	EFortStructuralGridQueryResults_MAX = 11,
};

enum class EFortBuildPreviewMarkerOptionalAdjustment : uint8
{
	None = 0,
	FreeWallPieceOnTop = 1,
	FreeWallPieceOnBottom = 2,
	EFortBuildPreviewMarkerOptionalAdjustment_MAX = 3,
};

enum class EFortItemEntryState : uint8
{
	NoneState = 0,
	NewItemCount = 1,
	ShouldShowItemToast = 2,
	DurabilityInitialized = 3,
	DoNotShowSpawnParticles = 4,
	FromRecoveredBackpack = 5,
	FromGift = 6,
	PendingUpgradeCriteriaProgress = 7,
	OwnerBuildingHandle = 8,
	FromDroppedPickup = 9,
	JustCrafted = 10,
	CraftAndSlotTarget = 11,
	GenericAttributeValueSet = 12,
	PickupInstigatorHandle = 13,
	RechargingWeaponServerTime = 14,
	DisallowSwapOnNextPickUpAttempt = 15,
	DroppedFromQuestSource = 16,
	Tossed = 17,
	Loaded = 18,
	SaveMagazineAmmo = 19,
	MultiItemLevel = 20,
	MultiItemXP = 21,
	FromAugment = 22,
	PreventDropping = 23,
	PreventDroppingExceptFromDeath = 24,
	PreventDroppingExceptFromDeathWithCount = 25,
	ReAddedItem = 26,
	WeaponAmmoOverride = 27,
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