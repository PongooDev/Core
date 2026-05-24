#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortLoadout/FortAthenaLoadout.h"

class AFortPlayerPawnAthena;
class AFortBroadcastRemoteClientInfo;
class UFortHero;
class UFortHeroType;
class AFortAircraft;
struct FFortAthenaLoadout;

class AFortPlayerControllerAthena : public AFortPlayerControllerZone {
public:
	DefineUnrealClass(AFortPlayerControllerAthena);

	DefineUProperty(AFortBroadcastRemoteClientInfo*, BroadcastRemoteClientInfo);

	DefineUProperty(UFortHero*, StrongMyHero);
	DefineUProperty(TArray<UFortHeroType*>, DefaultHeroes);
	DefineUProperty(FFortAthenaLoadout, CustomizationLoadout);
public:
	static inline void (*EnterAircraftOG)(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);
	static void EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);

	static inline void (*ServerAttemptAircraftJumpOG)(AFortPlayerControllerAthena* This, FRotator& ClientRotation);
	static void ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation);

	static inline void (*ClientOnPawnDiedOG)(AFortPlayerControllerAthena* This, FFortPlayerDeathReport& DeathReport);
	static void ClientOnPawnDied(AFortPlayerControllerAthena* This, FFortPlayerDeathReport& DeathReport);

	static inline void (*OnReadyToStartMatchOG)(AFortPlayerControllerAthena* This);
	static void OnReadyToStartMatch(AFortPlayerControllerAthena* This);

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

		/*HookEveryVTable(
			AFortPlayerControllerAthena::StaticClass(),
			AFortPlayerControllerAthena::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerZone.ClientOnPawnDied"),
			ClientOnPawnDied,
			(LPVOID*)&ClientOnPawnDiedOG
		);*/
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_ClientOnPawnDied()), ClientOnPawnDied, (LPVOID*)&ClientOnPawnDiedOG);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(
				AFortPlayerControllerAthena::GetDefaultObj(),
				Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()
			)),
			OnReadyToStartMatch,
			(LPVOID*)&OnReadyToStartMatchOG
		);

		Log("Hooked AFortPlayerControllerAthena");
	}
};