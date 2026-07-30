#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/Athena/Modifiers/BarrierTypes.h"
#include "FortniteGame/Public/FortEnums.h"

class AAthenaBarrierObjective : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaBarrierObjective);

	DefineUProperty(FBarrierObjectiveDisplayData, FoodDisplayData);
	DefineUProperty(EBarrierFoodTeam, FoodTeam);
	DefineUProperty(EBarrierObjectiveDamageState, ObjectiveDamageState);
	DefineUProperty(float, HeadRotationYaw);
	DefineUProperty(bool, bAllowDamage);
public:
	void OnGeneratorDestroyed();

	void OnRep_FoodTeam();

	void OnRep_HeadRotationYaw();

	void OnRep_ObjectiveDamageState();

	EBarrierFoodTeam GetFoodTeam();

	EBarrierObjectiveDamageState GetObjectiveDamageState();

	bool IsFriendlyTeam();

	void SetFoodTeam(EBarrierFoodTeam NewFoodTeam);
};
