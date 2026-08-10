#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetSerialization.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

FString UKismetSystemLibrary::GetEngineVersion()
{
	if (Finder::FindUKismetSystemLibrary_GetEngineVersion() != 0) {
		FString(*GetEngineVersionInternal)() = decltype(GetEngineVersionInternal)(ImageBase + Finder::FindUKismetSystemLibrary_GetEngineVersion());
		return GetEngineVersionInternal();
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.GetEngineVersion");

		if (!Func) {
			UE_LOG(LogEngine, Error, TEXT("UKismetSystemLibrary::GetEngineVersion: Failed to find function!"));
			return FString();
		}
		else {
			return GetDefaultObj()->Call<FString>(Func);
		}
	}
}

void UKismetSystemLibrary::ExecuteConsoleCommand(UObject* WorldContextObject, const FString& Command, APlayerController* Player)
{
	if (Finder::FindUKismetSystemLibrary_ExecuteConsoleCommand() != 0) {
		void (*ExecuteConsoleCommandInternal)(UObject * WorldContextObject, const FString & Command, APlayerController * Player) = decltype(ExecuteConsoleCommandInternal)(ImageBase + Finder::FindUKismetSystemLibrary_ExecuteConsoleCommand());
		ExecuteConsoleCommandInternal(WorldContextObject, Command, Player);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.ExecuteConsoleCommand");

		if (!Func) {
			UE_LOG(LogEngine, Error, TEXT("UKismetSystemLibrary::ExecuteConsoleCommand: Failed to find function!"));
			return;
		}

		return GetDefaultObj()->Call<void>(Func, WorldContextObject, Command, Player);
	}
}

bool UKismetSystemLibrary::LineTraceSingle(UObject* WorldContextObject, const FVector& Start, const FVector& End, uint8 TraceChannel, bool bTraceComplex, TArray<AActor*>& ActorsToIgnore, uint8 DrawDebugType, FHitResult* OutHit, bool bIgnoreSelf, const FLinearColor& TraceColor, const FLinearColor& TraceHitColor, float DrawTime)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.LineTraceSingle");

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.LineTraceSingle_NEW");

	if (!Func) {
		UE_LOG(LogEngine, Error, TEXT("UKismetSystemLibrary::LineTraceSingle: Failed to find function!"));
		return false;
	}

	return GetDefaultObj()->Call<bool>(Func, WorldContextObject, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHit, bIgnoreSelf, TraceColor, TraceHitColor, DrawTime);
}

FTimerHandle UKismetSystemLibrary::K2_SetTimer(UObject* Object, const FString& FunctionName, float Time, bool bLooping)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetSystemLibrary.K2_SetTimer");

	return GetDefaultObj()->Call<FTimerHandle>(Func, Object, FunctionName, Time, bLooping);
}

bool UKismetSystemLibrary::IsDedicatedServer(const UObject* WorldContextObject)
{
	UWorld* World = UWorld::GetWorld();
	return World && World->InternalGetNetMode() == NM_DedicatedServer;
}

bool UKismetSystemLibrary::IsServer(const UObject* WorldContextObject)
{
	UWorld* World = UWorld::GetWorld();
	return World && World->InternalGetNetMode() != NM_Client;
}

bool UKismetSystemLibrary::IsStandalone(const UObject* WorldContextObject)
{
	UWorld* World = UWorld::GetWorld();
	return World && World->InternalGetNetMode() == NM_Standalone;
}