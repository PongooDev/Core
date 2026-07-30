#include "pch.h"
#include "FortniteGame/Public/Events.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"

const FEvent* Events::GetEventForCurrentVersion()
{
	for (const FEvent& Event : Registry) {
		if (Event.Version == Version::Fortnite_Version) {
			return &Event;
		}
	}

	return nullptr;
}

AActor* Events::FindOrSpawnEventActor(const char* ClassPath)
{
	UWorld* World = UWorld::GetWorld();
	if (!World || !ClassPath) {
		return nullptr;
	}

	UClass* Class = StaticLoadObject<UClass>(ClassPath);
	if (!Class) {
		Log(std::string("Events: failed to load event class ") + ClassPath);
		return nullptr;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, Class, &Actors);

	AActor* Actor = Actors.Num() > 0 ? Actors[0] : nullptr;
	Actors.Free();

	if (!Actor) {
		Actor = World->SpawnActor(Class);
	}

	return Actor;
}

static void CallEventFunction(AActor* Target, UFunction* Function)
{
	auto Params = Function->GetParams();

	if (Params.Size == 0) {
		Target->ProcessEvent(Function, nullptr);
		return;
	}

	void* Mem = FMemory::Malloc(Params.Size);
	memset(Mem, 0, Params.Size);

	Target->ProcessEvent(Function, Mem);

	FMemory::Free(Mem);
}

static UFunction* ResolveEventFunction(AActor* Target, const char* FunctionPath)
{
	std::string Path = FunctionPath;

	size_t Sep = Path.find_last_of('.');
	std::string ShortName = Sep == std::string::npos ? Path : Path.substr(Sep + 1);

	return Target->FindFunction(ShortName.c_str());
}

bool Events::StartEvent()
{
	const FEvent* Event = GetEventForCurrentVersion();
	if (!Event) {
		Log("Events::StartEvent: no event registered for this version!");
		return false;
	}

	AActor* Loader = Event->LoaderClassPath ? FindOrSpawnEventActor(Event->LoaderClassPath) : nullptr;

	if (Loader && Event->LoadLevelFuncPath) {
		UFunction* LoadLevel = ResolveEventFunction(Loader, Event->LoadLevelFuncPath);
		if (LoadLevel) {
			CallEventFunction(Loader, LoadLevel);
			Log(std::string("Events::StartEvent: streamed event level via ") + Event->LoadLevelFuncPath);
		}
		else {
			Log(std::string("Events::StartEvent: failed to find level loader function ") + Event->LoadLevelFuncPath);
		}
	}

	AActor* Scripting = Event->ScriptingClassPath ? FindOrSpawnEventActor(Event->ScriptingClassPath) : nullptr;

	bool bFiredAny = false;

	for (const FEventFunction& EventFunction : Event->Functions) {
		AActor* Target = EventFunction.bCallOnLoader ? Loader : Scripting;
		if (!Target) {
			Log(std::string("Events::StartEvent: no target actor for ") + EventFunction.FunctionPath);
			continue;
		}

		UFunction* Function = ResolveEventFunction(Target, EventFunction.FunctionPath);
		if (!Function) {
			Log(std::string("Events::StartEvent: failed to find function ") + EventFunction.FunctionPath);
			continue;
		}

		CallEventFunction(Target, Function);
		Log(std::string("Events::StartEvent: fired ") + EventFunction.FunctionPath);

		bFiredAny = true;
	}

	return bFiredAny;
}
