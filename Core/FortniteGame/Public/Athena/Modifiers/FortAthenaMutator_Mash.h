#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "FortniteGame/Public/FortEnums.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "FortniteGame/Public/Items/ItemAndCount.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "FortniteGame/Public/Athena/Modifiers/MutatorObjectData.h"

class AFortAthenaSupplyDrop;

class AActor;
class AAthenaBarrierFlag;
class AAthenaBigBaseWall;
class AAthenaFillFloor;
class AAthenaGoosePickup;
class AAthenaSkyCap;
class ABuildingActor;
class ABuildingGameplayActorSpawnMachine;
class AFortAthenaVehicle;
class AFortGameModeAthena;
class AFortGameStateAthena;
class AFortPlayerController;
class AFortPlayerControllerAthena;
class AFortPlayerPawnAthena;
class AFortPlayerStateAthena;
class UAnimInstance;
class UCustomCharacterPart;
class UDataAsset;
class UEnvQuery;
class UFortAIEncounterInfo;
class UFortAISpawnGroup;
class UFortAthenaExitCraftInfo;
class UFortAthenaMutator_SpawningPolicyData;
class UFortDifficultyEncounterSettings;
class UFortGadgetItemDefinition;
class UFortMissionInfo;
class UFortMovementComp_AIChar;
class UFortServerBotManagerAthena;
class UFortTeamSettingsComponent;
class UFortWorldItemDefinition;
class UGameplayEffect;
class UMaterialInstance;
class USkeletalMesh;
class USoundCue;
class UStaticMesh;
class UTexture2D;

struct FFortAISpawnGroupAssetData {
public:
	DefineUnrealStruct(FFortAISpawnGroupAssetData);

	DefineStructProperty(UFortAISpawnGroup*, SpawnGroup);
	DefineStructProperty(FScalableFloat, Weight);
	DefineStructProperty(FScalableFloat, MinNumber);
	DefineStructProperty(FScalableFloat, MaxNumber);
public:
	uint8 Padding[0x68];
};

struct FFortAIUpgradeEffectAssetData {
public:
	DefineUnrealStruct(FFortAIUpgradeEffectAssetData);

	DefineStructProperty(FScalableFloat, ApplyingEffectPercentage);
	DefineStructProperty(TSubclassOf<UGameplayEffect>, GameplayEffect);
public:
	uint8 Padding[0x28];
};

struct FFortAIAssetData {
public:
	DefineUnrealStruct(FFortAIAssetData);

	DefineStructProperty(float, MovingZoneSpawningRate);
	DefineStructProperty(float, MovingZoneSpawningRadiusMultiplier);
	DefineStructProperty(float, SpawnSafeZoneRadiusRatio);
	DefineStructProperty(float, SpawnMovingSafeZoneRadiusRatio);
	DefineStructProperty(float, SpawnTraceStartHeightOffset);
	DefineStructProperty(float, SpawnTraceLength);
	DefineStructProperty(FScalableFloat, KillScore);
	DefineStructProperty(FScalableFloat, SpawnCount);
	DefineStructProperty(TArray<FFortAISpawnGroupAssetData>, SpawnGroupAssets);
	DefineStructProperty(TArray<FFortAIUpgradeEffectAssetData>, BaseUpgradeEffectAssets);
	DefineStructProperty(TArray<FFortAIUpgradeEffectAssetData>, ExtraUpgradeEffectAssets);
public:
	uint8 Padding[0x88];
};

struct FFortObjectiveAreaInstanceData {
public:
	DefineUnrealStruct(FFortObjectiveAreaInstanceData);

	DefineStructProperty(FVector, Location);
	DefineStructProperty(ABuildingGameplayActorSpawnMachine*, SpawnMachine);
	DefineStructProperty(AFortAthenaSupplyDrop*, MainObjective);
	DefineStructProperty(TArray<ABuildingActor*>, SubObjectives);
public:
	uint8 Padding[0x30];
};

struct FFortObjectiveMessagesAssetData {
public:
	DefineUnrealStruct(FFortObjectiveMessagesAssetData);

	DefineStructProperty(FAthenaGameMessageData, GameMsg_SafeZonePreObjective);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_Objectives);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_MainObjectiveSpawned);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_SafeZonePostObjective);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_MoveToNextArea);
public:
	uint8 Padding[0x118];
};

struct FFortSubObjectiveSpawnData {
public:
	DefineUnrealStruct(FFortSubObjectiveSpawnData);

	DefineStructProperty(FScalableFloat, ScoreMultiplier);
	DefineStructProperty(FScalableFloat, SpawnCount);
	DefineStructProperty(TSubclassOf<ABuildingActor>, ObjectClass);
public:
	uint8 Padding[0xC8];
};

struct FMashDifficultySettings {
public:
	DefineUnrealStruct(FMashDifficultySettings);

	DefineStructProperty(FScalableFloat, SpawnCountPlayerCountMultiplier);
	DefineStructProperty(FScalableFloat, SpawnCountDifficultyMultiplier);
	DefineStructProperty(FScalableFloat, AIEffectPlayerCountMultiplier);
	DefineStructProperty(FScalableFloat, AIEffectDifficultyMultiplier);
	DefineStructProperty(float, SpawnCountMultiplier);
	DefineStructProperty(float, AIEffectMultiplier);
public:
	uint8 Padding[0x88];
};

struct FMashPlayerScoreInstanceData {
public:
	DefineUnrealStruct(FMashPlayerScoreInstanceData);

	DefineStructProperty(FString, PlayerName);
	DefineStructProperty(int32, PlayerID);
	DefineStructProperty(int32, Score);
public:
	uint8 Padding[0x18];
};

class AFortAthenaMutator_Mash : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Mash);

	DefineUProperty(bool, bOverrideReplicationSettingsDebug);
	DefineUProperty(FScalableFloat, MaxAIPerClient);
	DefineUProperty(FScalableFloat, ReplicateGameplayCues);
	DefineUProperty(bool, bIsUsingSpecialLocationDebug);
	DefineUProperty(TArray<FGameplayTag>, SpecialLocationDebug);
	DefineUProperty(EMashDifficulty, CurrentDifficulty);
	DefineUProperty(float, SpawnMachineTraceStartHeight);
	DefineUProperty(float, SpawnMachineTraceEndHeight);
	DefineUProperty(TSubclassOf<ABuildingGameplayActorSpawnMachine>, SpawnMachineObjectClass);
	DefineUProperty(FGameplayTag, ObjectiveAreaTag);
	DefineUProperty(FScalableFloat, IsObjectiveArea);
	DefineUProperty(FScalableFloat, IsMovingSafeZone);
	DefineUProperty(float, SpawnObjectiveTraceStartHeight);
	DefineUProperty(float, SpawnObjectiveTraceEndHeight);
	DefineUProperty(FScalableFloat, MainObjectiveScoreMultiplier);
	DefineUProperty(FScalableFloat, MainObjectiveSpawnTimeBeforeEnd);
	DefineUProperty(TSubclassOf<AFortAthenaSupplyDrop>, MainObjectiveSupplyDropObjectClass);
	DefineUProperty(TSubclassOf<AFortAthenaSupplyDrop>, FinalMainObjectiveSupplyDropObjectClass);
	DefineUProperty(TArray<FFortSubObjectiveSpawnData>, SubObjectives);
	DefineUProperty(FFortAIAssetData, AIAsset);
	DefineUProperty(UFortDifficultyEncounterSettings*, DifficultyEncounterSettingsStatic);
	DefineUProperty(UFortDifficultyEncounterSettings*, DifficultyEncounterSettingsDynamic);
	DefineUProperty(bool, bUseEncounterSettingToSpawnAI);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
	DefineUProperty(TArray<FFortObjectiveMessagesAssetData>, ObjectiveMessages);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FinalObjectiveBeat);
	DefineUProperty(FMashDifficultySettings, DifficultySettings);
	DefineUProperty(int32, CurrentObjectiveAreaIndex);
	DefineUProperty(TArray<FFortObjectiveAreaInstanceData>, ObjectiveAreas);
	DefineUProperty(UFortAIEncounterInfo*, CurrentEncounter);
	DefineUProperty(float, CurrentScoreMultiplier);
	DefineUProperty(TArray<FMashPlayerScoreInstanceData>, PlayerScores);
};
