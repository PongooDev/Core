#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "GameViewportClient.h"
#include "Engine/Source/Runtime/Net/Core/Public/Net/Core/Connection/NetEnums.h"
#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"
#include "EngineBaseTypes.h"
#include "EngineTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class AMatineeActor;
class APlayerController;
class Error;
class FCanvas;
class FCommonViewportClient;
class FFineGrainedPerformanceTracker;
class FPerformanceTrackingChart;
class FScreenSaverInhibitor;
class FTypeContainer;
class FViewport;
class IEngineLoop;
class IHeadMountedDisplay;
class IMessageRpcClient;
class IPerformanceDataConsumer;
class IPortalRpcLocator;
class IPortalServiceLocator;
class FSceneViewExtensions;
class IStereoRendering;
class SViewport;
class UEditorEngine;
class UEngineCustomTimeStep;
class UGameUserSettings;
class UGameViewportClient;
class ULocalPlayer;
class UNetDriver;
class UTimecodeProvider;
class UConsole;
class UWorld;

struct FNetDriverDefinition
{
	/** Unique name of this net driver definition */
	FName DefName;

	/** Class name of primary net driver */
	FName DriverClassName;

	/** Class name of the fallback net driver if the main net driver class fails to initialize */
	FName DriverClassNameFallback;
};

struct FNamedNetDriver
{
	/** Instantiation of named net driver */
	UNetDriver* NetDriver;

	/** Definition associated with this net driver */
	FNetDriverDefinition* NetDriverDef;

	FNamedNetDriver() :
		NetDriver(nullptr),
		NetDriverDef(nullptr)
	{
	}

	FNamedNetDriver(class UNetDriver* InNetDriver, FNetDriverDefinition* InNetDriverDef) :
		NetDriver(InNetDriver),
		NetDriverDef(InNetDriverDef)
	{
	}

	~FNamedNetDriver() {}
};

struct FWorldContext {
public:
	DefineUnrealStruct(FWorldContext);

	/** URL the last time we traveled */
	DefineStructProperty(FURL, LastURL);

	DefineStructProperty(TArray<FNamedNetDriver>, ActiveNetDrivers);
public:
	/** Set CurrentWorld and update external reference pointers to reflect this*/
	void SetCurrentWorld(UWorld* World);

	FORCEINLINE UWorld* World() const
	{
		return ThisCurrentWorld;
	}
public:
	DefineCustomProperty(UWorld*, ThisCurrentWorld, ServerOffsets::FWorldContext__ThisCurrentWorld);
};

class UEngine : public UObject {
public:
	DefineUnrealClass(UEngine);
public:
	DefineUProperty(UGameViewportClient*, GameViewport);

	DefineUProperty(float, NetClientTicksPerSecond);

	DefineUProperty(TSubclassOf<UConsole>, ConsoleClass);

public:
	static UEngine* GetEngine();

	static inline bool (*LoadMapOG)(UEngine* This, FWorldContext& WorldContext, FURL& URL, class UPendingNetGame* Pending, FString& Error);
	static bool LoadMap(UEngine* This, FWorldContext& WorldContext, FURL& URL, class UPendingNetGame* Pending, FString& Error);

	void BroadcastNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString = FString()) {
		void (*BroadcastNetworkFailureInternal)(UEngine*, UWorld*, UNetDriver*, ENetworkFailure::Type, const FString&) = decltype(BroadcastNetworkFailureInternal)(ImageBase + Finder::FindUEngine_BroadcastNetworkFailure());
		BroadcastNetworkFailureInternal(this, World, NetDriver, FailureType, ErrorString);
	}

	UNetDriver* CreateNetDriver(UWorld* InWorld, FName NetDriverDefinition);

	bool CreateNamedNetDriver(UWorld* InWorld, FName NetDriverName, FName NetDriverDefinition);

	UNetDriver* FindNamedNetDriver(UWorld* InWorld, FName NetDriverName);

	void DestroyNamedNetDriver(UWorld* InWorld, FName NetDriverName);

	FWorldContext* GetWorldContextFromWorld(UWorld* InWorld);

	FWorldContext& GetWorldContextFromWorldChecked(UWorld* InWorld);

	FWorldContext& CreateNewWorldContext(EWorldType::Type WorldType);

	float GetMaxTickRate(float DeltaTime, bool bAllowFrameRateSmoothing = true) const;
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUEngine_LoadMap()), LoadMap, (LPVOID*)&LoadMapOG);

		Log("Hooked UEngine");
	}
};

inline UEngine** GEngine = (UEngine**)(ImageBase + ServerOffsets::GEngine);