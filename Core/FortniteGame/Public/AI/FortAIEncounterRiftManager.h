#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "FortniteGame/Public/AI/FortAIEncounterSpawnArea.h"
#include "FortniteGame/Public/AI/FortAIEncounterQueryDirectionTracker.h"
#include "FortniteGame/Public/AI/FortEncounterSettings.h"
#include "FortniteGame/Public/AI/EncounterEnvironmentQueryInfo.h"

class ABuildingRift;
class AFortAIDirector;
class UFortAIEncounterInfo;

class UFortAIEncounterRiftManager : public UObject {
public:
	DefineUnrealClass(UFortAIEncounterRiftManager);

	DefineUProperty(UFortAIEncounterInfo*, MyEncounter);
	DefineUProperty(AFortAIDirector*, AIDirector);
	DefineUProperty(FFortAIEncounterSpawnArea, CurrentSpawnArea);
	DefineUProperty(FFortAIEncounterSpawnArea, FutureSpawnArea);
	DefineUProperty(float, UpdateIntervalTimeSeconds);
	DefineUProperty(int32, NumRiftsToUse);
	DefineUProperty(int32, MinRiftsToUse);
	DefineUProperty(FFortEncounterSettings, EncounterSettings);
	DefineUProperty(float, ExtraSpawnLocationPercentage);
	DefineUProperty(FEncounterEnvironmentQueryInfo, CurrentEnvironmentQueryInfo);
	DefineUProperty(FEncounterEnvironmentQueryInfo, FallbackEnvironmentQueryInfo);
	DefineUProperty(FFortAIEncounterQueryDirectionTracker, EncounterQueryDirectionTracker);
	DefineUProperty(TSubclassOf<ABuildingRift>, RiftClassTemplate);
	DefineUProperty(float, LastObjectiveBatchPathCostUpdateTime);
	DefineUProperty(float, LastPlayerBatchPathCostUpdateTime);
};
