#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/PackageMapClient.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Package.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

APawn* AGameModeBase::SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SpawnDefaultPawnFor");

	return const_cast<AGameModeBase*>(this)->Call<APawn*>(Func, NewPlayer, StartSpot);
}

APawn* AGameModeBase::SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SpawnDefaultPawnAtTransform");

	return const_cast<AGameModeBase*>(this)->Call<APawn*>(Func, NewPlayer, SpawnTransform);
}

void AGameModeBase::RestartPlayer(AController* NewPlayer)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RestartPlayer");

	return const_cast<AGameModeBase*>(this)->Call<void>(Func, NewPlayer);
}

void AGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	void (*&FinishRestartPlayerInternal)(AGameModeBase*, AController*, const FRotator&) = decltype(FinishRestartPlayerInternal)(VTable[Finder::FindAGameModeBase_FinishRestartPlayerVFT()]);
	FinishRestartPlayerInternal(this, NewPlayer, StartRotation);
}

void AGameModeBase::HandleStartingNewPlayer(APlayerController* NewPlayer)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleStartingNewPlayer");

	return const_cast<AGameModeBase*>(this)->Call<void>(Func, NewPlayer);
}

AActor* AGameModeBase::ChoosePlayerStart(AController* Player)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ChoosePlayerStart");

	return const_cast<AGameModeBase*>(this)->Call<AActor*>(Func, Player);
}

void AGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	static UFunction* Func = nullptr;
	if (Func == nullptr)
		Func = FindFunction("RestartPlayerAtPlayerStart");
	
	return const_cast<AGameModeBase*>(this)->Call<void>(Func, NewPlayer, StartSpot);
}

void AGameModeBase::RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RestartPlayerAtTransform");

	return const_cast<AGameModeBase*>(this)->Call<void>(Func, NewPlayer, SpawnTransform);
}

APlayerController* AGameModeBase::SpawnPlayerController(ENetRole InRemoteRole, FVector const& SpawnLocation, FRotator const& SpawnRotation)
{
	APlayerController* (*&SpawnPlayerControllerInternal)(AGameModeBase*, ENetRole, const FVector&, const FRotator&) = decltype(SpawnPlayerControllerInternal)(VTable[Finder::FindAGameModeBase_SpawnPlayerControllerVFT()]);
	return SpawnPlayerControllerInternal(this, InRemoteRole, SpawnLocation, SpawnRotation);
}

void AGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	void (*&InitGameInternal)(AGameModeBase*, const FString&, const FString&, FString&) = decltype(InitGameInternal)(VTable[Finder::FindAGameModeBase_InitGameVFT()]);
	InitGameInternal(this, MapName, Options, ErrorMessage);
}

TSubclassOf<AGameSession>* AGameModeBase::GetGameSessionClass() const
{
	TSubclassOf<AGameSession> GameSessionClass;
	TSubclassOf<AGameSession> (*&GetGameSessionClassInternal)(const AGameModeBase*, TSubclassOf<AGameSession>*) = decltype(GetGameSessionClassInternal)(VTable[Finder::FindAGameModeBase_GetGameSessionClassVFT()]);
	GetGameSessionClassInternal(this, &GameSessionClass);
	return &GameSessionClass;
}

void AGameModeBase::ProcessServerTravel(AGameModeBase* This, FString& URL, bool bAbsolute)
{
	Log("ProcessServerTravel: " + URL.ToString());

	/*This->StartToLeaveMap();

	bool bSeamless = (This->bUseSeamlessTravel && This->GetWorld()->TimeSeconds < 172800.0f);

	FString NextMap;
	if (UKismetStringLibrary::Contains(UKismetStringLibrary::ToUpper(URL), "?RESTART", true, false))
	{
		NextMap = UWorld::RemovePIEPrefix(This->GetOutermost()->GetName());
	}
	else
	{
		int32 OptionStart = UKismetStringLibrary::FindSubstring(URL, TEXT("?"), true, false, 0);
		if (OptionStart == INDEX_NONE)
		{
			NextMap = URL;
		}
		else
		{
			NextMap = UKismetStringLibrary::Left(URL, OptionStart);
		}
	}

	FGuid NextMapGuid = UEngine::GetPackageGuid(FName(*NextMap), GetWorld()->IsPlayInEditor());

	// Notify clients we're switching level and give them time to receive.
	FString URLMod = URL;
	APlayerController* LocalPlayer = ProcessClientTravel(URLMod, NextMapGuid, bSeamless, bAbsolute);

	UWorld* World = This->GetWorld();
	check(World);
	World->NextURL = URL;
	ENetMode NetMode = InternalGetNetMode(This);

	if (bSeamless)
	{
		World->SeamlessTravel(World->NextURL, bAbsolute);
		World->NextURL = TEXT("");
	}
	else if (NetMode != NM_DedicatedServer && NetMode != NM_ListenServer)
	{
		World->NextSwitchCountdown = 0.0f;
	}*/
}

void AGameModeBase::StartToLeaveMap()
{

}