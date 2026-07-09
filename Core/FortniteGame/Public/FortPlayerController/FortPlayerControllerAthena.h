#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Engine/Classes/Camera/PlayerCameraManager.h"

#include "FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortLoadout/FortAthenaLoadout.h"
#include "FortniteGame/Public/Athena/AthenaRewardResult.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"
#include "FortniteGame/Public/Athena/AthenaMatchTeamStats.h"
#include "FortniteGame/Public/Athena/AthenaPlayerMatchReport.h"
#include "FortniteGame/Public/QuickChat/AthenaQuickChatActiveEntry.h"

class AFortPlayerPawnAthena;
class AFortBroadcastRemoteClientInfo;
class UFortHero;
class UFortHeroType;
class AFortAircraft;
struct FFortAthenaLoadout;
class UFortWeaponItemDefinition;
struct FAthenaRewardResult;
class UAthenaPlayerMatchReport;

class AFortPlayerControllerAthena : public AFortPlayerControllerZone {
public:
	DefineUnrealClass(AFortPlayerControllerAthena);

	DefineUProperty(AFortBroadcastRemoteClientInfo*, BroadcastRemoteClientInfo);

	DefineUProperty(UFortHero*, StrongMyHero);
	DefineUProperty(TArray<UFortHeroType*>, DefaultHeroes);
	DefineUProperty(FFortAthenaLoadout, CustomizationLoadout);
	DefineUProperty(APawn*, PlayerToSpectateOnDeath);
	DefineUProperty(bool, bMarkedAlive);
	DefineUProperty(UAthenaPlayerMatchReport*, MatchReport);
public:
	static inline void (*EnterAircraftOG)(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);
	static void EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);

	static inline void (*ServerAttemptAircraftJumpOG)(AFortPlayerControllerAthena* This, FRotator& ClientRotation);
	static void ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation);

	void ClientOnPawnDied_Implementation(FFortPlayerDeathReport& DeathReport);

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerControllerAthena* This);
	static void OnReadyToStartMatch(AFortPlayerControllerAthena* This);

	static inline void (*ServerReturnToMainMenuOG)(AFortPlayerControllerAthena* This);
	static void ServerReturnToMainMenu(AFortPlayerControllerAthena* This);

	void ClientNotifyTeamWon(APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);

	void ClientNotifyWon(APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);

	void ClientSendEndBattleRoyaleMatchForPlayer(bool bSuccess, const FAthenaRewardResult& Result);

	void ClientSendMatchStatsForPlayer(const FAthenaMatchStats& Stats);

	void ClientSendTeamStatsForPlayer(const FAthenaMatchTeamStats& TeamStats);

	FAthenaRewardResult& ConstructAthenaRewardResult();

	FAthenaMatchStats& ConstructAthenaMatchStats();

	FAthenaMatchTeamStats& ConstructAthenaMatchTeamStats();

	static inline void (*ServerPlaySquadQuickChatMessageOG)(AFortPlayerControllerAthena* This, FAthenaQuickChatActiveEntry& ChatEntry, FUniqueNetIdRepl& SenderID);
	static void ServerPlaySquadQuickChatMessage(AFortPlayerControllerAthena* This, FAthenaQuickChatActiveEntry& ChatEntry, FUniqueNetIdRepl& SenderID);

	static void Hook() {
		UObject* AircraftComp = FUObjectArray::FindObject("Class FortniteGame.FortControllerComponent_Aircraft");
		if (!AircraftComp) {
			MH_CreateHook(
				(LPVOID)(ImageBase + Finder::FindUFortControllerComponent_Aircraft_EnterAircraft()),
				EnterAircraft,
				(LPVOID*)&EnterAircraftOG
			);

			HookEveryVTable(
				AFortPlayerControllerAthena::StaticClass(),
				AFortPlayerControllerAthena::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerAthena.ServerAttemptAircraftJump"),
				ServerAttemptAircraftJump,
				(LPVOID*)&ServerAttemptAircraftJumpOG
			);
		}

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(
				AFortPlayerControllerAthena::GetDefaultObj(),
				Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()
			)),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);

		HookVTable(
			AFortPlayerControllerAthena::GetDefaultObj(),
			AFortPlayerControllerAthena::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerReturnToMainMenu"),
			ServerReturnToMainMenu,
			(LPVOID*)&ServerReturnToMainMenuOG
		);

		HookEveryVTable(
			AFortPlayerControllerAthena::StaticClass(),
			AFortPlayerControllerAthena::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerAthena.ServerPlaySquadQuickChatMessage"),
			ServerPlaySquadQuickChatMessage,
			(LPVOID*)&ServerPlaySquadQuickChatMessageOG
		);

		Log("Hooked AFortPlayerControllerAthena");
	}
};