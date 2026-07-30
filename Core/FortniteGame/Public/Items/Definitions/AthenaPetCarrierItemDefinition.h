#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaBackpackItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class UAthenaPetItemDefinition;

struct FFortUICameraFrameTargetBounds {
public:
	DefineUnrealStruct(FFortUICameraFrameTargetBounds);

	DefineStructProperty(FVector, Origin);
	DefineStructProperty(float, CylinderHalfHeight);
	DefineStructProperty(float, CylinderRadius);
public:
	uint8 Padding[0x14];
};

class UAthenaPetCarrierItemDefinition : public UAthenaBackpackItemDefinition {
public:
	DefineUnrealClass(UAthenaPetCarrierItemDefinition);

	DefineUProperty(UAthenaPetItemDefinition*, DefaultPet);
	DefineUProperty(FFortUICameraFrameTargetBounds, CameraFramingBounds);
};
