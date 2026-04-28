#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class ULocalPlayer;
class UOnlineSession;
class AGameModeBase;
class APlayerController;
class UWorld;

class UGameInstance : public UObject {
public:
	DefineUnrealClass(UGameInstance);

	DefineUProperty(TArray<ULocalPlayer*>, LocalPlayers);
	DefineUProperty(UOnlineSession*, OnlineSession);
	DefineUProperty(TArray<UObject>, ReferencedObjects);
public:
	APlayerController* GetFirstLocalPlayerController(const UWorld* World = nullptr) const;

	void CleanupGameViewport();
};