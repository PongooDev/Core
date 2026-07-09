#pragma once
#include "pch.h"

#include "GameModeBase.h"
#include "../Kismet/KismetStringLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

class APlayerState;
class ULocalMessage;
class UNetDriver;
class AController;

struct FVector;
struct FRotator;

namespace MatchState
{
	extern const FName EnteringMap;			// We are entering this map, actors are not yet ticking
	extern const FName WaitingToStart;		// Actors are ticking, but the match has not yet started
	extern const FName InProgress;			// Normal gameplay is occurring. Specific games will have their own state machine inside this state
	extern const FName WaitingPostMatch;		// Match has ended so we aren't accepting new players, but actors are still ticking
	extern const FName LeavingMap;			// We are transitioning out of the map to another location
	extern const FName Aborted;				// Match has failed due to network issues or other problems, cannot continue
}

class AGameMode : public AGameModeBase {
public:
	DefineUnrealClass(AGameMode);
public:
	DefineUProperty(FName, MatchState);

	DefineUProperty(int32, NumSpectators);

	DefineUProperty(int32, NumPlayers);
	DefineUProperty(int32, NumBots);

	DefineUProperty(int32, NumTravellingPlayers);
public:
	bool ReadyToStartMatch();

	void Say(const FString& Msg);

	FName GetMatchState() const {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetMatchState"));

		return const_cast<AGameMode*>(this)->Call<FName>(Func);
	}

	bool IsMatchInProgress() const;

	bool HasMatchEnded() const;

	static inline void (*AddInactivePlayerOG)(AGameMode* This, APlayerState* PlayerState, APlayerController* PC);
	void AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC);

	static void Hook() {
		CreateVTableOriginal(AGameMode::GetDefaultObj(), Finder::FindAGameMode_AddInactivePlayerVFT(), (LPVOID*)&AddInactivePlayerOG);

		Log("AGameMode Hooked!");
	}
};