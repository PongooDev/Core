#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"

#include "FortniteGame/Public/AI/FortAIEncounterInfo.h"
#include "FortniteGame/Public/AI/IntensityData.h"
#include "FortniteGame/Public/AI/UtilityTypeFloatPair.h"
#include "FortniteGame/Public/AI/FortAIEncounterPIDController.h"
#include "FortniteGame/Public/AI/FortAIEncounterPIDControllerSettings.h"
#include "FortniteGame/Public/AI/FortPlayerPerformanceEstimateSettings.h"
#include "FortniteGame/Public/AI/FortEncounterPawnNumberCaps.h"
#include "FortniteGame/Public/AI/FortAIEncounterSpawnGroupCapsProfile.h"
#include "FortniteGame/Public/AI/FortAIEncounterSpawnPointsProfile.h"
#include "FortniteGame/Public/AI/FortAIEncounterPawnDifficultyLevelModifier.h"
#include "FortniteGame/Public/AI/FortAISpawnGroupUpgradeData.h"
#include "FortniteGame/Public/AI/FortAILootDropModifiers.h"
#include "FortniteGame/Public/AI/FortPendingStoppedEncounterData.h"
#include "FortniteGame/Public/AI/PendingSpawnInfo.h"

class AFortAIPawn;
class ABuildingRift;
class UCurveFloat;
class UReporterGraph;
class UEQSRenderingComponent;
class UFortAIEncounterSequence;
class UFortAIDirectorDataTrackingSettings;
class IFortPatrolWardInterface;

class AFortAIDirector : public AActor {
public:
	DefineUnrealClass(AFortAIDirector);

	DefineUProperty(FCurveTableRowHandle, MaxAliveCurve);
	DefineUProperty(FCurveTableRowHandle, MinAliveCurve);
	DefineUProperty(FIntensityData, IntensityInfo);
	DefineUProperty(FFortAIEncounterPIDController, AIDirectorPIDController);
	DefineUProperty(FFortAIEncounterPIDControllerSettings, PIDControllerSettings);
	DefineUProperty(float, UnreachableLocationPathCost);
	DefineUProperty(FFortPlayerPerformanceEstimateSettings, PlayerPerformanceEstimateSettings);
	DefineBitfieldUProperty(bUsePrototypeEnemies);
	DefineUProperty(TArray<uint8>, DebugEncounterDirections);
	DefineUProperty(FGameplayTagContainer, DebugSpawnAIGroupTags);
	DefineUProperty(FGameplayTagContainer, DebugEncounterTags);
	DefineUProperty(float, BurstSpawnThreatVisualsEndDelay);
	DefineUProperty(TArray<FFortAIEncounterSpawnGroupCapsProfile>, EncounterSpawnGroupCapSettings);
	DefineUProperty(TArray<FFortAIEncounterSpawnPointsProfile>, EncounterSpawnPointsSettings);
	DefineUProperty(TArray<FFortAIEncounterPawnDifficultyLevelModifier>, EncounterPawnDifficultyLevelModifiers);
	DefineUProperty(TArray<FFortAISpawnGroupUpgradeData>, SpawnGroupUpgrades);
	DefineUProperty(float, GuaranteedUpgradeGroupUtilityBonus);
	DefineUProperty(UFortAIDirectorDataTrackingSettings*, DataTrackingSettings);
	DefineUProperty(TArray<FDataTableRowHandle>, BaseLootDropData);
	DefineUProperty(TArray<FFortAILootDropModifiers>, LootDropModifiers);
	DefineUProperty(FGameplayTagQuery, LootDropDenialQuery);
	DefineBitfieldUProperty(bDebugEncounterQueries);
	DefineUProperty(FFortEncounterPawnNumberCaps, DefaultEncounterPawnCaps);
	DefineUProperty(UReporterGraph*, IntensityGraph);
	DefineUProperty(UReporterGraph*, UtilitiesGraph);
	DefineUProperty(UReporterGraph*, PIDValuesGraph);
	DefineUProperty(UReporterGraph*, PIDContributionsGraph);
	DefineBitfieldUProperty(bNightActive);
	DefineBitfieldUProperty(bAIDisabled);
	DefineBitfieldUProperty(bRegisteredForDayPhaseChange);
	DefineBitfieldUProperty(bUseSpawnCap);
	DefineUProperty(int32, NightCount);
	DefineUProperty(TArray<ABuildingRift*>, RiftSpawnPoints);
	DefineUProperty(float, NightEncounterFailureBreatherTime);
	DefineUProperty(float, EncounterPawnSpawnInterval);
	DefineUProperty(TSubclassOf<UFortAIEncounterInfo>, DefaultNightEncounter);
	DefineUProperty(TSubclassOf<UFortAIEncounterInfo>, DummyDebugEncounter);
	DefineUProperty(TSubclassOf<UFortAIEncounterInfo>, BaseEncounterClass);
	DefineUProperty(int32, MaxActiveAlive);
	DefineUProperty(int32, NumActiveAlive);
	DefineUProperty(int32, NumPendingCapRelevantAI);
	DefineUProperty(TArray<FPendingSpawnInfo>, PendingSpawns);
	DefineUProperty(int32, MaxAISpawnedPerFrame);
	DefineUProperty(int32, MaxAIDespawnedPerFrame);
	DefineUProperty(TArray<TWeakObjectPtr<AFortAIPawn>>, PendingDespawns);
	DefineUProperty(int32, NumWorldSubdivides);
	DefineUProperty(float, MinAISpawnDistanceFromPlayers);
	DefineUProperty(FGameplayTagContainer, AmbientThreatPreferredPlacementActorTags);
	DefineUProperty(FGameplayTagContainer, AmbientThreatRequiredPlacementActorTags);
	DefineUProperty(TArray<UFortAIEncounterInfo*>, ActiveEncounters);
	DefineUProperty(TArray<FFortPendingStoppedEncounterData>, PendingStoppedEncounters);
	DefineUProperty(UFortAIEncounterInfo*, ActiveDefaultEncounter);
	DefineUProperty(UFortAIEncounterInfo*, ActiveDummyDebugEncounter);
	DefineUProperty(TArray<UFortAIEncounterSequence*>, EncounterSequences);
	DefineUProperty(UEQSRenderingComponent*, EQSRenderingComp);
	DefineUProperty(int32, DebugGraphUpdateFrequency);
	DefineUProperty(float, MaxNormalLODDistanceToPlayer);
	DefineUProperty(float, AIRelevantDistanceToPlayer);
	DefineUProperty(float, EncounterRelevantDistanceToPlayer);
	DefineUProperty(float, EncounterRelevantDistanceToDefender);
	DefineUProperty(TArray<TScriptInterface<IFortPatrolWardInterface>>, PatrolWards);
	DefineUProperty(int32, MaxTotalActiveAliveAI);
	DefineUProperty(int32, MaxEncounterActiveAliveAI);
	DefineUProperty(int32, MaxSPUsed);
	DefineUProperty(TArray<FUtilityTypeFloatPair>, DebugEncounterTopUtilityPercentages);
	DefineUProperty(TArray<UCurveFloat*>, DebugEncounterSpawnPointsCurves);
	DefineBitfieldUProperty(bDebugAllowEncounterModifierTags);
	DefineUProperty(int32, SimulatedNumberOfPlayersForAIEncounters);
public:
	UFortAIEncounterInfo* StartEncounterWithoutObjective(TSubclassOf<UFortAIEncounterInfo> EncounterTemplate);

	void Activate();
	static void execActivate(AFortAIDirector* Context, FFrame& Stack);

	static AFortAIDirector* GetCurrent(UObject* WorldContextObject);

	static void Hook();
};
