#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

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
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.FinishSpawningActor");

	if (!Func)
		return nullptr;

	return GetDefaultObj()->Call<AActor*>(Func, Actor, SpawnTransform);
}

UObject* UGameplayStatics::SpawnObject(TSubclassOf<UObject> ObjectClass, UObject* Outer) {
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.SpawnObject");

	return GetDefaultObj()->Call<UObject*>(Func, ObjectClass, Outer);
}

float UGameplayStatics::GetTimeSeconds(const class UObject* WorldContextObject)
{
	static UFunction* Func = nullptr;
	static UObject* DefaultObj = GetDefaultObj();

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetTimeSeconds");

	return DefaultObj->Call<float>(Func, WorldContextObject);
}

void UGameplayStatics::GetAllActorsOfClass(const UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, TArray<AActor*>* OutActors)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetAllActorsOfClass");

	return GetDefaultObj()->Call<void>(Func, WorldContextObject, ActorClass, OutActors);
}

APlayerController* UGameplayStatics::GetPlayerController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.GameplayStatics.GetPlayerController");

	return GetDefaultObj()->Call<APlayerController*>(Func, WorldContextObject, PlayerIndex);
}