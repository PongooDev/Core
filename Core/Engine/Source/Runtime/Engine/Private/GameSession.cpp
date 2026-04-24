#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"

void AGameSession::InitOptions(const FString& Options)
{
	void (*InitOptionsInternal)(AGameSession*, const FString&) = decltype(InitOptionsInternal)(ImageBase + Finder::FindAGameSession_InitOptions());
	InitOptionsInternal(this, Options);
}

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

bool AGameSession::ProcessAutoLogin()
{
	bool (*ProcessAutoLoginInternal)(AGameSession*) = decltype(ProcessAutoLoginInternal)(ImageBase + Finder::FindAGameSession_ProcessAutoLogin());
	return ProcessAutoLoginInternal(this);
}

void AGameSession::OnAutoLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FString& Error)
{
	void (*OnAutoLoginCompleteInternal)(AGameSession*, int32, bool, const FString&) = decltype(OnAutoLoginCompleteInternal)(ImageBase + Finder::FindAGameSession_OnAutoLoginComplete());
	OnAutoLoginCompleteInternal(this, LocalUserNum, bWasSuccessful, Error);
}

FString AGameSession::ApproveLogin(const FString& Options)
{
	FString(*ApproveLoginInternal)(const FString&) = decltype(ApproveLoginInternal)(ImageBase + Finder::FindAGameSession_ApproveLogin());
	return ApproveLoginInternal(Options);
}

void AGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetId& UniqueId, bool bWasFromInvite)
{
	void (*RegisterPlayerInternal)(AGameSession*, APlayerController*, const FUniqueNetId&, bool) = decltype(RegisterPlayerInternal)(ImageBase + Finder::FindAGameSession_RegisterPlayer());
	RegisterPlayerInternal(this, NewPlayer, UniqueId, bWasFromInvite);
}

bool AGameSession::AtCapacity(bool bSpectator)
{
	bool (*AtCapacityInternal)(AGameSession*, bool) = decltype(AtCapacityInternal)(ImageBase + Finder::FindAGameSession_AtCapacity());
	return AtCapacityInternal(this, bSpectator);
}

bool AGameSession::GetSessionJoinability(FName InSessionName, FJoinabilitySettings& OutSettings)
{
	bool (*GetSessionJoinabilityInternal)(AGameSession*, FName, FJoinabilitySettings&) = decltype(GetSessionJoinabilityInternal)(ImageBase + Finder::FindAGameSession_GetSessionJoinability());
	return GetSessionJoinabilityInternal(this, InSessionName, OutSettings);
}

void AGameSession::UpdateSessionJoinability(FName InSessionName, bool bPublicSearchable, bool bAllowInvites, bool bJoinViaPresence, bool bJoinViaPresenceFriendsOnly)
{
	void (*UpdateSessionJoinabilityInternal)(AGameSession*, FName, bool, bool, bool, bool) = decltype(UpdateSessionJoinabilityInternal)(ImageBase + Finder::FindAGameSession_UpdateSessionJoinability());
	UpdateSessionJoinabilityInternal(this, InSessionName, bPublicSearchable, bAllowInvites, bJoinViaPresence, bJoinViaPresenceFriendsOnly);
}

bool AGameSession::KickPlayer(AGameSession* This, APlayerController* KickedPlayer, const FText& KickReason)
{
	Log("AGameSession::KickPlayer called");
	return false;
}

void AGameSession::ReturnToMainMenuHost()
{
	void (*ReturnToMainMenuHostInternal)(AGameSession*) = decltype(ReturnToMainMenuHostInternal)(ImageBase + Finder::FindAGameSession_ReturnToMainMenuHost());
	ReturnToMainMenuHostInternal(this);
}

void AGameSession::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	void (*UnregisterPlayerInternal)(AGameSession*, FName, const FUniqueNetIdRepl&) = decltype(UnregisterPlayerInternal)(ImageBase + Finder::FindAGameSession_UnregisterPlayer_1());
	UnregisterPlayerInternal(this, InSessionName, UniqueId);
}

void AGameSession::UnregisterPlayers(FName InSessionName, const TArray<const FUniqueNetId>& Players)
{
	void (*UnregisterPlayersInternal)(AGameSession*, FName, const TArray<const FUniqueNetId>&) = decltype(UnregisterPlayersInternal)(ImageBase + Finder::FindAGameSession_UnregisterPlayers());
	UnregisterPlayersInternal(this, InSessionName, Players);
}

void AGameSession::UnregisterPlayer(const APlayerController* ExitingPlayer)
{
	void (*UnregisterPlayerInternal)(AGameSession*, const APlayerController*) = decltype(UnregisterPlayerInternal)(ImageBase + Finder::FindAGameSession_UnregisterPlayer());
	UnregisterPlayerInternal(this, ExitingPlayer);
}

APlayerController* GetPlayerControllerFromNetId(UWorld* World, const FUniqueNetId& PlayerNetId)
{
	APlayerController* (*GetPlayerControllerFromNetIdInternal)(UWorld*, const FUniqueNetId&) = decltype(GetPlayerControllerFromNetIdInternal)(ImageBase + Finder::FindAGameSession_GetPlayerControllerFromNetId());
	return GetPlayerControllerFromNetIdInternal(World, PlayerNetId);
}