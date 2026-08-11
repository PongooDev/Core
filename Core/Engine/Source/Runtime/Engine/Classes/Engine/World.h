#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Parse.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/IntVector.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

class ULevel;
class AController;
class AActor;
class ULevelStreaming;
struct FTransform;
class UAISystemBase;
class FLevelCollection;
class ULocalPlayer;
class APlayerController;
class UGameViewportClient;
struct FTimerManager;
class AWorldSettings;
class AGameStateBase;
class UNavigationSystemBase;
class UNetConnection;
class UGameInstance;
class UNetDriver;
class AGameNetworkManager;
class AGameModeBase;
class UDemoNetDriver;
class UNavigationSystem;

struct FStreamingLevelsToConsider
{
	DefineUnrealStruct(FStreamingLevelsToConsider);

private:

	/** Priority sorted array of streaming levels actively being considered. */
	DefineStructProperty(TArray<ULevelStreaming*>, StreamingLevels);
};

struct FLevelCollection
{
public:
	DefineUnrealStruct(FLevelCollection);
public:
	/** The type of this collection. */
	ELevelCollectionType CollectionType;

	/**
	* Whether or not this collection is currently visible. While invisible, actors in this collection's
	* levels will not be rendered and sounds originating from levels in this collection will not be played.
	*/
	bool bIsVisible;

	/**
	 * The GameState associated with this collection. This may be different than the UWorld's GameState
	 * since the source collection and the duplicated collection will have their own instances.
	 */
	AGameStateBase* GameState;

	/**
	 * The network driver associated with this collection.
	 * The source collection and the duplicated collection will have their own instances.
	 */
	UNetDriver* NetDriver;

	/**
	 * The demo network driver associated with this collection.
	 * The source collection and the duplicated collection will have their own instances.
	 */
	UDemoNetDriver* DemoNetDriver;

	/**
	 * The persistent level associated with this collection.
	 * The source collection and the duplicated collection will have their own instances.
	 */
	ULevel* PersistentLevel;

	/** All the levels in this collection. */
	TSet<ULevel*> Levels;

public:
	/** Gets the net driver for this collection. */
	UNetDriver* GetNetDriver() const { return NetDriver; }

	/** Sets the net driver for this collection. */
	void SetNetDriver(UNetDriver* const InNetDriver) { NetDriver = InNetDriver; }
};

class UWorld : public UObject {
public:
	DefineUnrealClass(UWorld);
public:
	DefineUProperty(ULevel*, PersistentLevel);

	DefineBitfieldUProperty(bAreConstraintsDirty);

	DefineUProperty(UGameInstance*, OwningGameInstance);

	/** Pointer to the current level in the queue to be made visible, NULL if none are pending. */
	DefineUProperty(ULevel*, CurrentLevelPendingVisibility);

	/** Pointer to the current level in the queue to be made invisible, NULL if none are pending. */
	DefineUProperty(ULevel*, CurrentLevelPendingInvisibility);

	/** Level collection. ULevels are referenced by FName (Package name) to avoid serialized references. Also contains offsets in world units */
	DefineUProperty(TArray<ULevelStreaming*>, StreamingLevels);

	/** This is the list of streaming levels that are actively being considered for what their state should be. It will be a subset of StreamingLevels */
	DefineUProperty(FStreamingLevelsToConsider, StreamingLevelsToConsider);

	/**
	 * The network driver associated with this collection.
	 * The source collection and the duplicated collection will have their own instances.
	 */
	DefineUProperty(UNetDriver*, NetDriver);

	/** Amount of time to wait before traveling to next map, gives clients time to receive final RPCs @see ServerTravelPause */
	DefineCustomProperty(float, NextSwitchCountdown, ServerOffsets::UWorld__NextSwitchCountdown);

	/** The current GameMode, valid only on the server */
	DefineUProperty(AGameModeBase*, AuthorityGameMode);

	DefineUProperty(AGameStateBase*, GameState);

	DefineUProperty(TArray<ULevel*>, Levels);

	DefineUProperty(UDemoNetDriver*, DemoNetDriver);

	DefineUProperty(TArray<FLevelCollection>, LevelCollections);

	DefineUProperty(AGameNetworkManager*, NetworkManager);

	DefineCustomProperty(float, TimeSeconds, ServerOffsets::UWorld__TimeSeconds);

	static FORCEINLINE uintptr_t ClockOffset(int32 DeltaFromTimeSeconds)
	{
		const uintptr_t Base = ServerOffsets::UWorld__TimeSeconds;
		return Base ? Base + DeltaFromTimeSeconds : 0;
	}

	DefineCustomProperty(float, UnpausedTimeSeconds, UWorld::ClockOffset(0x04));
	DefineCustomProperty(float, RealTimeSeconds, UWorld::ClockOffset(0x08));
	DefineCustomProperty(float, AudioTimeSeconds, UWorld::ClockOffset(0x0C));
	DefineCustomProperty(float, DeltaTimeSeconds, UWorld::ClockOffset(0x10));
	DefineCustomProperty(float, PauseDelay, UWorld::ClockOffset(0x14));

	static FORCEINLINE bool OriginBlockValid()
	{
		static int32 Cached = -1;

		if (Cached < 0)
		{
			const uintptr_t Base = ServerOffsets::UWorld__TimeSeconds;
			const uintptr_t Composition = UWorld::StaticClass()->GetPropertyOffset("WorldComposition");

			Cached = (Base && Composition != (uintptr_t)-1 && Composition == Base + 0x40) ? 1 : 0;

			if (!Cached)
			{
				if (Composition != (uintptr_t)-1 && Base)
				{
					UE_LOG(LogWorld, Warning,
						TEXT("UWorld origin block check failed - WorldComposition is at 0x%X but TimeSeconds 0x%X implies 0x%X. ")
						TEXT("UnpausedTimeSeconds/RealTimeSeconds/AudioTimeSeconds/DeltaTimeSeconds/PauseDelay are suspect on this build."),
						(uint32)Composition, (uint32)Base, (uint32)(Base + 0x40));
				}
				else
				{
					UE_LOG(LogWorld, Warning, TEXT("UWorld origin block could not be verified - OriginLocation/RequestedOriginLocation/OriginOffsetThisFrame are unavailable."));
				}
			}
		}

		return Cached == 1;
	}

	static FORCEINLINE uintptr_t OriginOffset(int32 DeltaFromTimeSeconds)
	{
		return OriginBlockValid() ? ServerOffsets::UWorld__TimeSeconds + DeltaFromTimeSeconds : 0;
	}

	DefineCustomProperty(FIntVector, OriginLocation, UWorld::OriginOffset(0x18));
	DefineCustomProperty(FIntVector, RequestedOriginLocation, UWorld::OriginOffset(0x24));
	DefineCustomProperty(FVector, OriginOffsetThisFrame, UWorld::OriginOffset(0x30));

	DefineUProperty(UNavigationSystem*, NavigationSystem);
public:
	static UWorld* GetWorld();
	
	ENetMode InternalGetNetMode();
	static inline ENetMode (*InternalGetNetModeOG)(UWorld* This);
	static ENetMode InternalGetNetModeHK(UWorld* This);

	AActor* SpawnActorUnfinished(UClass* InClass, FVector Location = FVector(), FRotator Rotation = FRotator(), AActor* Owner = nullptr);
	AActor* FinishSpawnActor(AActor* Actor, FVector Location = FVector(), FRotator Rotation = FRotator());
	AActor* SpawnActor(UClass* InClass, FVector Location = FVector(), FRotator Rotation = FRotator(), AActor* Owner = nullptr);

	AActor* SpawnActor(UClass* Class, FTransform Transform, AActor* Owner = nullptr);
	AActor* SpawnActorUnfinished(UClass* Class, FTransform Transform, AActor* Owner = nullptr);
	AActor* FinishSpawnActor(AActor* Actor, FTransform Transform);

	bool ServerTravel(const FString& InURL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);

	bool Listen(FURL& InURL);
	static bool ListenHK(UWorld* World, FURL& InURL);

	FLevelCollection* FindCollectionByType(const ELevelCollectionType InType);

	class AWorldSettings* K2_GetWorldSettings();
	class AWorldSettings* GetWorldSettings(bool bCheckStreamingPersistent = false, bool bChecked = true) const;

	FORCEINLINE TArray<ULevel*> GetLevels() const { return Levels; }

	FORCEINLINE UGameInstance* GetGameInstance() const { return OwningGameInstance; }

	template<class T>
	FORCEINLINE T* GetGameState() const { return ::Cast<T>(GameState); }

	FORCEINLINE AGameStateBase* GetGameState() const { return GameState; }

	FORCEINLINE UDemoNetDriver* GetDemoNetDriver() const { return DemoNetDriver; }

	ULevel* GetCurrentLevelPendingVisibility() const { return CurrentLevelPendingVisibility; }

	ULevel* GetCurrentLevelPendingInvisibility() const { return CurrentLevelPendingInvisibility; }

	class ULevel* GetCurrentLevel() const;

	APlayerController* GetFirstPlayerController() const;

	bool IsInSeamlessTravel();

	void SetNavigationSystem(UNavigationSystem* InNavigationSystem);

	static FString RemovePIEPrefix(const FString& Source);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUWorld_InternalGetNetMode()), InternalGetNetModeHK, (LPVOID*)&InternalGetNetModeOG);

		if (Finder::FindUWorld_ListenPatch()) {
			PatchCallFar(ImageBase + Finder::FindUWorld_ListenPatch(), (LPVOID*)&ListenHK);
		}

		Log("Hooked UWorld");
	}
};

inline UWorld** GWorld = (UWorld**)(ImageBase + ServerOffsets::GWorld);