#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"

namespace MatchState
{
	const FName EnteringMap = UKismetStringLibrary::Conv_StringToName(TEXT("EnteringMap"));
	const FName WaitingToStart = UKismetStringLibrary::Conv_StringToName(TEXT("WaitingToStart"));
	const FName InProgress = UKismetStringLibrary::Conv_StringToName(TEXT("InProgress"));
	const FName WaitingPostMatch = UKismetStringLibrary::Conv_StringToName(TEXT("WaitingPostMatch"));
	const FName LeavingMap = UKismetStringLibrary::Conv_StringToName(TEXT("LeavingMap"));
	const FName Aborted = UKismetStringLibrary::Conv_StringToName(TEXT("Aborted"));
}

bool AGameMode::ReadyToStartMatch()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ReadyToStartMatch");

	return Call<bool>(Func);
}

void AGameMode::Say(const FString& Msg)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Say");

	Call(Func, Msg);
}

bool AGameMode::IsMatchInProgress() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsMatchInProgress");

	return const_cast<AGameMode*>(this)->Call<bool>(Func);
}

bool AGameMode::HasMatchEnded() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HasMatchEnded");

	return const_cast<AGameMode*>(this)->Call<bool>(Func);
}

void AGameMode::AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC)
{
	void (*&AddInactivePlayerInternal)(AGameMode*, APlayerState*, APlayerController*) = decltype(AddInactivePlayerInternal)(VTable[Finder::FindAGameMode_AddInactivePlayerVFT()]);
	AddInactivePlayerInternal(this, PlayerState, PC);
}