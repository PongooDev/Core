#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Quat.h"

struct FReplicatedAthenaVehiclePhysicsState
{
	DefineUnrealStruct(FReplicatedAthenaVehiclePhysicsState);

	DefineStructProperty(FVector, Translation);
	DefineStructProperty(FQuat, Rotation);
	DefineStructProperty(FVector, LinearVelocity);
	DefineStructProperty(FVector, AngularVelocity);
	DefineStructProperty(uint16, SyncKey);

	uint8 Padding[0x50];
};

class AFortAthenaVehicle : public APawn {
public:
	DefineUnrealClass(AFortAthenaVehicle);

	void ServerUpdatePhysicsParams(FReplicatedAthenaVehiclePhysicsState& InState);
	static void execServerUpdatePhysicsParams(AFortAthenaVehicle* Context, FFrame& Stack, void* Result);

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.FortAthenaVehicle.ServerUpdatePhysicsParams", execServerUpdatePhysicsParams);

		Log("AFortAthenaVehicle Hooked!");
	}
};
