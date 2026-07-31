#pragma once
#include "pch.h"

#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Athena/GhostModeRepData.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"


class AFortPlayerControllerGameplay : public AFortPlayerController {
public:
	DefineUnrealClass(AFortPlayerControllerGameplay);

	DefineUProperty(FGhostModeRepData, GhostModeRepData);

	DefineUProperty(FMulticastScriptDelegate, OnGhostModeChanged);
public:

	static inline void (*execStartGhostModeOG)(AFortPlayerControllerGameplay* Context, FFrame& Stack);
	static void execStartGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack);

	static inline void (*execEndGhostModeOG)(AFortPlayerControllerGameplay* Context, FFrame& Stack);
	static void execEndGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack);

	static void CheckGhostModeItemReplicated();
	static void CheckGhostModeItemRemoved(UFortWorldItemDefinition* GhostModeItemDef);

	void StartGhostMode(UFortWorldItemDefinition* ItemProvidingGhostMode);
	void EndGhostMode();

	static void Hook();
};