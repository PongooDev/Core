#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Athena/Modifiers/FortGameplayMutator.h"

class AFortGameplayMutator_AILevelVariance : public AFortGameplayMutator {
public:
	DefineUnrealClass(AFortGameplayMutator_AILevelVariance);

	DefineUProperty(float, MinVariance);
	DefineUProperty(float, MaxVariance);
};
