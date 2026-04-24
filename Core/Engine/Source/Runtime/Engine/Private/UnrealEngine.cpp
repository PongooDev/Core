#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"

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
		Log("UEngine::GetEngine failed to find GEngine!");
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
			Log("LoadMap: failed to Listen(" + URL.ToString().ToString() + ")");
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
		Log("CreateNamedNetDriver " + NetDriverName.ToString().ToString() + " already exists as " + NetDriver->GetName().ToString());
	}
	else
	{
		Log("CreateNamedNetDriver failed to create driver " + NetDriverName.ToString().ToString() + " from definition " + NetDriverDefinition.ToString().ToString());
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
	float (*GetMaxTickRateInternal)(const UEngine*, float, bool) = decltype(GetMaxTickRateInternal)(ImageBase + Finder::FindUEngine_GetMaxTickRate());
	return GetMaxTickRateInternal(this, DeltaTime, bAllowFrameRateSmoothing);
}