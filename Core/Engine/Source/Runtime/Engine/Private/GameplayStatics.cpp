#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Materials/Material.h"
#include "Engine/Source/Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/DamageType.h"
#include "Engine/Source/Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Engine/Source/Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Level.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

static UWorld* GetWorldFromContext(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UObject* ContextObject = const_cast<UObject*>(WorldContextObject);

		if (UWorld* World = Cast<UWorld>(ContextObject))
			return World;

		if (const AActor* Actor = Cast<AActor>(ContextObject))
		{
			if (ULevel* Level = Actor->GetLevel())
			{
				if (UWorld* World = Level->OwningWorld)
					return World;
			}
		}

		if (UWorld* World = ContextObject->GetTypedOuter<UWorld>())
			return World;
	}

	return UWorld::GetWorld();
}

static bool IsActorInWorld(const AActor* Actor, const UWorld* World)
{
	ULevel* Level = Actor->GetLevel();
	return Level && Level->OwningWorld == World;
}

AActor* UGameplayStatics::BeginDeferredActorSpawnFromClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform, ESpawnActorCollisionHandlingMethod CollisionHandlingOverride, AActor* Owner)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.BeginDeferredActorSpawnFromClass");

	if (!Func)
		return nullptr;

	return GetDefaultObj()->Call<AActor*>(Func, WorldContextObject, ActorClass, SpawnTransform, CollisionHandlingOverride, Owner);
}

AActor* UGameplayStatics::FinishSpawningActor(AActor* Actor, const FTransform& SpawnTransform)
{
	if (!Actor)
		return nullptr;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.FinishSpawningActor");

	if (!Func)
		return nullptr;

	return GetDefaultObj()->Call<AActor*>(Func, Actor, SpawnTransform);
}

UObject* UGameplayStatics::SpawnObject(TSubclassOf<UObject> ObjectClass, UObject* Outer)
{
	UClass* Class = ObjectClass.Get();

	if (!Class || !Outer)
		return nullptr;

	if (Class->IsChildOf(AActor::StaticClass()))
	{
		UE_LOG(LogEngine, Warning, TEXT("UGameplayStatics::SpawnObject: cannot spawn an actor class, use SpawnActor instead: %s"), *Class->GetName());
		return nullptr;
	}

	if (Class->IsChildOf(UActorComponent::StaticClass()))
	{
		UE_LOG(LogEngine, Warning, TEXT("UGameplayStatics::SpawnObject: cannot spawn a component class: %s"), *Class->GetName());
		return nullptr;
	}

	return NewObject<UObject>(Outer, Class);
}

void UGameplayStatics::FlushLevelStreaming(const UObject* WorldContextObject)
{
	static UFunction* Func = nullptr;
	static bool bSearched = false;

	if (!bSearched) {
		bSearched = true;
		Func = GetDefaultObj()->FindFunction("FlushLevelStreaming");

		if (!Func) {
			UE_LOG(LogEngine, Error, TEXT("UGameplayStatics::FlushLevelStreaming: function not found"));
		}
	}

	if (!Func) {
		return;
	}

	GetDefaultObj()->Call(Func, WorldContextObject);
}

float UGameplayStatics::GetTimeSeconds(const UObject* WorldContextObject)
{
	UWorld* World = GetWorldFromContext(WorldContextObject);
	return World ? World->TimeSeconds : 0.f;
}

void UGameplayStatics::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>* OutActors)
{
	if (!OutActors)
		return;

	OutActors->Reset();

	UClass* Class = ActorClass.Get();
	if (!Class)
		return;

	UWorld* World = GetWorldFromContext(WorldContextObject);
	if (!World)
		return;

	for (AActor* Actor : TObjectRange<AActor>())
	{
		if (!Actor->IsA(Class))
			continue;

		if (Actor->IsPendingKillPending())
			continue;

		if (!IsActorInWorld(Actor, World))
			continue;

		OutActors->Add(Actor);
	}
}

void UGameplayStatics::GetAllActorsWithTag(const UObject* WorldContextObject, FName Tag, TArray<AActor*>* OutActors)
{
	if (!OutActors)
		return;

	OutActors->Reset();

	if (Tag == FName())
		return;

	UWorld* World = GetWorldFromContext(WorldContextObject);
	if (!World)
		return;

	for (AActor* Actor : TObjectRange<AActor>())
	{
		if (Actor->IsPendingKillPending())
			continue;

		if (!Actor->ActorHasTag(Tag))
			continue;

		if (!IsActorInWorld(Actor, World))
			continue;

		OutActors->Add(Actor);
	}
}

APlayerController* UGameplayStatics::GetPlayerController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	UWorld* World = GetWorldFromContext(WorldContextObject);
	if (!World)
		return nullptr;

	int32 Index = 0;
	for (APlayerController* PlayerController : TObjectRange<APlayerController>())
	{
		if (PlayerController->IsPendingKillPending())
			continue;

		if (!IsActorInWorld(PlayerController, World))
			continue;

		if (Index == PlayerIndex)
			return PlayerController;

		Index++;
	}

	return nullptr;
}

APawn* UGameplayStatics::GetPlayerPawn(const UObject* WorldContextObject, int32 PlayerIndex)
{
	APlayerController* PlayerController = GetPlayerController(WorldContextObject, PlayerIndex);
	return PlayerController ? PlayerController->GetPawnOrSpectator() : nullptr;
}

AGameModeBase* UGameplayStatics::GetGameMode(const UObject* WorldContextObject)
{
	UWorld* World = GetWorldFromContext(WorldContextObject);
	return World ? World->AuthorityGameMode : nullptr;
}

AGameStateBase* UGameplayStatics::GetGameState(const UObject* WorldContextObject)
{
	UWorld* World = GetWorldFromContext(WorldContextObject);
	return World ? World->GameState : nullptr;
}
