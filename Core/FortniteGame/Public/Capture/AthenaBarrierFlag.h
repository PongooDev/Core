#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/Athena/Modifiers/BarrierTypes.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AAthenaBarrierObjective;
class UStaticMesh;

class AAthenaBarrierFlag : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaBarrierFlag);

	DefineUProperty(EBarrierFlagState, CurrentState);
	DefineUProperty(EBarrierFoodTeam, FoodTeam);
	DefineUProperty(FBarrierFlagDisplayData, FoodDisplayData);
public:
	void OnRep_CurrentState();

	void OnRep_FoodTeam();

	UStaticMesh* GetHeadMesh();

	FVector GetMeshScale();

	void SetFoodTeam(EBarrierFoodTeam NewFoodTeam);

	static inline AAthenaBarrierObjective* (*GetObjectiveActorOG)(AAthenaBarrierFlag* This);
	static AAthenaBarrierObjective* GetObjectiveActor(AAthenaBarrierFlag* This);

	static void Hook();
};
