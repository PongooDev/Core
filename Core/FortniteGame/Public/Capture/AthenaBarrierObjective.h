#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AAthenaBarrierFlag;
class UCustomCharacterPart;
class UMaterialInterface;
class UStaticMesh;

class EBarrierFoodTeam {
public:
	DefineUnrealEnum(EBarrierFoodTeam);

	DefineEnumProperty(Burger);
	DefineEnumProperty(Tomato);
	DefineEnumProperty(MAX);
};

class EBarrierObjectiveDamageState {
public:
	DefineUnrealEnum(EBarrierObjectiveDamageState);

	DefineEnumProperty(Health_75);
	DefineEnumProperty(Health_50);
	DefineEnumProperty(Health_25);
	DefineEnumProperty(Health_10);
	DefineEnumProperty(Health_5);
	DefineEnumProperty(Health_4);
	DefineEnumProperty(Health_3);
	DefineEnumProperty(Health_2);
	DefineEnumProperty(Health_1);
	DefineEnumProperty(MAX);
};

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
