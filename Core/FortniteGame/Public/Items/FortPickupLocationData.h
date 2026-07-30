#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortItemDefinition;
class AFortPawn;
class AFortPickup;
class AFortPickupEffect;
class IFortInventoryOwnerInterface;

struct FFortPickupLocationData
{
public:
	DefineUnrealStruct(FFortPickupLocationData);

	DefineStructProperty(AFortPawn*, PickupTarget);
	DefineStructProperty(AFortPawn*, ItemOwner);
	DefineStructProperty(FGuid, PickupGuid);
	DefineStructProperty(float, FlyTime);
	DefineStructProperty(FVector, StartDirection);
public:
	uint8 Padding[0x60];
public:
	bool& GetPlayPickupSound() {
		uintptr_t PlayPickupSoundAddr = StaticStruct()->GetPropertyOffset("bPlayPickupSound");
		if (PlayPickupSoundAddr > 0) {
			return *reinterpret_cast<bool*>(uintptr_t(this) + PlayPickupSoundAddr);
		}

		uintptr_t TossStateAddr = StaticStruct()->GetPropertyOffset("TossState");
		if (TossStateAddr > 0) {
			PlayPickupSoundAddr = TossStateAddr + 0x1; // bPlayPickupSound is right after TossState
			return *reinterpret_cast<bool*>(uintptr_t(this) + PlayPickupSoundAddr);
		}

		static bool Dummy = false;
		return Dummy; // Couldnt find!!
	}
};