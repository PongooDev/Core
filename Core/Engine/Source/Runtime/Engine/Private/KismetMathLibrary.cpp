#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

int32 UKismetMathLibrary::RandomIntegerInRange(int32 Min, int32 Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomIntegerInRange");

	if (!Func) {
		Log("UKismetMathLibrary::RandomIntegerInRange: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, Min, Max);
}

float UKismetMathLibrary::RandomFloatInRange(float Min, float Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomFloatInRange");

	if (!Func) {
		Log("UKismetMathLibrary::RandomFloatInRange: Failed to find function!");
		return 0.0f;
	}

	return GetDefaultObj()->Call<float>(Func, Min, Max);
}

int32 UKismetMathLibrary::Min(int32 A, int32 B)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Min");

	if (!Func) {
		Log("UKismetMathLibrary::Min: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, A, B);
}

int32 UKismetMathLibrary::FTrunc(float A)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.FTrunc");

	if (!Func) {
		Log("UKismetMathLibrary::FTrunc: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, A);
}

int32 UKismetMathLibrary::Max(int32 A, int32 B)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Max");

	if (!Func) {
		Log("UKismetMathLibrary::Max: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, A, B);
}

int32 UKismetMathLibrary::Clamp(int32 Value, int32 Min, int32 Max)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Clamp");

	if (!Func) {
		Log("UKismetMathLibrary::Clamp: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, Value, Min, Max);
}

float UKismetMathLibrary::Lerp(float A, float B, float Alpha)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Lerp");

	if (!Func) {
		Log("UKismetMathLibrary::Lerp: Failed to find function!");
		return 0.0f;
	}

	return GetDefaultObj()->Call<float>(Func, A, B, Alpha);
}

int32 UKismetMathLibrary::Round(float A)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.Round");

	if (!Func) {
		Log("UKismetMathLibrary::Round: Failed to find function!");
		return 0;
	}

	return GetDefaultObj()->Call<int32>(Func, A);
}

float UKismetMathLibrary::RandomFloat()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetMathLibrary.RandomFloat");

	if (!Func) {
		Log("UKismetMathLibrary::RandomFloat: Failed to find function!");
		return 0.0f;
	}

	return GetDefaultObj()->Call<float>(Func);
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