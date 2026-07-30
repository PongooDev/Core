#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

enum class EAthenaGamePhase : uint8 {
	None = 0,
	Setup = 1,
	Warmup = 2,
	Aircraft = 3,
	SafeZones = 4,
	EndGame = 5,
	Count = 6,
};

enum class EAthenaGamePhaseStep : uint8 {
	None = 0,
	Setup = 1,
	Warmup = 2,
	GetReady = 3,
	BusLocked = 4,
	BusFlying = 5,
	StormForming = 6,
	StormHolding = 7,
	StormShrinking = 8,
	Countdown = 9,
	FinalCountdown = 10,
	EndGame = 11,
	Count = 12,
};

class EFortPickupSourceTypeFlag {
public:
	DefineUnrealEnum(EFortPickupSourceTypeFlag);

	DefineEnumProperty(Other);
	DefineEnumProperty(Player);
	DefineEnumProperty(Destruction);
	DefineEnumProperty(Container);
	DefineEnumProperty(AI);
	DefineEnumProperty(Tossed);
	DefineEnumProperty(FloorLoot);
	DefineEnumProperty(Fishing);
	DefineEnumProperty(NPCService);
};

class EFortPickupSpawnSource {
public:
	DefineUnrealEnum(EFortPickupSpawnSource);

	DefineEnumProperty(Unset);
	DefineEnumProperty(PlayerElimination);
	DefineEnumProperty(Chest);
	DefineEnumProperty(SupplyDrop);
	DefineEnumProperty(AmmoBox);
	DefineEnumProperty(Drone);
	DefineEnumProperty(ItemSpawner);
	DefineEnumProperty(BotElimination);
	DefineEnumProperty(NPCElimination);
	DefineEnumProperty(LootDrop);
	DefineEnumProperty(TossedByPlayer);
	DefineEnumProperty(NPC);
	DefineEnumProperty(NPCGift);
	DefineEnumProperty(CraftingBench);
	DefineEnumProperty(VendingMachine);
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
};

class EFortBuildPreviewMarkerOptionalAdjustment {
public:
	DefineUnrealEnum(EFortBuildPreviewMarkerOptionalAdjustment);

	DefineEnumProperty(None);
	DefineEnumProperty(FreeWallPieceOnTop);
	DefineEnumProperty(FreeWallPieceOnBottom);
};

class EFortItemEntryState {
public:
	DefineUnrealEnum(EFortItemEntryState);

	DefineEnumProperty(NoneState);
	DefineEnumProperty(NewItemCount);
	DefineEnumProperty(ShouldShowItemToast);
	DefineEnumProperty(DurabilityInitialized);
	DefineEnumProperty(DoNotShowSpawnParticles);
	DefineEnumProperty(FromRecoveredBackpack);
	DefineEnumProperty(FromGift);
	DefineEnumProperty(PendingUpgradeCriteriaProgress);
	DefineEnumProperty(OwnerBuildingHandle);
	DefineEnumProperty(FromDroppedPickup);
	DefineEnumProperty(CraftAndSlotTarget);
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

class EFortCustomPartType {
public:
	DefineUnrealEnum(EFortCustomPartType);

	DefineEnumProperty(Head);
	DefineEnumProperty(Body);
	DefineEnumProperty(Hat);
	DefineEnumProperty(Backpack);
	DefineEnumProperty(Charm);
	DefineEnumProperty(Face);
	DefineEnumProperty(NumTypes);
};

class EBuildingReplacementType {
public:
	DefineUnrealEnum(EBuildingReplacementType);

	DefineEnumProperty(BRT_None);
	DefineEnumProperty(BRT_Edited);
	DefineEnumProperty(BRT_Conversion);
};

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

enum EAnalyticsProviderType : int32
{
	EPT_Client = 0,
    EPT_Server = 1,
    EPT_ServerGameInstance = 2,
    EPT_EsportsServerGameInstance = 3,
    EPT_MMRServerGameInstance = 4,
};

class EBuildingAttachmentType {
public:
	DefineUnrealEnum(EBuildingAttachmentType);
};

class EUpdateCompletionStatus {
public:
	DefineUnrealEnum(EUpdateCompletionStatus);

	DefineEnumProperty(UpdateUnknown);
	DefineEnumProperty(UpdateSuccess);
	DefineEnumProperty(UpdateSuccess_NoChange);
	DefineEnumProperty(UpdateSuccess_NeedsReload);
	DefineEnumProperty(UpdateSuccess_NeedsRelaunch);
	DefineEnumProperty(UpdateSuccess_NeedsPatch);
	DefineEnumProperty(UpdateFailure_PatchCheck);
	DefineEnumProperty(UpdateFailure_HotfixCheck);
	DefineEnumProperty(UpdateFailure_NotLoggedIn);
};

class ETInteractionType {
public:
	DefineUnrealEnum(ETInteractionType);

	DefineEnumProperty(IT_NoInteraction);
	DefineEnumProperty(IT_Simple);
	DefineEnumProperty(IT_LongPress);
	DefineEnumProperty(IT_BuildingEdit);
	DefineEnumProperty(IT_BuildingImprovement);
	DefineEnumProperty(IT_TrapPlacement);
};

class ESubGame {
public:
	DefineUnrealEnum(ESubGame);

	DefineEnumProperty(Campaign);
	DefineEnumProperty(Athena);
	DefineEnumProperty(Invalid);
};

class EFortQuestObjectiveStatEvent {
public:
	DefineUnrealEnum(EFortQuestObjectiveStatEvent);

	DefineEnumProperty(Kill);
	DefineEnumProperty(TeamKill);
	DefineEnumProperty(KillContribution);
	DefineEnumProperty(Build);
	DefineEnumProperty(BuildingEdit);
	DefineEnumProperty(BuildingRepair);
	DefineEnumProperty(BuildingUpgrade);
	DefineEnumProperty(Complete);
	DefineEnumProperty(Craft);
	DefineEnumProperty(Collect);
	DefineEnumProperty(Win);
	DefineEnumProperty(Interact);
	DefineEnumProperty(Destroy);
	DefineEnumProperty(Ability);
	DefineEnumProperty(WaveComplete);
	DefineEnumProperty(Custom);
	DefineEnumProperty(Client);
	DefineEnumProperty(AthenaRank);
	DefineEnumProperty(AthenaOutlive);
	DefineEnumProperty(NumGameplayEvents);
	DefineEnumProperty(Acquire);
	DefineEnumProperty(Consume);
	DefineEnumProperty(OpenCardPack);
	DefineEnumProperty(PurchaseCardPack);
	DefineEnumProperty(Convert);
	DefineEnumProperty(Upgrade);
	DefineEnumProperty(QuestComplete);
	DefineEnumProperty(AssignWorker);
	DefineEnumProperty(CollectExpedition);
	DefineEnumProperty(CollectSuccessfulExpedition);
	DefineEnumProperty(LevelUpCollectionBook);
	DefineEnumProperty(HasItem);
	DefineEnumProperty(SlotInCollection);
	DefineEnumProperty(HasCompletedQuest);
	DefineEnumProperty(HasAssignedWorker);
	DefineEnumProperty(HasUpgraded);
	DefineEnumProperty(HasConverted);
	DefineEnumProperty(HasLeveledUpCollectionBook);
	DefineEnumProperty(Max_None);
};

class EGameplayAbilityInstancingPolicy {
public:
	DefineUnrealEnum(EGameplayAbilityInstancingPolicy);

	DefineEnumProperty(NonInstanced);
	DefineEnumProperty(InstancedPerActor);
	DefineEnumProperty(InstancedPerExecution);
};

class EDeathCause {
public:
	DefineUnrealEnum(EDeathCause);

	DefineEnumProperty(OutsideSafeZone);
	DefineEnumProperty(FallDamage);
	DefineEnumProperty(Pistol);
	DefineEnumProperty(Shotgun);
	DefineEnumProperty(Rifle);
	DefineEnumProperty(SMG);
	DefineEnumProperty(Sniper);
	DefineEnumProperty(Melee);
	DefineEnumProperty(Heavy);
	DefineEnumProperty(DBNOTimeout);
	DefineEnumProperty(Banhammer);
	DefineEnumProperty(RemovedFromGame);
	DefineEnumProperty(Unspecified);
};

class EStatMod {
public:
	DefineUnrealEnum(EStatMod);

	DefineEnumProperty(Delta);
	DefineEnumProperty(Set);
	DefineEnumProperty(Maximum);
};

class EWorldItemDropBehavior {
public:
	DefineUnrealEnum(EWorldItemDropBehavior);

	DefineEnumProperty(DropAsPickup);
	DefineEnumProperty(DestroyOnDrop);
	DefineEnumProperty(DropAsPickupDestroyOnEmpty);
};

class EAthenaRespawnType {
public:
	DefineUnrealEnum(EAthenaRespawnType);

	DefineEnumProperty(None);
	DefineEnumProperty(InfiniteRespawn);
	DefineEnumProperty(InfiniteRespawnExceptStorm);
};

class EAthenaWinCondition {
public:
	DefineUnrealEnum(EAthenaWinCondition);

	DefineEnumProperty(LastManStanding);
	DefineEnumProperty(LastManStandingIncludingAllies);
	DefineEnumProperty(TimedTeamFinalFight);
	DefineEnumProperty(FirstToGoalScore);
	DefineEnumProperty(TimedLastMenStanding);
	DefineEnumProperty(MutatorControlled);
};

class EAirCraftBehavior {
public:
	DefineUnrealEnum(EAirCraftBehavior);

	DefineEnumProperty(Default);
	DefineEnumProperty(OpposingAirCraftForEachTeam);
};

class EExitCraftState {
public:
	DefineUnrealEnum(EExitCraftState);

	DefineEnumProperty(None);
	DefineEnumProperty(Spawned);
	DefineEnumProperty(Landed);
	DefineEnumProperty(SpawnBalloon);
	DefineEnumProperty(GettingIntoPosition);
	DefineEnumProperty(GettingIntoPosition_Simple);
	DefineEnumProperty(WaitingForPawns);
	DefineEnumProperty(Exiting);
};

class ERiftCosmeticState {
public:
	DefineUnrealEnum(ERiftCosmeticState);

	DefineEnumProperty(None);
	DefineEnumProperty(Intro);
	DefineEnumProperty(Idle);
	DefineEnumProperty(RampUp);
	DefineEnumProperty(ShouldDie);
};

class EControlPointState {
public:
	DefineUnrealEnum(EControlPointState);

	DefineEnumProperty(None);
	DefineEnumProperty(Disabled);
	DefineEnumProperty(Enabled);
};

class EBarrierFoodTeam {
public:
	DefineUnrealEnum(EBarrierFoodTeam);

	DefineEnumProperty(Burger);
	DefineEnumProperty(Tomato);
	DefineEnumProperty(MAX);
};

class EBarrierState {
public:
	DefineUnrealEnum(EBarrierState);

	DefineEnumProperty(BarrierUp);
	DefineEnumProperty(BarrierComingDown);
	DefineEnumProperty(BarrierDown);
};

class EBarrierFlagState {
public:
	DefineUnrealEnum(EBarrierFlagState);

	DefineEnumProperty(FlagUp);
	DefineEnumProperty(FlagDown);
};

class EBarrierObjectiveDamageState {
public:
	DefineUnrealEnum(EBarrierObjectiveDamageState);

	DefineEnumProperty(Health_75);
	DefineEnumProperty(Health_50);
	DefineEnumProperty(Health_25);
	DefineEnumProperty(Health_10);
	DefineEnumProperty(Health_5);
	DefineEnumProperty(Health_4);
	DefineEnumProperty(Health_3);
	DefineEnumProperty(Health_2);
	DefineEnumProperty(Health_1);
	DefineEnumProperty(MAX);
};

class EDynamicFoundationEnabledState {
public:
	DefineUnrealEnum(EDynamicFoundationEnabledState);

	DefineEnumProperty(Unknown);
	DefineEnumProperty(Enabled);
	DefineEnumProperty(Disabled);
};

class EAshtonStoneStateType {
public:
	DefineUnrealEnum(EAshtonStoneStateType);

	DefineEnumProperty(NotSpawned);
	DefineEnumProperty(Spawned);
	DefineEnumProperty(Captured);
	DefineEnumProperty(MAX);
};

class EAshtonStoneType {
public:
	DefineUnrealEnum(EAshtonStoneType);

	DefineEnumProperty(Purple);
	DefineEnumProperty(Blue);
	DefineEnumProperty(Red);
	DefineEnumProperty(Orange);
	DefineEnumProperty(Green);
	DefineEnumProperty(Yellow);
	DefineEnumProperty(MAX);
};

class EAthenaInventorySpawnOverride {
public:
	DefineUnrealEnum(EAthenaInventorySpawnOverride);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(Always);
	DefineEnumProperty(IntialSpawn);
	DefineEnumProperty(AircraftPhaseOnly);
	DefineEnumProperty(EAthenaInventorySpawnOverride_MAX);
};

class EAthenaLootDropOverride {
public:
	DefineUnrealEnum(EAthenaLootDropOverride);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(ForceDrop);
	DefineEnumProperty(ForceKeep);
	DefineEnumProperty(EAthenaLootDropOverride_MAX);
};

class EAthenaMutatorEvaluators {
public:
	DefineUnrealEnum(EAthenaMutatorEvaluators);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(ForceOverride);
	DefineEnumProperty(Add);
	DefineEnumProperty(Multiply);
	DefineEnumProperty(EAthenaMutatorEvaluators_MAX);
};

class EAthenaTimeOfDayOverride {
public:
	DefineUnrealEnum(EAthenaTimeOfDayOverride);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(ForceDay);
	DefineEnumProperty(ForceNight);
	DefineEnumProperty(Custom);
	DefineEnumProperty(Hour0);
	DefineEnumProperty(Hour1);
	DefineEnumProperty(Hour2);
	DefineEnumProperty(Hour3);
	DefineEnumProperty(Hour4);
	DefineEnumProperty(Hour5);
	DefineEnumProperty(Hour6);
	DefineEnumProperty(Hour7);
	DefineEnumProperty(Hour8);
	DefineEnumProperty(Hour9);
	DefineEnumProperty(Hour10);
	DefineEnumProperty(Hour11);
	DefineEnumProperty(Hour12);
	DefineEnumProperty(Hour13);
	DefineEnumProperty(Hour14);
	DefineEnumProperty(Hour15);
	DefineEnumProperty(Hour16);
	DefineEnumProperty(Hour17);
	DefineEnumProperty(Hour18);
	DefineEnumProperty(Hour19);
	DefineEnumProperty(Hour20);
	DefineEnumProperty(Hour21);
	DefineEnumProperty(Hour22);
	DefineEnumProperty(Hour23);
	DefineEnumProperty(Random);
	DefineEnumProperty(EAthenaTimeOfDayOverride_MAX);
};

class EDBNOMutatorType {
public:
	DefineUnrealEnum(EDBNOMutatorType);

	DefineEnumProperty(Default);
	DefineEnumProperty(On);
	DefineEnumProperty(Off);
	DefineEnumProperty(EDBNOMutatorType_MAX);
};

class EFortAthenaMutator_VoiceChatChannelType {
public:
	DefineUnrealEnum(EFortAthenaMutator_VoiceChatChannelType);

	DefineEnumProperty(Default);
	DefineEnumProperty(None);
	DefineEnumProperty(SquadOnly);
	DefineEnumProperty(TeamOnly);
	DefineEnumProperty(WholeServer);
	DefineEnumProperty(ScopeOnly);
	DefineEnumProperty(ScopeSquadOnly);
	DefineEnumProperty(ScopeTeamOnly);
	DefineEnumProperty(EFortAthenaMutator_MAX);
};

class EIndicatorDisplayMode {
public:
	DefineUnrealEnum(EIndicatorDisplayMode);

	DefineEnumProperty(Default);
	DefineEnumProperty(Always);
	DefineEnumProperty(Never);
	DefineEnumProperty(EIndicatorDisplayMode_MAX);
};

class EMashDifficulty {
public:
	DefineUnrealEnum(EMashDifficulty);

	DefineEnumProperty(Easy);
	DefineEnumProperty(Medium);
	DefineEnumProperty(Hard);
	DefineEnumProperty(EMashDifficulty_MAX);
};

class ERespawnRequirements {
public:
	DefineUnrealEnum(ERespawnRequirements);

	DefineEnumProperty(RespawnOnly);
	DefineEnumProperty(NoRespawnOnly);
	DefineEnumProperty(Both);
	DefineEnumProperty(ERespawnRequirements_MAX);
};
