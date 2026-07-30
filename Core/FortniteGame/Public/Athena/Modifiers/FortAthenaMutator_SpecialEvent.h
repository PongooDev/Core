#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_SpecialEvent : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SpecialEvent);

	DefineUProperty(bool, bPlayersAreInvincible);
	DefineUProperty(bool, bDisableHUD);
	DefineUProperty(int32, AllPlayerTeleportedCount);
	DefineUProperty(FFortDelayRTMMData, DelayPlayersFromReturningToLobbyData);
	DefineUProperty(FName, NamedWeightForContainerLootRoll);
	DefineUProperty(float, WeightForContainerLootRoll);
};
