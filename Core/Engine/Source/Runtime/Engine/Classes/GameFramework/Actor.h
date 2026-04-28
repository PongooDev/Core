#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Level.h"

class AActor;
class AController;
class AMatineeActor;
class APawn;
class APlayerController;
class UActorChannel;
class UChildActorComponent;
class UNetDriver;
class UPrimitiveComponent;
struct FAttachedActorInfo;
struct FNetViewer;
struct FNetworkObjectInfo;
struct FComponentInstanceDataCache;
class ULevel;

class AActor : public UObject {
public:
	DefineUnrealClass(AActor);

	/** Set when actor is about to be deleted. */
	DefineBitfieldUProperty(bActorIsBeingDestroyed);

	DefineBitfieldUProperty(bTearOff);

	DefineUProperty(AActor*, Owner);

	DefineUProperty(FName, NetDriverName);

	DefineUProperty(ENetDormancy, NetDormancy);

	DefineBitfieldUProperty(bNetStartup);

	DefineUProperty(float, NetUpdateFrequency);

	DefineUProperty(float, MinNetUpdateFrequency);

	DefineUProperty(int32, NetTag);

	DefineBitfieldUProperty(bOnlyRelevantToOwner);

	DefineBitfieldUProperty(bAlwaysRelevant);

	DefineBitfieldUProperty(bAllowReceiveTickEventOnDedicatedServer);

	DefineCustomProperty(float, CreationTime, ServerOffsets::AActor__CreationTime);
public:
	static inline ENetMode(*InternalGetNetModeOG)(AActor* This);
	static ENetMode InternalGetNetMode(AActor* This);

	void FinishSpawning(const FTransform& Transform, bool bIsDefaultTransform = false, const FComponentInstanceDataCache* InstanceDataCache = nullptr);

	bool IsNetStartupActor() const;

	float GetNetPriority(const FVector& ViewPos, const FVector& ViewDir, class AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth) {
		float (*&GetNetPriorityInternal)(AActor*, const FVector&, const FVector&, AActor*, AActor*, UActorChannel*, float, bool) = decltype(GetNetPriorityInternal)(VTable[Finder::FindAActor_GetNetPriorityVFT()]);
		return GetNetPriorityInternal(this, ViewPos, ViewDir, Viewer, ViewTarget, InChannel, Time, bLowBandwidth);
	}

	bool IsRelevancyOwnerFor(const AActor* ReplicatedActor, const AActor* ActorOwner, const AActor* ConnectionActor) const;

	bool GetNetDormancy(const FVector& ViewPos, const FVector& ViewDir, class AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth);

	bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const;

	inline bool IsPendingKillPending() const
	{
		bool (*IsPendingKillPendingInternal)(const AActor*) = decltype(IsPendingKillPendingInternal)(ImageBase + Finder::FindAActor_IsPendingKillPending());
		return IsPendingKillPendingInternal(this);
	}

	FVector GetActorForwardVector() const;

	FVector GetActorRightVector() const;

	FVector GetActorUpVector() const;

	ENetRole GetRemoteRole() const;

	ULevel* GetLevel() const { return GetOuter()->Cast<ULevel>(); }

	FName GetNetDriverName() const { return NetDriverName; }

	bool IsActorInitialized() const {
		if (Version::Engine_Version == 4.16) {
			static UBoolProperty* Prop = (UBoolProperty*)FindPropertyByName("bAllowReceiveTickEventOnDedicatedServer"); \
			if (Prop) {
				int32 Offset = Prop->Offset_Internal;
				uint8 BitMask = Prop->FieldMask;

				uint8 ActorInitializedBit = BitMask << 1;

				return (*((uint8*)this + Offset) & ActorInitializedBit) != 0;
			}
		}

		return true;
	}

	void CallPreReplication(UNetDriver* NetDriver);

	void ForceNetUpdate();

	const AActor* GetNetOwner() const;
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAActor_InternalGetNetMode()), InternalGetNetMode, (LPVOID*)&InternalGetNetModeOG);

		Log("Hooked AActor");
	}
};

FORCEINLINE const AActor* AActor::GetNetOwner() const
{
	AActor* (*&GetNetOwner)(const AActor*) = decltype(GetNetOwner)(VTable[Finder::FindAActor_GetNetOwnerVFT()]);
	return GetNetOwner(this);
}