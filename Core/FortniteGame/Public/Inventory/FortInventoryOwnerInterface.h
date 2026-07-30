#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Interface.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

class IFortInventoryOwnerInterface : public IInterface {
public:
	DefineUnrealClass(IFortInventoryOwnerInterface);
};