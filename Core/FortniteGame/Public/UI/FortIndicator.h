#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortIndicator : public UObject {
public:
	DefineUnrealClass(UFortIndicator);

	DefineUProperty(FVector, RelativeLocation);
	DefineUProperty(float, MaxDistance);
	DefineUProperty(bool, bMoveWithPawnState);
	DefineUProperty(FName, AttachedSocketName);
};
