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
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ReadyToStartMatch"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ReadyToStartMatchInternal)(AGameMode*) = decltype(ReadyToStartMatchInternal)(VTable[VTableIdx]);
		ReadyToStartMatchInternal(this);
	}
}

void AGameMode::Say(const FString& Msg)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"Say"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&SayInternal)(AGameMode*, const FString&) = decltype(SayInternal)(VTable[VTableIdx]);
		SayInternal(this, Msg);
	}
}

bool AGameMode::IsMatchInProgress() const
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"IsMatchInProgress"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&IsMatchInProgressInternal)(const AGameMode*) = decltype(IsMatchInProgressInternal)(VTable[VTableIdx]);
		IsMatchInProgressInternal(this);
	}
}

bool AGameMode::HasMatchEnded() const
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"HasMatchEnded"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&HasMatchEndedInternal)(const AGameMode*) = decltype(HasMatchEndedInternal)(VTable[VTableIdx]);
		HasMatchEndedInternal(this);
	}
}