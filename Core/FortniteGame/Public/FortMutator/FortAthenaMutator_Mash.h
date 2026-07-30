#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

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
