#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"

#include "FortniteGame/Public/FortEnums.h"

class FUniqueNetIdRepl;
class FText;

class AFortGameSession : public AGameSession {
public:
	DefineUnrealClass(AFortGameSession);

	DefineUProperty(float, DisconnectedReservationTimeout);
public:
	static inline EFortPlayerValidationType(*ValidatePlayerOG)(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason);
	static EFortPlayerValidationType ValidatePlayer(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason);

	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameSession_ValidatePlayer()), (LPVOID)ValidatePlayer, (LPVOID*)&ValidatePlayerOG);

		Log("Hooked AFortGameSession");
	}
};