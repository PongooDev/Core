#include "pch.h"
#include "FortniteGame/Public/Capture/AthenaBigBaseWall.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

void AAthenaBigBaseWall::OnRep_BarrierState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_BarrierState");

	if (!Func) {
		Log("AAthenaBigBaseWall::OnRep_BarrierState: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBigBaseWall::OnRep_ResetBool()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_ResetBool");

	if (!Func) {
		Log("AAthenaBigBaseWall::OnRep_ResetBool: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBigBaseWall::OnRep_TimeUntilWallComesDown()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TimeUntilWallComesDown");

	if (!Func) {
		Log("AAthenaBigBaseWall::OnRep_TimeUntilWallComesDown: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBigBaseWall::OnRep_WallGravity()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WallGravity");

	if (!Func) {
		Log("AAthenaBigBaseWall::OnRep_WallGravity: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

AAthenaBigBaseWall* AAthenaBigBaseWall::CreateWall(UWorld* World, TSubclassOf<AAthenaBigBaseWall> BigBaseWallClass, const FVector& MidlineStart, const FVector& MidlineEnd, float ZLevel)
{
    if (!World || !BigBaseWallClass)
        return nullptr;

    FVector Direction = MidlineEnd - MidlineStart;
    FRotator WallRotation = Direction.ToOrientationRotator();
    WallRotation.Pitch = 0.0;
    WallRotation.Roll = 0.0;

    FVector WallLocation = MidlineStart + Direction * 0.5;
    WallLocation.Z = (double)ZLevel;

	AAthenaBigBaseWall* NewWall = (AAthenaBigBaseWall*)World->SpawnActorUnfinished(BigBaseWallClass.Get(), WallLocation, WallRotation, nullptr);

    if (NewWall)
    {
		FTransform FinalTransform(WallRotation, WallLocation, FVector(1.f, 1.f, 1.f));
		UGameplayStatics::FinishSpawningActor(NewWall, FinalTransform);
    }

    return NewWall;
}