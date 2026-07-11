#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Core/Public/Math/UnrealMathUtility.h"

int32 UKismetMathLibrary::RandomIntegerInRange(int32 InMin, int32 InMax)
{
	return FMath::RandRange(InMin, InMax);
}

float UKismetMathLibrary::RandomFloatInRange(float InMin, float InMax)
{
	return FMath::FRandRange(InMin, InMax);
}

float UKismetMathLibrary::RandomFloat()
{
	return FMath::FRand();
}

int32 UKismetMathLibrary::Min(int32 A, int32 B)
{
	return FMath::Min(A, B);
}

float UKismetMathLibrary::Min(float A, float B)
{
	return FMath::Min(A, B);
}

int32 UKismetMathLibrary::Max(int32 A, int32 B)
{
	return FMath::Max(A, B);
}

float UKismetMathLibrary::Max(float A, float B)
{
	return FMath::Max(A, B);
}

int32 UKismetMathLibrary::Clamp(int32 Value, int32 InMin, int32 InMax)
{
	return FMath::Clamp(Value, InMin, InMax);
}

float UKismetMathLibrary::Clamp(float Value, float InMin, float InMax)
{
	return FMath::Clamp(Value, InMin, InMax);
}

int32 UKismetMathLibrary::FTrunc(float A)
{
	return FMath::TruncToInt(A);
}

int32 UKismetMathLibrary::Round(float A)
{
	return FMath::RoundToInt(A);
}

float UKismetMathLibrary::Lerp(float A, float B, float Alpha)
{
	return FMath::Lerp(A, B, Alpha);
}

FVector UKismetMathLibrary::GetForwardVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetForwardVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}

FVector UKismetMathLibrary::GetRightVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetRightVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}

FVector UKismetMathLibrary::GetUpVector(const FRotator& InRot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.GetUpVector");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return GetDefaultObj()->Call<FVector>(Func, InRot);
}
