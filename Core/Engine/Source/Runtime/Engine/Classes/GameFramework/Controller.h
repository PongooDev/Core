#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"

class ACharacter;
class APawn;
class APlayerState;
class FDebugDisplayInfo;
class UDamageType;

class AController : public AActor {
public:
	DefineUnrealClass(AController);
public:
	DefineUProperty(APlayerState*, PlayerState);
	DefineUProperty(APawn*, Pawn);
	DefineUProperty(FName, StateName);
public:
	void Possess(APawn* InPawn);
	void PossessVFT(APawn* InPawn);

	void UnPossess();
	void UnPossessVFT();

	APawn* K2_GetPawn() const;

	AActor* GetViewTarget() const;

	void InitPlayerState();

	void OnRep_Pawn();

	void OnRep_PlayerState();

	static inline void (*GetPlayerViewPointOG)(AController* This, FVector& out_Location, FRotator& out_Rotation);

	FRotator GetControlRotation() const;

	static void Hook() {
		CreateVTableOriginal(AController::GetDefaultObj(), Finder::FindAController_GetPlayerViewPointVFT(), (LPVOID*)&GetPlayerViewPointOG);

		Log("Hooked AController");
	}
};