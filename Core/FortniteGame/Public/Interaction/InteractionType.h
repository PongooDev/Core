#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"

#include "FortniteGame/Public/FortEnums.h"

class AFortPlayerPawn;

struct FInteractionType {
public:
	DefineUnrealStruct(FInteractionType);

	DefineStructProperty(uint8, InteractionType);
	DefineStructProperty(TWeakObjectPtr<AFortPlayerPawn>, RequestingPawn);
};