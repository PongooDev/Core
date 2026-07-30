#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Capture/AthenaBarrierObjective.h"

class AAthenaBarrierObjective;
class UStaticMesh;

class UCustomCharacterPart;
class UMaterialInterface;

class EBarrierFlagState {
public:
	DefineUnrealEnum(EBarrierFlagState);

	DefineEnumProperty(FlagUp);
	DefineEnumProperty(FlagDown);
};

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
