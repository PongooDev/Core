#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_GiveItemsAtPhase.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"


void AFortAthenaMutator_GiveItemsAtPhase::OnGamePhaseStepChanged(EAthenaGamePhaseStep GamePhaseStep)
{
	
	if (PhaseToGiveItems == GamePhaseStep)
	{
		TArray<AFortPlayerController*> PlayerControllers = UFortKismetLibrary::GetAllFortPlayerControllers(this, true, false);

		for (AFortPlayerController* PlayerController : PlayerControllers) 
		{
			for (FItemsToGive ItemToGive : ItemsToGive)
				PlayerController->WorldInventory->AddItem(ItemToGive.ItemToDrop, ItemToGive.NumberToGive.Evaluate());
			
		}
	}
}

void AFortAthenaMutator_GiveItemsAtPhase::execOnGamePhaseStepChanged(AFortAthenaMutator_GiveItemsAtPhase* Context, FFrame& Stack)
{
	
	struct FortAthenaMutator_GiveItemsAtPhase_OnGamePhaseStepChanged
	{
	public:
		EAthenaGamePhaseStep GamePhaseStep;
	};
	FortAthenaMutator_GiveItemsAtPhase_OnGamePhaseStepChanged* Parms = (FortAthenaMutator_GiveItemsAtPhase_OnGamePhaseStepChanged*)Stack.Locals;

	Context->OnGamePhaseStepChanged(Parms->GamePhaseStep);
	
}

void AFortAthenaMutator_GiveItemsAtPhase::Hook() {
	
	ExecHook("Function /Script/FortniteGame.FortAthenaMutator_GiveItemsAtPhase.OnGamePhaseStepChanged", execOnGamePhaseStepChanged, execOnGamePhaseStepChangedOG);

	Log("Hooked AFortAthenaMutator_GiveItemsAtPhase");
}