#include "pch.h"
#include "FortniteGame/Public/FortGameSessionDedicated.h"
#include "FortniteGame/Public/Mcp/McpProfile.h"

bool AFortGameSessionDedicated::FinalizeCreation() {
	bool (*&FinalizeCreationInternal)(AFortGameSessionDedicated * This) = decltype(FinalizeCreationInternal)(VTable[Finder::FindAFortGameSessionDedicated_FinalizeCreationVFT()]);
	return FinalizeCreationInternal(this);
}

void AFortGameSessionDedicated::OnUpdateComplete(AFortGameSessionDedicated* This, uint8 Result, void* Callback) {
	Log("AFortGameSessionDedicated::OnUpdateComplete Called!");
	return OnUpdateCompleteOG(This, EUpdateCompletionStatus::GetUpdateSuccess(), Callback);
}

void AFortGameSessionDedicated::OnAllPlayersUnregistered() {
	void (*OnAllPlayersUnregisteredInternal)(AFortGameSessionDedicated * This) = decltype(OnAllPlayersUnregisteredInternal)(ImageBase + Finder::FindAFortGameSessionDedicated_OnAllPlayersUnregistered());
	OnAllPlayersUnregisteredInternal(this);
}

void AFortGameSessionDedicated::OnServerConfigurationRequest(const FUniqueNetIdRepl& GameSessionOwner, FEmptyServerReservation& ReservationData) {
	void (*&OnServerConfigurationRequestInternal)(AFortGameSessionDedicated * This, const FUniqueNetIdRepl&, FEmptyServerReservation&) = decltype(OnServerConfigurationRequestInternal)(VTable[Finder::FindAFortGameSessionDedicated_OnServerConfigurationRequestVFT()]);
	OnServerConfigurationRequestInternal(this, GameSessionOwner, ReservationData);
}
