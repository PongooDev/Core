#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_GiveItemsAtGamePhaseStep.h"

class AFortAthenaMutator_GiveItemsAtPhase : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GiveItemsAtPhase);

	DefineUProperty(EAthenaGamePhaseStep, PhaseToGiveItems);
	uint8 Pad_379[0x7];
	DefineUProperty(TArray<FItemsToGive>, ItemsToGive);
public:

	void OnGamePhaseStepChanged(EAthenaGamePhaseStep GamePhaseStep);
	
	static inline void (*execOnGamePhaseStepChangedOG)(AFortAthenaMutator_GiveItemsAtPhase* Context, FFrame& Stack);
	static void execOnGamePhaseStepChanged(AFortAthenaMutator_GiveItemsAtPhase* Context, FFrame& Stack);
	
	static void Hook();
};
