#include "pch.h"
#include "FortniteGame/Public/FortGameSession/FortGameSession.h"

EFortPlayerValidationType AFortGameSession::ValidatePlayer(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason) {
	if (Version::Fortnite_Version == 1.72) {
		return EFortPlayerValidationType::ValidatedPlayer;
	}

	return ValidatePlayerOG(This, UniqueId, bIsLocalPlayer, ReturnReason);
}