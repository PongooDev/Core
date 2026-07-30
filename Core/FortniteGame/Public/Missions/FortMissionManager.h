#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortniteGame/Public/Missions/FortMissionInfo.h"

class AFortBluGloManager;
class AFortMission;

class AFortMissionManager : public AActor {
public:
	DefineUnrealClass(AFortMissionManager);

	DefineUProperty(TSoftObjectPtr<UClass>, BluGloManagerClass);
	DefineUProperty(AFortBluGloManager*, BluGloManager);
	DefineUProperty(TArray<AFortMission*>, Missions);
	DefineUProperty(AFortMission*, CurrentUIFocusedMission);
	DefineUProperty(AFortMission*, MissionClosestToPar);
	DefineUProperty(FMulticastScriptDelegate, OnMissionsUpdated);
	DefineUProperty(bool, bDisplaySecondaryMissionHeaders);
public:
	void OnRep_Missions();
};
