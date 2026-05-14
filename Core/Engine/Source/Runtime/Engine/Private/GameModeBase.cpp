#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

APawn* AGameModeBase::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SpawnDefaultPawnFor"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		APawn* (*&SpawnDefaultPawnForInternal)(AGameModeBase*, AController*, AActor*) = decltype(SpawnDefaultPawnForInternal)(VTable[VTableIdx]);
		return SpawnDefaultPawnForInternal(this, NewPlayer, StartSpot);
	}

	return nullptr;
}

APawn* AGameModeBase::SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"SpawnDefaultPawnAtTransform"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		APawn* (*&SpawnDefaultPawnAtTransformInternal)(AGameModeBase*, AController*, const FTransform&) = decltype(SpawnDefaultPawnAtTransformInternal)(VTable[VTableIdx]);
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

AActor* AGameModeBase::ChoosePlayerStart(AController* Player)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ChoosePlayerStart"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);
		AActor* (*&ChoosePlayerStartInternal)(AGameModeBase*, AController*) = decltype(ChoosePlayerStartInternal)(VTable[VTableIdx]);
		return ChoosePlayerStartInternal(this, Player);
	}

	return nullptr;
}

void AGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"RestartPlayerAtPlayerStart"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);
		void (*&RestartPlayerAtPlayerStartInternal)(AGameModeBase*, AController*, AActor*) = decltype(RestartPlayerAtPlayerStartInternal)(VTable[VTableIdx]);
		RestartPlayerAtPlayerStartInternal(this, NewPlayer, StartSpot);
	}
}

void AGameModeBase::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"RestartPlayerAtTransform"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);
		void (*&RestartPlayerAtTransformInternal)(AGameModeBase*, AController*, const FTransform&) = decltype(RestartPlayerAtTransformInternal)(VTable[VTableIdx]);
		RestartPlayerAtTransformInternal(this, NewPlayer, SpawnTransform);
	}
}

APlayerController* AGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation)
{
	APlayerController* (*&SpawnPlayerControllerInternal)(AGameModeBase*, ENetRole, const FVector&, const FRotator&) = decltype(SpawnPlayerControllerInternal)(VTable[Finder::FindAGameModeBase_SpawnPlayerControllerVFT()]);
	return SpawnPlayerControllerInternal(this, InRemoteRole, SpawnLocation, SpawnRotation);
}