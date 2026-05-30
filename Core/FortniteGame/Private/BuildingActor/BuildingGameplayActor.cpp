#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingGameplayActor.h"

void AB_SupplyDropPlacement_C::SpawnTierDrops(AB_SupplyDropPlacement_C* This, FFrame& Stack)
{
	int32 LootTableIndex = 0;
	Stack.StepCompiledIn(&LootTableIndex);
	Stack.IncrementCode();

	Log("AB_SupplyDropPlacement_C::SpawnTierDrops called for " + This->GetName().ToString());
}