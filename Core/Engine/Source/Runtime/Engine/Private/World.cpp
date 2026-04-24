#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

UWorld* UWorld::GetWorld() {
	if (ServerOffsets::GWorld != 0)
		return *reinterpret_cast<UWorld**>(ImageBase + ServerOffsets::GWorld);

	if (UEngine* Engine = UEngine::GetEngine())
	{
		if (!Engine->GameViewport)
			return nullptr;

		return Engine->GameViewport->World;
	}

	return nullptr;
}

ENetMode UWorld::InternalGetNetMode(UWorld* This)
{
	return NM_DedicatedServer;
}

AActor* UWorld::SpawnActor(UClass* Class, FVector Location, FRotator Rotation, AActor* Owner) {
	FTransform Transform;

	Transform.Translation = Location;
	Transform.Rotation = FQuat(Rotation);
	Transform.Scale3D.X = 1.f;
	Transform.Scale3D.Y = 1.f;
	Transform.Scale3D.Z = 1.f;

	return SpawnActor(Class, Transform, Owner);
}

AActor* UWorld::SpawnActor(UClass* Class, FTransform Transform, AActor* Owner) {
	auto Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner);
	if (Actor) {
		return UGameplayStatics::FinishSpawningActor(Actor, Transform);
	}
	else {
		return nullptr;
	}
}

bool UWorld::ServerTravel(const FString& FURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	bool (*ServerTravelInternal)(UWorld*, const FString&, bool, bool) = decltype(ServerTravelInternal)(ImageBase + Finder::FindUWorld_ServerTravel());
	return ServerTravelInternal(this, FURL, bAbsolute, bShouldSkipGameNotify);
}

bool UWorld::Listen(FURL& InURL)
{
	UEngine* Engine = UEngine::GetEngine();
	FName NAME_GameNetDriver = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");

	if (NetDriver)
	{
		Engine->BroadcastNetworkFailure(this, NetDriver, ENetworkFailure::NetDriverAlreadyExists);
		return false;
	}

	// Create net driver.
	if (Engine->CreateNamedNetDriver(this, NAME_GameNetDriver, NAME_GameNetDriver)) {
		NetDriver = Engine->FindNamedNetDriver(this, NAME_GameNetDriver);

		NetDriver->SetWorld(this);
		FLevelCollection* const SourceCollection = FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(NetDriver);
		}
		FLevelCollection* const StaticCollection = FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(NetDriver);
		}
	}

	if (Version::Engine_Version == 4.16) {
		if (NetDriver == NULL)
		{
			Engine->BroadcastNetworkFailure(this, NULL, ENetworkFailure::NetDriverCreateFailure);
			return false;
		}
	}

	FString Error;
	if (!NetDriver->InitListen(this, InURL, false, Error)) {
		Engine->BroadcastNetworkFailure(this, NetDriver, ENetworkFailure::NetDriverListenFailure, Error);
		Log("Failed to listen: " + Error.ToString());
		if (Version::Engine_Version == 4.16) {
			NetDriver->SetWorld(NULL);
		}
		else {
			Engine->DestroyNamedNetDriver(this, NetDriver->NetDriverName);
		}
		NetDriver = nullptr;
		FLevelCollection* SourceCollection = FindCollectionByType(ELevelCollectionType::DynamicSourceLevels);
		if (SourceCollection)
		{
			SourceCollection->SetNetDriver(nullptr);
		}
		FLevelCollection* StaticCollection = FindCollectionByType(ELevelCollectionType::StaticLevels);
		if (StaticCollection)
		{
			StaticCollection->SetNetDriver(nullptr);
		}
		return false;
	}
	NetDriver->SetWorld(this); // Fixes a crash after listening, not sure why because in unreal it doesent get called again
	if (Version::Engine_Version == 4.16) {
		static bool LanPlay = FParse::Param(GetCommandLineW(), TEXT("lanplay"));
		if (!LanPlay && (NetDriver->MaxInternetClientRate < NetDriver->MaxClientRate) && (NetDriver->MaxInternetClientRate > 2500))
		{
			NetDriver->MaxClientRate = NetDriver->MaxInternetClientRate;
		}
	}
	else {
		static const bool bLanPlay = FParse::Param(GetCommandLineW(), TEXT("lanplay"));
		const bool bLanSpeed = bLanPlay || InURL.HasOption(TEXT("LAN"));
		if (!bLanSpeed && (NetDriver->MaxInternetClientRate < NetDriver->MaxClientRate) && (NetDriver->MaxInternetClientRate > 2500))
		{
			NetDriver->MaxClientRate = NetDriver->MaxInternetClientRate;
		}
	}

	NextSwitchCountdown = NetDriver->ServerTravelPause;

	SetConsoleTitleA((std::format("Core ({:.2f}) | Listening: ", Version::Fortnite_Version).c_str() + std::to_string(InURL.Port)).c_str());
	return true;
}

FLevelCollection* UWorld::FindCollectionByType(const ELevelCollectionType InType)
{
	FLevelCollection* (*FindCollectionByTypeInternal)(UWorld*, ELevelCollectionType) = decltype(FindCollectionByTypeInternal)(ImageBase + Finder::FindUWorld_FindCollectionByType());
	return FindCollectionByTypeInternal(this, InType);
}

AWorldSettings* UWorld::K2_GetWorldSettings()
{
	if (Finder::FindUWorld_K2_GetWorldSettings() == 0) {
		return GetWorldSettings(false, false);
	}
	else {
		AWorldSettings* (*K2_GetWorldSettingsInternal)(UWorld*) = decltype(K2_GetWorldSettingsInternal)(ImageBase + Finder::FindUWorld_K2_GetWorldSettings());
		return K2_GetWorldSettingsInternal(this);
	}
}

AWorldSettings* UWorld::GetWorldSettings(const bool bCheckStreamingPersistent, const bool bChecked) const
{
	AWorldSettings* (*GetWorldSettingsInternal)(const UWorld*, bool, bool) = decltype(GetWorldSettingsInternal)(ImageBase + Finder::FindUWorld_GetWorldSettings());
	return GetWorldSettingsInternal(this, bCheckStreamingPersistent, bChecked);
}

ULevel* UWorld::GetCurrentLevel() const
{
	return PersistentLevel;
}