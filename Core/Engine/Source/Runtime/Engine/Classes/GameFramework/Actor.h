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
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

class AActor;
class AController;
class AMatineeActor;
class APawn;
class APlayerController;
class UActorChannel;
class UChildActorComponent;
class UNetDriver;
class UPrimitiveComponent;
class USceneComponent;
class UActorComponent;
struct FAttachedActorInfo;
struct FNetViewer;
struct FNetworkObjectInfo;
struct FComponentInstanceDataCache;
class ULevel;
class AOnlineBeaconClient;
struct FHitResult;

class AActor : public UObject {
public:
	DefineUnrealClass(AActor);

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
	DefineBitfieldUProperty(bReplicates);
	DefineUProperty(TArray<FName>, Tags);
	DefineUProperty(FGameplayTagContainer, StaticGameplayTags);
	DefineUProperty(uint8, Role);
	DefineUProperty(USceneComponent*, RootComponent);
	DefineUProperty(APawn*, Instigator);
	DefineBitfieldUProperty(bHidden);
	DefineUProperty(TSet<UActorComponent*>, OwnedComponents);
public:
	FORCEINLINE bool HasAuthority() const { return Role == ROLE_Authority; }
	FORCEINLINE ENetRole GetLocalRole() const { return (ENetRole)Role; }

	FORCEINLINE USceneComponent* GetRootComponent() const { return RootComponent; }
	FORCEINLINE APawn* GetInstigator() const { return Instigator; }
	FORCEINLINE bool IsHidden() const { return bHidden; }

	FORCEINLINE bool ActorHasTag(FName Tag) const { return Tag != FName() && const_cast<AActor*>(this)->Tags.Contains(Tag); }

	UActorComponent* FindComponentByClass(UClass* ComponentClass) const;

	template<class T>
	FORCEINLINE T* FindComponentByClass() const { return (T*)FindComponentByClass(T::StaticClass()); }

	FORCEINLINE UActorComponent* GetComponentByClass(UClass* ComponentClass) const { return FindComponentByClass(ComponentClass); }

	void GetComponents(TArray<UActorComponent*>& OutComponents) const;

	float GetGameTimeSinceCreation() const;

	FVector GetVelocity() const;

	FRepMovement* GetReplicatedMovement()
	{
		static int32 Offset = -2;
		if (Offset == -2)
			Offset = (int32)StaticClass()->GetPropertyOffset("ReplicatedMovement");
		if (Offset < 0)
			return nullptr;
		return (FRepMovement*)((uint8*)this + Offset);
	}
public:
	static inline ENetMode(*InternalGetNetModeOG)(AActor* This);
	static ENetMode InternalGetNetMode(AActor* This);

	void FinishSpawning(const FTransform& Transform, bool bIsDefaultTransform = false, const FComponentInstanceDataCache* InstanceDataCache = nullptr);

	bool IsNetStartupActor() const;

	float GetNetPriority(const FVector& ViewPos, const FVector& ViewDir, class AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth) {
		if (Finder::FindAActor_GetNetPriorityVFT()) {
			float (*&GetNetPriorityInternal)(AActor*, const FVector&, const FVector&, AActor*, AActor*, UActorChannel*, float, bool) = decltype(GetNetPriorityInternal)(VTable[Finder::FindAActor_GetNetPriorityVFT()]);
			return GetNetPriorityInternal(this, ViewPos, ViewDir, Viewer, ViewTarget, InChannel, Time, bLowBandwidth);
		}

		return 0.f;
	}

	bool IsRelevancyOwnerFor(const AActor* ReplicatedActor, const AActor* ActorOwner, const AActor* ConnectionActor) const;

	bool GetNetDormancy(const FVector& ViewPos, const FVector& ViewDir, class AActor* Viewer, AActor* ViewTarget, UActorChannel* InChannel, float Time, bool bLowBandwidth);

	bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const;

	inline bool IsPendingKillPending() const
	{
		if (Finder::FindAActor_IsPendingKillPending()) {
			bool (*IsPendingKillPendingInternal)(const AActor*) = decltype(IsPendingKillPendingInternal)(ImageBase + Finder::FindAActor_IsPendingKillPending());
			return IsPendingKillPendingInternal(this);
		}
		else {
			return bActorIsBeingDestroyed;
		}
	}

	FVector GetActorForwardVector() const;

	FVector GetActorRightVector() const;

	FVector GetActorUpVector() const;

	FVector K2_GetActorLocation() const;

	FRotator K2_GetActorRotation() const;

	FVector GetActorScale3D();

	void SetActorScale3D(FVector NewScale3D);

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

	void K2_DestroyActor();

	FTransform GetTransform() const;

	void SetNetDormancy(ENetDormancy NewDormancy);

	void GetActorEyesViewPoint(FVector* OutLocation, FRotator* OutRotation) const;

	void Reset();

	float GetDistanceTo(const AActor* OtherActor) const;

	bool K2_SetActorLocation(FVector& NewLocation, bool bSweep, FHitResult* SweepHitResult, bool bTeleport);

	void FlushNetDormancy();

	void SetOwner(AActor* NewOwner);

	void SetReplicateMovement(bool bInReplicateMovement);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindAActor_InternalGetNetMode()), InternalGetNetMode, (LPVOID*)&InternalGetNetModeOG);

		Log("Hooked AActor");
	}
};