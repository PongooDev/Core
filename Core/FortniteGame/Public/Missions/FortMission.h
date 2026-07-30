#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"

class AFortObjectiveBase;
class UFortBotMissionLogic;
class UFortMissionGenerator;
class UFortMissionInfo;

class AFortMission : public AActor {
public:
	DefineUnrealClass(AFortMission);

	DefineUProperty(UFortMissionInfo*, MissionInfo);
	DefineUProperty(TArray<UClass*>, MissionEnemyClasses);
	DefineUProperty(TSubclassOf<UFortBotMissionLogic>, BotLogicClass);
	DefineUProperty(UFortBotMissionLogic*, BotLogic);
	DefineUProperty(UFortMissionGenerator*, MissionGenerator);
	DefineUProperty(uint8, MissionCategory);
	DefineUProperty(int32, UIIndex);
	DefineUProperty(FMulticastScriptDelegate, OnObjectivesUpdated);
	DefineUProperty(FMulticastScriptDelegate, OnMissionInfoSet);
	DefineUProperty(bool, bLoadedFromRecord);
	DefineUProperty(float, MissionActivationTime);
	DefineUProperty(bool, bSilentDestroyNextFrame);
	DefineUProperty(TArray<AFortObjectiveBase*>, ActiveObjectives);
};
