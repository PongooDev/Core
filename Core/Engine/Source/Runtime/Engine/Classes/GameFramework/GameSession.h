#pragma once
#include "pch.h"

#include "Info.h"
#include "../Engine/World.h"

struct FJoinabilitySettings;
struct FUniqueNetId;
struct FUniqueNetIdRepl;
struct FText;

class AGameSession : public AInfo {
public:
	DefineUnrealClass(AGameSession);
public:
	DefineUProperty(int32, MaxSpectators);

	DefineUProperty(int32, MaxPlayers);

	DefineUProperty(int32, MaxPartySize);

	DefineUProperty(uint8, MaxSplitscreensPerConnection);

	DefineUProperty(bool, bRequiresPushToTalk);

	DefineUProperty(FName, SessionName);
public:
	int32 GetNextPlayerID();

	bool KickPlayer(APlayerController* KickedPlayer, const FText& KickReason);

	static void Hook() {
		Log("Hooked AGameSession");
	}
};

APlayerController* GetPlayerControllerFromNetId(UWorld* World, const FUniqueNetId& PlayerNetId);