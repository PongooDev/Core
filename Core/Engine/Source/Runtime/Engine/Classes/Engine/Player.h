#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class APlayerController;
class UWorld;

class UPlayer : public UObject {
public:
	DefineUnrealClass(UPlayer);

	DefineUProperty(APlayerController*, PlayerController);

	DefineUProperty(int32, CurrentNetSpeed);

	DefineUProperty(int32, ConfiguredInternetSpeed);

	DefineUProperty(int32, ConfiguredLanSpeed);
public:
	APlayerController* GetPlayerController(UWorld* InWorld) const;
};