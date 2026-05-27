#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"

class UWorld;

class UNavigationSystem : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UNavigationSystem);
public:
	static UNavigationSystem* CreateNavigationSystem(UWorld* WorldOwner);
};