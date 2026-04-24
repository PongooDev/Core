#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

ENetMode AActor::InternalGetNetMode(AActor* This)
{
	return InternalGetNetModeOG(This);
	return NM_DedicatedServer;
}

void AActor::FinishSpawning(const FTransform& UserTransform, bool bIsDefaultTransform, const FComponentInstanceDataCache* InstanceDataCache)
{
	void (*FinishSpawningInternal)(AActor*, const FTransform&, bool, const FComponentInstanceDataCache*) = decltype(FinishSpawningInternal)(ImageBase + Finder::FindAActor_FinishSpawning());
	FinishSpawningInternal(this, UserTransform, bIsDefaultTransform, InstanceDataCache);
}

bool AActor::IsNetStartupActor() const
{
	return bNetStartup;
}

bool AActor::IsRelevancyOwnerFor(const AActor* ReplicatedActor, const AActor* ActorOwner, const AActor* ConnectionActor) const
{
	bool (*IsRelevancyOwnerForInternal)(const AActor*, const AActor*, const AActor*, const AActor*) = decltype(IsRelevancyOwnerForInternal)(ImageBase + Finder::FindAActor_IsRelevancyOwnerFor());
	return IsRelevancyOwnerForInternal(this, ReplicatedActor, ActorOwner, ConnectionActor);
}

bool AActor::GetNetDormancy(const FVector& ViewPos, const FVector& ViewDir, AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth)
{
	// For now, per peer dormancy is not supported
	return false;
}

bool AActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool (*IsNetRelevantForInternal)(const AActor*, const AActor*, const AActor*, const FVector&) = decltype(IsNetRelevantForInternal)(ImageBase + Finder::FindAActor_IsNetRelevantFor());
	return IsNetRelevantForInternal(this, RealViewer, ViewTarget, SrcLocation);
}

UWorld* AActor::GetWorld() const
{
	UWorld* (*GetWorldInternal)(const AActor*) = decltype(GetWorldInternal)(ImageBase + Finder::FindAActor_GetWorld());
	return GetWorldInternal(this);
}

FVector AActor::GetActorForwardVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorForwardVector"));

	struct Actor_GetActorForwardVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorForwardVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector AActor::GetActorRightVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorRightVector"));

	struct Actor_GetActorRightVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorRightVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

FVector AActor::GetActorUpVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorUpVector"));

	struct Actor_GetActorUpVector final
	{
	public:
		FVector ReturnValue;
	};

	Actor_GetActorUpVector Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

ENetRole AActor::GetRemoteRole() const
{
	if (!this) {
		return ROLE_None;
	}

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetRemoteRole"));

	if (!Func) {
		Log("AActor::GetRemoteRole: Failed to find function!");
		return ROLE_None;
	}

	struct Actor_GetRemoteRole final
	{
	public:
		ENetRole ReturnValue;
	};

	Actor_GetRemoteRole Parms{};

	const_cast<AActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void AActor::CallPreReplication(UNetDriver* NetDriver)
{
	void (*CallPreReplicationInternal)(AActor*, UNetDriver*) = decltype(CallPreReplicationInternal)(ImageBase + Finder::FindAActor_CallPreReplication());
	CallPreReplicationInternal(this, NetDriver);
}

void AActor::ForceNetUpdate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ForceNetUpdate"));

	ProcessEvent(Func, nullptr);
}