#pragma once
#include "pch.h"

#include "FortPlayerControllerGameplay.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

class AFortPlayerPawnAthena;

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay {
public:
	DefineUnrealClass(AFortPlayerControllerZone);
public:
	static void ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerControllerZone* This);
	static void OnReadyToStartMatch(AFortPlayerControllerZone* This);

	static inline void (*ServerSendLoadoutConfigOG)(AFortPlayerControllerZone* This, int32 LoadoutSeed, TArray<uint8>& Loadout);
	static void ServerSendLoadoutConfig(AFortPlayerControllerZone* This, int32 LoadoutSeed, TArray<uint8>& Loadout);

	static inline void (*ServerSetShouldDisablePlayerTeleportingDuringMissionResultsOG)(AFortPlayerControllerZone* This);
	static void ServerSetShouldDisablePlayerTeleportingDuringMissionResults(AFortPlayerControllerZone* This);

	static void Hook();
};