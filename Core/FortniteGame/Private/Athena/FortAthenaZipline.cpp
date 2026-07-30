#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaZipline.h"

void AFortAthenaZipline::Initialize(const FVector& NewStartPosition, const FVector& NewEndPosition)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Initialize");

	if (!Func) {
		Log("AFortAthenaZipline::Initialize: Failed to find function!");
		return;
	}

	return Call<void>(Func, NewStartPosition, NewEndPosition);
}

bool AFortAthenaZipline::UseInteractToEnterZipline()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("UseInteractToEnterZipline");

	if (!Func) {
		Log("AFortAthenaZipline::UseInteractToEnterZipline: Failed to find function!");
		return false;
	}

	return Call<bool>(Func);
}
