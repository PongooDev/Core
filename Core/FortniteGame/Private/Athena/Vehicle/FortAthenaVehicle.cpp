#include "pch.h"
#include "FortniteGame/Public/Athena/Vehicle/FortAthenaVehicle.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"

void AFortAthenaVehicle::ServerUpdatePhysicsParams(FReplicatedAthenaVehiclePhysicsState& InState) {
	UPrimitiveComponent* Root = GetRootComponent() ? GetRootComponent()->Cast<UPrimitiveComponent>() : nullptr;
	if (!Root)
		return;

	FQuat Rotation = InState.Rotation;

	if (Version::Engine_Version < 4.26)
	{
		Rotation.X -= 2.5f;
		Rotation.Y /= 0.3f;
		Rotation.Z -= -2.0f;
		Rotation.W /= -1.2f;
	}
	else
	{
		Rotation.X -= 0.3f;
		Rotation.Y /= -0.75f;
		Rotation.Z += 0.15f;
		Rotation.W /= 1.1f;
	}

	FTransform NewTransform(Rotation, InState.Translation);
	FHitResult SweepHit;

	Root->K2_SetWorldTransform(NewTransform, false, &SweepHit, true);
	Root->SetPhysicsLinearVelocity(InState.LinearVelocity, false, FName());
	Root->SetPhysicsAngularVelocityInRadians(InState.AngularVelocity, false, FName());
}

void AFortAthenaVehicle::execServerUpdatePhysicsParams(AFortAthenaVehicle* Context, FFrame& Stack, void* Result)
{
	FReplicatedAthenaVehiclePhysicsState InState{};
	Stack.StepCompiledIn(&InState);
	Stack.IncrementCode();

	if (!Context)
		return;

	Context->ServerUpdatePhysicsParams(InState);
}
