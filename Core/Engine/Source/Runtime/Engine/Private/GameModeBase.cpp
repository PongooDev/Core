#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

APawn* AGameModeBase::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SpawnDefaultPawnFor"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&SpawnDefaultPawnForInternal)(AGameModeBase*, AController*, AActor*) = decltype(SpawnDefaultPawnForInternal)(VTable[VTableIdx]);
		SpawnDefaultPawnForInternal(this, NewPlayer, StartSpot);
	}
}

APawn* AGameModeBase::SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SpawnDefaultPawnAtTransform"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&SpawnDefaultPawnAtTransformInternal)(AGameModeBase*, AController*, const FTransform&) = decltype(SpawnDefaultPawnAtTransformInternal)(VTable[VTableIdx]);
		SpawnDefaultPawnAtTransformInternal(this, NewPlayer, SpawnTransform);
	}
}

void AGameModeBase::RestartPlayer(AController* NewPlayer)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"RestartPlayer"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&RestartPlayerInternal)(AGameModeBase*, AController*) = decltype(RestartPlayerInternal)(VTable[VTableIdx]);
		RestartPlayerInternal(this, NewPlayer);
	}
}

void AGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	void (*&FinishRestartPlayerInternal)(AGameModeBase*, AController*, const FRotator&) = decltype(FinishRestartPlayerInternal)(VTable[Finder::FindAGameModeBase_FinishRestartPlayerVFT()]);
	FinishRestartPlayerInternal(this, NewPlayer, StartRotation);
}

void AGameModeBase::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"HandleStartingNewPlayer"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&HandleStartingNewPlayerInternal)(AGameModeBase*, APlayerController*) = decltype(HandleStartingNewPlayerInternal)(VTable[VTableIdx]);
		HandleStartingNewPlayerInternal(this, NewPlayer);
	}
}