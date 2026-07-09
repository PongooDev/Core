#pragma once
#include "pch.h"

#include "FortPlayerControllerGameplay.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"

class AFortPlayerPawnAthena;
struct FFortPlayerDeathReport;

class AFortPlayerControllerZone : public AFortPlayerControllerGameplay {
public:
	DefineUnrealClass(AFortPlayerControllerZone);
public:
	static void ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P);

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerControllerZone* This);
	static void OnReadyToStartMatch(AFortPlayerControllerZone* This);

	static inline void (*ServerReturnToMainMenuOG)(AFortPlayerControllerZone* This);
	static void ServerReturnToMainMenu(AFortPlayerControllerZone* This);

	void ClientOnPawnDied_Implementation(FFortPlayerDeathReport& DeathReport);

	static inline void (*ClientOnPawnDiedOG)(AFortPlayerControllerZone* This, FFortPlayerDeathReport& DeathReport);
	static void ClientOnPawnDied(AFortPlayerControllerZone* This, FFortPlayerDeathReport& DeathReport);

	void ClientOnPawnRevived(AController* EventInstigator);

	static void Hook();
};