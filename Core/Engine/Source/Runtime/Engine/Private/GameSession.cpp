#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"

int32 AGameSession::GetNextPlayerID()
{
	if (Finder::FindAGameSession__NextPlayerID()) {
		int32* NextPlayerID = (int32*)(ImageBase + Finder::FindAGameSession__NextPlayerID());
		return (*NextPlayerID)++;
	}
	else {
		// Start at 256, because 255 is special (means all team for some UT Emote stuff)
		static int32 NextPlayerID = 256;
		return NextPlayerID++;
	}
}

bool AGameSession::KickPlayer(APlayerController* KickedPlayer, const FText& KickReason)
{
	bool (*&KickPlayerInternal)(AGameSession*, APlayerController*, const FText&) = decltype(KickPlayerInternal)(VTable[Finder::FindAGameSession_KickPlayerVFT()]);
	return KickPlayerInternal(this, KickedPlayer, KickReason);
}

APlayerController* GetPlayerControllerFromNetId(UWorld* World, const FUniqueNetId& PlayerNetId)
{
	APlayerController* (*GetPlayerControllerFromNetIdInternal)(UWorld*, const FUniqueNetId&) = decltype(GetPlayerControllerFromNetIdInternal)(ImageBase + Finder::FindAGameSession_GetPlayerControllerFromNetId());
	return GetPlayerControllerFromNetIdInternal(World, PlayerNetId);
}