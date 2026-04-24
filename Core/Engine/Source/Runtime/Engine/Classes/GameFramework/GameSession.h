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
	void InitOptions(const FString& Options);

	int32 GetNextPlayerID();

	bool ProcessAutoLogin();

	void OnAutoLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& Error);

	FString ApproveLogin(const FString& Options);

	void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetId& UniqueId, bool bWasFromInvite);

	bool AtCapacity(bool bSpectator);

	bool GetSessionJoinability(FName InSessionName, FJoinabilitySettings& OutSettings);

	void UpdateSessionJoinability(FName InSessionName, bool bPublicSearchable, bool bAllowInvites, bool bJoinViaPresence, bool bJoinViaPresenceFriendsOnly);

	static inline bool (*KickPlayerOG)(AGameSession* This, APlayerController* KickedPlayer, const FText& KickReason);
	static bool KickPlayer(AGameSession* This, APlayerController* KickedPlayer, const FText& KickReason);

	void ReturnToMainMenuHost();

	void UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId);
	void UnregisterPlayers(FName InSessionName, const TArray<const FUniqueNetId>& Players);

	void UnregisterPlayer(const APlayerController* ExitingPlayer);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAGameSession_KickPlayer()), KickPlayer, (LPVOID*)&KickPlayerOG);

		Log("Hooked AGameSession");
	}
};

APlayerController* GetPlayerControllerFromNetId(UWorld* World, const FUniqueNetId& PlayerNetId);