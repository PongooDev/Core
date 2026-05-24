#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);
}

void AFortPlayerControllerAthena::ClientOnPawnDied(AFortPlayerControllerAthena* This, FFortPlayerDeathReport& DeathReport) {
	if (This->IsA(AFortPlayerControllerAthena::StaticClass())) {
		if (This->WorldInventory) {
			This->WorldInventory->DropAllItems();
		}
	}

	ClientOnPawnDiedOG(This, DeathReport);
}

void AFortPlayerControllerAthena::OnReadyToStartMatch(AFortPlayerControllerAthena* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerAthena::OnReadyToStartMatch: World is null!");
		return;
	}

	if (This->CustomizationLoadout.Pickaxe) {
		This->WorldInventory->AddItem(This->CustomizationLoadout.Pickaxe->WeaponDefinition);
	}
}