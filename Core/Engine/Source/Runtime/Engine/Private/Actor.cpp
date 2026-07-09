#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Public/OnlineBeaconClient.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"

ENetMode AActor::InternalGetNetMode(AActor* This)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bListenServer) {
		return NM_ListenServer;
	}

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
	bool (*&IsRelevancyOwnerForInternal)(const AActor*, const AActor*, const AActor*, const AActor*) = decltype(IsRelevancyOwnerForInternal)(VTable[Finder::FindAActor_IsRelevancyOwnerForVFT()]);
	return IsRelevancyOwnerForInternal(this, ReplicatedActor, ActorOwner, ConnectionActor);
}

bool AActor::GetNetDormancy(const FVector& ViewPos, const FVector& ViewDir, AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth)
{
	// For now, per peer dormancy is not supported
	return false;
}

bool AActor::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool (*&IsNetRelevantForInternal)(const AActor*, const AActor*, const AActor*, const FVector&) = decltype(IsNetRelevantForInternal)(VTable[Finder::FindAActor_IsNetRelevantForVFT()]);
	return IsNetRelevantForInternal(this, RealViewer, ViewTarget, SrcLocation);
}

FVector AActor::GetActorForwardVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorForwardVector"));

	return const_cast<AActor*>(this)->Call<FVector>(Func);
}

FVector AActor::GetActorRightVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorRightVector"));

	return const_cast<AActor*>(this)->Call<FVector>(Func);
}

FVector AActor::GetActorUpVector() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetActorUpVector"));

	return const_cast<AActor*>(this)->Call<FVector>(Func);
}

FVector AActor::K2_GetActorLocation() const
{
	if (!this) {
		return *FVector::Allocate();
	}

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_GetActorLocation"));

	return const_cast<AActor*>(this)->Call<FVector>(Func);
}

FRotator AActor::K2_GetActorRotation() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("K2_GetActorRotation");

	return const_cast<AActor*>(this)->Call<FRotator>(Func);
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

	return const_cast<AActor*>(this)->Call<ENetRole>(Func);
}

void AActor::CallPreReplication(UNetDriver* NetDriver)
{
	void (*CallPreReplicationInternal)(AActor*, UNetDriver*) = decltype(CallPreReplicationInternal)(ImageBase + Finder::FindAActor_CallPreReplication());
	CallPreReplicationInternal(this, NetDriver);
}

void AActor::ForceNetUpdate()
{
	if (!this) {
		return;
	}
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ForceNetUpdate");

	if (!Func) {
		return;
	}

	Call(Func);
}

const AActor* AActor::GetNetOwner() const
{
	if (IsA(APawn::StaticClass())) {
		return this;
	}
	if (IsA(APlayerController::StaticClass())) {
		return this;
	}
	if (IsA(AOnlineBeaconClient::StaticClass())) {
		AOnlineBeaconClient* BeaconClient = (AOnlineBeaconClient*)this;
		return (AActor*)BeaconClient->BeaconOwner;
	}

	return Owner;
}

void AActor::K2_DestroyActor()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_DestroyActor"));

	Call(Func);
}

FTransform AActor::GetTransform() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetTransform");

	return const_cast<AActor*>(this)->Call<FTransform>(Func);
}

void AActor::SetNetDormancy(ENetDormancy NewDormancy)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetNetDormancy");

	Call(Func, NewDormancy);
}

void AActor::GetActorEyesViewPoint(FVector* OutLocation, FRotator* OutRotation) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetActorEyesViewPoint");

	return const_cast<AActor*>(this)->Call(Func, OutLocation, OutRotation);
}

void AActor::Reset()
{
	void (*&ResetInternal)(AActor*) = decltype(ResetInternal)(VTable[Finder::FindAActor_ResetVFT()]);
	ResetInternal(this);
}

float AActor::GetDistanceTo(const AActor* OtherActor) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetDistanceTo");

	return const_cast<AActor*>(this)->Call<float>(Func, OtherActor);
}

bool AActor::K2_SetActorLocation(FVector& NewLocation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("K2_SetActorLocation");

	return Call<bool>(Func, NewLocation, bSweep, SweepHitResult, bTeleport);
}

void AActor::FlushNetDormancy()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("FlushNetDormancy");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AActor::SetOwner(class AActor* NewOwner)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetOwner");

	if (!Func) {
		return;
	}

	Call(Func, NewOwner);
}

void AActor::SetReplicateMovement(bool bInReplicateMovement)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetReplicateMovement");

	if (!Func) {
		return;
	}

	Call(Func, bInReplicateMovement);
}