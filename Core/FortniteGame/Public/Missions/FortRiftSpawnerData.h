#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class ABuildingRift;
class AController;
class AFortPlayerPawn;
class UFortAIEncounterInfo;
class UFortAIEncounterSequence;

struct FFortRiftSpawnerData {
public:
	DefineUnrealStruct(FFortRiftSpawnerData);

	DefineStructProperty(ABuildingRift*, Rift);
	DefineStructProperty(UFortAIEncounterInfo*, Encounter);
	DefineStructProperty(UFortAIEncounterSequence*, EncounterSequence);
	DefineStructProperty(TArray<AFortPlayerPawn*>, PlayersInRange);
	DefineStructProperty(float, TriggerDamagePercentage);
	DefineStructProperty(AController*, KillingInstigator);
	DefineStructProperty(AActor*, KillingDamageCauser);
public:
	uint8 Padding[0x50];
};
