#include "pch.h"
#include "FortniteGame/Public/FortWeapon/FortDecoTool.h"

#include "FortniteGame/Public/BuildingActor/BuildingTrap.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"

void AFortDecoTool::ServerSpawnDeco(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType) {
	Log("ServerSpawnDeco Called!");

	ServerSpawnDecoOG(This, Location, Rotation, AttachedActor, InBuildingAttachmentType);

	AFortPawn* Pawn = This->Owner->Cast<AFortPawn>();
	if (!Pawn) {
        Log("ServerSpawnDeco: Failed to get Pawn from Owner!");
        return;
    }
    
	AFortPlayerController* PlayerController = Pawn->Controller ? Pawn->Controller->Cast<AFortPlayerController>() : nullptr;
    if (!PlayerController) {
        Log("ServerSpawnDeco: Failed to get PlayerController from Pawn!");
        return;
	}

	AFortPlayerStateAthena* FortPlayerStateAthena = PlayerController->PlayerState->Cast<AFortPlayerStateAthena>();
    if (FortPlayerStateAthena) {
		ABuildingTrap* TrapActor = nullptr;
        for (ABuildingSMActor* Attached : AttachedActor->AttachedBuildingActors) {
            if (!Attached)
                continue;

            if (Attached->IsA(ABuildingTrap::StaticClass()) && Attached->Team != FortPlayerStateAthena->TeamIndex) {
                TrapActor = Attached->Cast<ABuildingTrap>();
            }
        }

        if (TrapActor) {
            TrapActor->Team = ((AFortPlayerStateAthena*)PlayerController->PlayerState)->TeamIndex;
            TrapActor->TeamIndex = TrapActor->Team;
			Log("ServerSpawnDeco: Updated trap team to " + std::to_string(TrapActor->Team));
        }
    }
}