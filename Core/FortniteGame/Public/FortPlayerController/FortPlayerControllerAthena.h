#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "FortPlayerControllerZone.h"

class AFortPlayerPawnAthena;
class AFortBroadcastRemoteClientInfo;
class UFortHero;
class UFortHeroType;
class AFortAircraft;

class AFortPlayerControllerAthena : public AFortPlayerControllerZone {
public:
	DefineUnrealClass(AFortPlayerControllerAthena);

	DefineUProperty(AFortBroadcastRemoteClientInfo*, BroadcastRemoteClientInfo);

	DefineUProperty(UFortHero*, StrongMyHero);
	DefineUProperty(TArray<UFortHeroType*>, DefaultHeroes);
public:
	static inline void (*EnterAircraftOG)(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);
	static void EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft);

	static inline void (*ServerAttemptAircraftJumpOG)(AFortPlayerControllerAthena* This, FRotator& ClientRotation);
	static void ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation);

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

		Log("Hooked AFortPlayerControllerAthena");
	}
};