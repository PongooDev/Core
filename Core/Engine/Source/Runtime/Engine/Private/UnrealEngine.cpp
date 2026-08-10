#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

void FWorldContext::SetCurrentWorld(UWorld* World)
{
	void (*SetCurrentWorldInternal)(FWorldContext*, UWorld*) = decltype(SetCurrentWorldInternal)(ImageBase + Finder::FindFWorldContext_SetCurrentWorld());
	SetCurrentWorldInternal(this, World);
}

UEngine* UEngine::GetEngine() {
	if (ServerOffsets::GEngine != 0)
		return *reinterpret_cast<UEngine**>(ImageBase + ServerOffsets::GEngine);

	static UEngine* GEngine = nullptr;

	if (GEngine)
		return GEngine;

	for (int i = 0; i < FUObjectArray::Num(); i++)
	{
		FUObjectItem* Item = FUObjectArray::IndexToObject(i);
		if (!Item)
			continue;

		UObjectBase* Object = Item->Object;

		if (!Object)
			continue;

		if (!Object->IsA(UEngine::StaticClass())) {
			continue;
		}

		if (Object->IsDefaultObject()) {
			continue;
		}

		GEngine = (UEngine*)Object;
		break;
	}

	if (!GEngine || !GEngine->IsA(UEngine::StaticClass())) {
		UE_LOG(LogEngine, Error, TEXT("UEngine::GetEngine failed to find GEngine!"));
		return GEngine = nullptr;
	}

	return GEngine;
}

bool UEngine::LoadMap(UEngine* This, FWorldContext& WorldContext, FURL& URL, class UPendingNetGame* Pending, FString& Error)
{
	bool result = LoadMapOG(This, WorldContext, URL, Pending, Error);

	// Get last URL so we can still use the URL
	URL = WorldContext.LastURL;

	// Listen for clients.
	if (Pending == NULL && (!*GIsClient || URL.HasOption(TEXT("Listen"))))
	{
		if (!WorldContext.World()->Listen(URL))
		{
			//UE_LOG(LogNet, Error, TEXT("LoadMap: failed to Listen(%s)"), *URL.ToString());
		}
	}

	return result;
}

UNetDriver* CreateNetDriver_Local(UEngine* Engine, FWorldContext& Context, FName NetDriverDefinition)
{
	UNetDriver* (*CreateNetDriverInternal)(UEngine*, FWorldContext&, FName) = decltype(CreateNetDriverInternal)(ImageBase + Finder::FindUEngine_CreateNetDriver_Local());
	return CreateNetDriverInternal(Engine, Context, NetDriverDefinition);
}

UNetDriver* UEngine::CreateNetDriver(UWorld* InWorld, FName NetDriverDefinition)
{
	return CreateNetDriver_Local(this, GetWorldContextFromWorldChecked(InWorld), NetDriverDefinition);
}

UNetDriver* FindNamedNetDriver_Local(const TArray<FNamedNetDriver>& ActiveNetDrivers, FName NetDriverName)
{
	for (int32 Index = 0; Index < ActiveNetDrivers.Num(); Index++)
	{
		const FNamedNetDriver& NamedNetDriver = ActiveNetDrivers[Index];
		UNetDriver* NetDriver = NamedNetDriver.NetDriver;
		if (NetDriver && NetDriver->NetDriverName == NetDriverName)
		{
			return NetDriver;
		}
	}
	return NULL;
}

bool CreateNamedNetDriver_Local(UEngine* Engine, FWorldContext& Context, FName NetDriverName, FName NetDriverDefinition)
{
	// 4.16 implementation
	UNetDriver* NetDriver = FindNamedNetDriver_Local(Context.ActiveNetDrivers, NetDriverName);
	if (NetDriver == nullptr)
	{
		NetDriver = CreateNetDriver_Local(Engine, Context, NetDriverDefinition);
		if (NetDriver)
		{
			NetDriver->SetNetDriverName(NetDriverName);
			return true;
		}
	}

	if (NetDriver)
	{
		UE_LOG(LogNet, Log, TEXT("CreateNamedNetDriver %s already exists as %s"), *NetDriverName.ToString(), *NetDriver->GetName());
	}
	else
	{
		UE_LOG(LogNet, Log, TEXT("CreateNamedNetDriver failed to create driver %s from definition %s"), *NetDriverName.ToString(), *NetDriverDefinition.ToString());
	}

	return false;
}

bool UEngine::CreateNamedNetDriver(UWorld* InWorld, FName NetDriverName, FName NetDriverDefinition)
{
	return CreateNamedNetDriver_Local(this, GetWorldContextFromWorldChecked(InWorld), NetDriverName, NetDriverDefinition);
}

UNetDriver* UEngine::FindNamedNetDriver(UWorld* InWorld, FName NetDriverName)
{
	return FindNamedNetDriver_Local(GetWorldContextFromWorldChecked(InWorld).ActiveNetDrivers, NetDriverName);
}

void DestroyNamedNetDriver_Local(FWorldContext& Context, FName NetDriverName) {
	void (*DestroyNamedNetDriverInternal)(FWorldContext&, FName) = decltype(DestroyNamedNetDriverInternal)(ImageBase + Finder::FindUEngine_DestroyNamedNetDriver());
	DestroyNamedNetDriverInternal(Context, NetDriverName);
}

void UEngine::DestroyNamedNetDriver(UWorld* InWorld, FName NetDriverName)
{
	DestroyNamedNetDriver_Local(GetWorldContextFromWorldChecked(InWorld), NetDriverName);
}

FWorldContext* UEngine::GetWorldContextFromWorld(UWorld* InWorld)
{
	FWorldContext* (*GetWorldContextFromWorldInternal)(UEngine*, UWorld*) = decltype(GetWorldContextFromWorldInternal)(ImageBase + Finder::FindUEngine_GetWorldContextFromWorld());
	return GetWorldContextFromWorldInternal(this, InWorld);
}

FWorldContext& UEngine::CreateNewWorldContext(EWorldType::Type WorldType)
{
	FWorldContext& (*CreateNewWorldContextInternal)(UEngine*, EWorldType::Type) = decltype(CreateNewWorldContextInternal)(ImageBase + Finder::FindUEngine_CreateNewWorldContext());
	return CreateNewWorldContextInternal(this, WorldType);
}

FWorldContext& HandleInvalidWorldContext()
{
	UEngine* GEngine = UEngine::GetEngine();
	return GEngine->CreateNewWorldContext(EWorldType::None);
}

FWorldContext& UEngine::GetWorldContextFromWorldChecked(UWorld* InWorld)
{
	FWorldContext& (*GetWorldContextFromWorldCheckedInternal)(UEngine*, UWorld*) = decltype(GetWorldContextFromWorldCheckedInternal)(ImageBase + Finder::FindUEngine_GetWorldContextFromWorldChecked());
	return GetWorldContextFromWorldCheckedInternal(this, InWorld);
}

float UEngine::GetMaxTickRate(float DeltaTime, bool bAllowFrameRateSmoothing) const
{
	float (*&GetMaxTickRateInternal)(const UEngine*, float, bool) = decltype(GetMaxTickRateInternal)(VTable[Finder::FindUEngine_GetMaxTickRateVFT()]);
	return GetMaxTickRateInternal(this, DeltaTime, bAllowFrameRateSmoothing);
}