#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"

#include "FortPawn.h"

class UFortHero;

class AFortPlayerPawn : public AFortPawn {
public:
	DefineUnrealClass(AFortPlayerPawn);

	DefineUProperty(float, PickupSpeedMultiplier);

	DefineUProperty(TWeakObjectPtr<UFortHero>, Hero);
public:
	void BeginSkydiving(bool bFromBus);

	void ForceFinishIncomingPickups();

	void ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, uint8 Part);

	void RandomizeCharacter(const FString& GenderString);

	static inline void (*ServerReviveFromDBNOOG)(AFortPlayerPawn* This, AController* EventInstigator);
	static void ServerReviveFromDBNO(AFortPlayerPawn* This, AController* EventInstigator);

	static inline void (*ServerHandlePickupOG)(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);
	static void ServerHandlePickup(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound);

	static inline void (*execOnCapsuleBeginOverlapOG)(UObject* Context, FFrame& Stack);
	static void execOnCapsuleBeginOverlap(AFortPlayerPawn* Context, FFrame& Stack);
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AFortPickup* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult& SweepResult);

	void TryToAutoPickup(AFortPickup* Pickup);

	void TryToAutoPickupWeapon(AFortPickup* Pickup);

	static void Hook() {
		HookEveryVTable(
			AFortPlayerPawn::StaticClass(),
			AFortPlayerPawn::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawn.ServerReviveFromDBNO"),
			ServerReviveFromDBNO,
			(LPVOID*)&ServerReviveFromDBNOOG
		);

		HookEveryVTable(
			AFortPlayerPawn::StaticClass(),
			AFortPlayerPawn::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawn.ServerHandlePickup"),
			ServerHandlePickup,
			(LPVOID*)&ServerHandlePickupOG
		);

		if (Version::Fortnite_Version >= 1.82) {
			ExecHook("Function /Script/FortniteGame.FortPlayerPawnAthena.OnCapsuleBeginOverlap", execOnCapsuleBeginOverlap, execOnCapsuleBeginOverlapOG);
		}
		
		Log("AFortPlayerPawn Hooked!");
	}
};