#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingSMActor.h"

ABuildingSMActor* ABuildingSMActor::ReplaceBuildingActor(EBuildingReplacementType ReplacementType, UClass* ReplacementClass, int ReplacementUpgradeLevel, int RotationIterations, bool InbMirrored, AFortPlayerController* EditingController) {
	ABuildingSMActor* (*&ReplaceBuildingActorInternal)(ABuildingSMActor * This, EBuildingReplacementType ReplacementType, UClass * ReplacementClass, int ReplacementUpgradeLevel, int RotationIterations, bool InbMirrored, AFortPlayerController * EditingController) = decltype(ReplaceBuildingActorInternal)(VTable[Finder::FindABuildingSMActor_ReplaceBuildingActorVFT()]);
	return ReplaceBuildingActorInternal(this, ReplacementType, ReplacementClass, ReplacementUpgradeLevel, RotationIterations, InbMirrored, EditingController);
}

void ABuildingSMActor::RepairBuilding(AFortPlayerController* RepairingController, int32 ResourcesSpent)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("RepairBuilding"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&RepairBuildingInternal)(ABuildingSMActor*, AFortPlayerController*, int32) = decltype(RepairBuildingInternal)(VTable[VTableIdx]);
		return RepairBuildingInternal(this, RepairingController, ResourcesSpent);
	}
}

void ABuildingSMActor::SetEditingPlayer(AFortPlayerStateZone* NewEditingPlayer) {
	void (*&SetEditingPlayerInternal)(ABuildingSMActor * This, AFortPlayerStateZone * NewEditingPlayer) = decltype(SetEditingPlayerInternal)(VTable[Finder::FindABuildingSMActor_SetEditingPlayerVFT()]);
	SetEditingPlayerInternal(this, NewEditingPlayer);
}