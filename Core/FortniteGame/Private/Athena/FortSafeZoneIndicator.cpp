#include "pch.h"
#include "FortniteGame/Public/Athena/FortSafeZoneIndicator.h"

FVector AFortSafeZoneIndicator::GetSafeZoneCenter()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetSafeZoneCenter");

	if (Func) {
		return Call<FVector>(Func);
	}

	return K2_GetActorLocation();
}

float AFortSafeZoneIndicator::GetSafeZoneRadius()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetSafeZoneRadius");

	if (Func) {
		return Call<float>(Func);
	}

	return Radius;
}

bool AFortSafeZoneIndicator::IsInCurrentSafeZone(const FVector& TestLocation)
{
	FVector Center = GetSafeZoneCenter();
	float SafeZoneRadius = GetSafeZoneRadius();

	float DX = Center.X - TestLocation.X;
	float DY = Center.Y - TestLocation.Y;

	return SafeZoneRadius * SafeZoneRadius >= DX * DX + DY * DY;
}
