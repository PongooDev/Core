#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "FortniteGame/Public/FortEnums.h"

class AAthenaBarrierObjective;
class UStaticMesh;

class UCustomCharacterPart;
class UMaterialInterface;

struct FBarrierFlagDisplayData {
public:
	DefineUnrealStruct(FBarrierFlagDisplayData);

	DefineStructProperty(UStaticMesh*, HeadMesh);
	DefineStructProperty(FVector2D, MapSize);
	DefineStructProperty(FVector2D, CompassSize);
	DefineStructProperty(FVector, MeshScale);
public:
	uint8 Padding[0x248];
};

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
