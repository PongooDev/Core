#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"

APlayerController* UGameInstance::GetFirstLocalPlayerController(const UWorld* World) const
{
	APlayerController* (*GetFirstLocalPlayerControllerInternal)(const UGameInstance*, const UWorld*) = decltype(GetFirstLocalPlayerControllerInternal)(ImageBase + Finder::FindUGameInstance_GetFirstLocalPlayerController());
	return GetFirstLocalPlayerControllerInternal(this, World);
}

void UGameInstance::CleanupGameViewport()
{
	void (*CleanupGameViewportInternal)(UGameInstance*) = decltype(CleanupGameViewportInternal)(ImageBase + Finder::FindUGameInstance_CleanupGameViewport());
	CleanupGameViewportInternal(this);
}