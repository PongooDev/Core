#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Actor.h"

class AController;
class APhysicsVolume;
class APlayerController;
class APlayerState;
class FDebugDisplayInfo;
class UCanvas;
class UDamageType;
class UInputComponent;
class UPawnMovementComponent;
class UPawnNoiseEmitterComponent;
class UPlayer;
class UPrimitiveComponent;

class APawn : public AActor {
public:
	DefineUnrealClass(APawn);

	DefineUProperty(AController*, Controller);
};