#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AActor;
class AFortBotController;
class AFortBotStructureBuilder;
class AFortMission;
class UBehaviorTree;

class UFortBotMissionLogic : public UObject {
public:
	DefineUnrealClass(UFortBotMissionLogic);

	DefineUProperty(AFortMission*, Mission);
	DefineUProperty(TArray<AActor*>, Goals);
	DefineUProperty(TArray<AFortBotStructureBuilder*>, Builders);
	DefineUProperty(UBehaviorTree*, CurrentBehaviorAsset);
	DefineUProperty(TArray<AFortBotController*>, AssignedAI);
	DefineUProperty(TArray<uint8>, GoalHasLocator);
};
