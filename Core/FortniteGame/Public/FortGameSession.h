#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"


class FUniqueNetIdRepl;
class FText;

enum EFortPlayerValidationType : uint8
{
	NotValidated = 0,
	ValidatedPlayer = 1,
	ValidatedSpectator = 2,
};

class AFortGameSession : public AGameSession {
public:
	DefineUnrealClass(AFortGameSession);

	DefineUProperty(float, DisconnectedReservationTimeout);
public:
	static inline EFortPlayerValidationType(*ValidatePlayerOG)(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason);
	static EFortPlayerValidationType ValidatePlayer(AFortGameSession* This, const FUniqueNetIdRepl* UniqueId, bool bIsLocalPlayer, FText* ReturnReason);

	void CreateServerGame();

	static void Hook();
};
