#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingSMActor.h"

ABuildingSMActor* ABuildingSMActor::ReplaceBuildingActor(uint8 ReplacementType, UClass* ReplacementClass, int ReplacementUpgradeLevel, int RotationIterations, bool InbMirrored, AFortPlayerController* EditingController) {
	ABuildingSMActor* (*&ReplaceBuildingActorInternal)(ABuildingSMActor * This, uint8 ReplacementType, UClass * ReplacementClass, int ReplacementUpgradeLevel, int RotationIterations, bool InbMirrored, AFortPlayerController * EditingController) = decltype(ReplaceBuildingActorInternal)(VTable[Finder::FindABuildingSMActor_ReplaceBuildingActorVFT()]);
	return ReplaceBuildingActorInternal(this, ReplacementType, ReplacementClass, ReplacementUpgradeLevel, RotationIterations, InbMirrored, EditingController);
}

void ABuildingSMActor::RepairBuilding(AFortPlayerController* RepairingController, int32 ResourcesSpent)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RepairBuilding");

	return Call(Func, RepairingController, ResourcesSpent);
}

void ABuildingSMActor::SetEditingPlayer(AFortPlayerStateZone* NewEditingPlayer) {
	void (*&SetEditingPlayerInternal)(ABuildingSMActor * This, AFortPlayerStateZone * NewEditingPlayer) = decltype(SetEditingPlayerInternal)(VTable[Finder::FindABuildingSMActor_SetEditingPlayerVFT()]);
	SetEditingPlayerInternal(this, NewEditingPlayer);
}