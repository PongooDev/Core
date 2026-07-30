#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UStaticMesh;

class EFortVehicleDecoType {
public:
	DefineUnrealEnum(EFortVehicleDecoType);

	DefineEnumProperty(Unknown);
	DefineEnumProperty(Flag);
	DefineEnumProperty(HoodOrnament);
	DefineEnumProperty(Wings);
};

class UAthenaVehicleCosmeticItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaVehicleCosmeticItemDefinition);

	DefineUProperty(EFortVehicleDecoType, DecoType);
	DefineUProperty(FTransform, DecoSocketTransform);
	DefineUProperty(TSoftObjectPtr<UStaticMesh>, TestDecoMesh);
	DefineUProperty(FName, DecoSocketName);
};
