#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Player.h"

APlayerController* UPlayer::GetPlayerController(UWorld* InWorld) const
{
	APlayerController* (*GetPlayerControllerInternal)(const UPlayer*, UWorld*) = decltype(GetPlayerControllerInternal)(ImageBase + Finder::FindUPlayer_GetPlayerController());
	return GetPlayerControllerInternal(this, InWorld);
}