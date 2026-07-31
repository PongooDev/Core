#pragma once
#include "pch.h"

#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Athena/GhostModeRepData.h"


class AFortPlayerControllerGameplay : public AFortPlayerController {
public:
	DefineUnrealClass(AFortPlayerControllerGameplay);

	DefineUProperty(FGhostModeRepData, GhostModeRepData);
public:

	static inline void (*execStartGhostModeOG)(AFortPlayerControllerGameplay* Context, FFrame& Stack);
	static void execStartGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack);

	static inline void (*StartGhostModeOG)(AFortPlayerControllerGameplay * PlayerController, UFortWorldItemDefinition * ItemProvidingGhostMode);
	static void StartGhostMode(AFortPlayerControllerGameplay * PlayerController, UFortWorldItemDefinition * ItemProvidingGhostMode);

	static inline void (*execEndGhostModeOG)(AFortPlayerControllerGameplay* Context, FFrame& Stack);
	static void execEndGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack);


	static void Hook();
};