#pragma once
#include "pch.h"

#include "BlueprintFunctionLibrary.h"

#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class UKismetMathLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UKismetMathLibrary);
public:
	/** Return a random integer between Min and Max (>= Min and <= Max) */
	static int32 RandomIntegerInRange(int32 Min, int32 Max);

	/** Generate a random number between Min and Max */
	static float RandomFloatInRange(float Min, float Max);

	static int32 Min(int32 A, int32 B);

	static float Min(float A, float B);

	static int32 FTrunc(float A);

	static int32 Max(int32 A, int32 B);

	static float Max(float A, float B);

	static int32 Clamp(int32 Value, int32 InMin, int32 InMax);

	static float Clamp(float Value, float InMin, float InMax);

	static float Lerp(float A, float B, float Alpha);

	static int32 Round(float A);

	static float RandomFloat();

	static FVector GetForwardVector(const FRotator& InRot);

	static FVector GetRightVector(const FRotator& InRot);

	static FVector GetUpVector(const FRotator& InRot);
};