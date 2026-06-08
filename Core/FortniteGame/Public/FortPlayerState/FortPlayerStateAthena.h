#pragma once
#include "pch.h"

#include "FortPlayerStateZone.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

class AFortPlayerStateAthena final : public AFortPlayerStateZone {
public:
	DefineUnrealClass(AFortPlayerStateAthena);

	DefineUProperty(uint8, TeamIndex);
	DefineUProperty(int32, SecondsAlive);
	DefineUProperty(int32, KillScore);
	DefineUProperty(int32, TeamKillScore);
	DefineUProperty(int32, Place);
public:
	static void Hook() {
		Log("AFortPlayerStateAthena Hooked!");
	}
};