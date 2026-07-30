#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/FortEnums.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"

class AAthenaBarrierFlag;
class UCustomCharacterPart;
class UMaterialInterface;
class UStaticMesh;

struct FBarrierObjectiveDisplayData {
public:
	DefineUnrealStruct(FBarrierObjectiveDisplayData);

	DefineStructProperty(UStaticMesh*, HeadMesh);
	DefineStructProperty(FVector, MeshScale);
	DefineStructProperty(FVector, MeshRelativeOffset);
	DefineStructProperty(TArray<UMaterialInterface*>, MaterialsToSwap);
public:
	uint8 Padding[0x30];
};

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
