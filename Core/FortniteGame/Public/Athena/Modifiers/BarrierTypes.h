#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"

#include "FortniteGame/Public/FortEnums.h"

class AAthenaBarrierFlag;
class AAthenaBarrierObjective;
class UCustomCharacterPart;
class UMaterialInterface;
class UStaticMesh;

struct FBarrierHeadData {
public:
	DefineUnrealStruct(FBarrierHeadData);

	DefineStructProperty(TArray<UCustomCharacterPart*>, PartsToSwapIn);
public:
	uint8 Padding[0x10];
};

struct FBarrierMountedTurretData {
public:
	DefineUnrealStruct(FBarrierMountedTurretData);

	DefineStructProperty(TArray<UMaterialInterface*>, MaterialOverrides);
	DefineStructProperty(UMaterialInterface*, BaseMaterialOverride);
public:
	uint8 Padding[0x18];
};

struct FBarrierTeamState {
public:
	DefineUnrealStruct(FBarrierTeamState);

	DefineStructProperty(uint8, TeamNum);
	DefineStructProperty(EBarrierFoodTeam, FoodTeam);
	DefineStructProperty(AAthenaBarrierFlag*, ObjectiveFlag);
	DefineStructProperty(AAthenaBarrierObjective*, ObjectiveObject);
	DefineStructProperty(bool, bRespawnEnabled);
public:
	uint8 Padding[0x28];
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
