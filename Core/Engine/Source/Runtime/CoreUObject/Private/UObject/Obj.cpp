#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

UFunction* UObject::FindFunction(FName InName) const
{
	if (!this)
		return nullptr;
	if (InName.IsNone())
		return nullptr;

	return GetClass()->FindFunctionByName(InName, EIncludeSuperFlag::IncludeSuper);
}

UObject* UObject::GetArchetypeFromRequiredInfo(UClass* Class, UObject* Outer, FName Name, EObjectFlags ObjectFlags)
{
	UObject* (*GetArchetypeFromRequiredInfoInternal)(UClass*, UObject*, FName, EObjectFlags) = decltype(GetArchetypeFromRequiredInfoInternal)(ImageBase + Finder::FindUObject_GetArchetypeFromRequiredInfo());
	return GetArchetypeFromRequiredInfoInternal(Class, Outer, Name, ObjectFlags);
}

void UObject::ProcessEvent(UFunction* Function, void* Parms)
{
	if (!this)
		return;
	if (!Function)
		return;

	if (!IsValidLowLevelFast())
		return;

	void (*&ProcessEventInternal)(UObject*, UFunction*, void*) = decltype(ProcessEventInternal)(VTable[Finder::FindProcessEventVFT()]);
	ProcessEventInternal(this, Function, Parms);
}

UWorld* UObject::GetWorld() const
{
	if (Finder::FindUObject_GetWorldVFT()) {
		UWorld* (*&GetWorldInternal)(const UObject*) = decltype(GetWorldInternal)(VTable[Finder::FindUObject_GetWorldVFT()]);
		return GetWorldInternal(this);
	}
	else {
		return UWorld::GetWorld();
	}
}

bool UObject::NeedsLoadForClient(UObject* This)
{
	return true;
}

bool UObject::CanCreateInCurrentContext(UObject* Template)
{
	return true;
}

void UObject::Hook() {
	/*HookEveryVTableIdx(
		UObject::StaticClass(),
		Finder::FindUObject_NeedsLoadForClientVFT(),
		NeedsLoadForClient,
		nullptr,
		true
	);*/

	MH_CreateHook(
		(LPVOID*)(ImageBase + Finder::FindUObject_CanCreateInCurrentContext()),
		CanCreateInCurrentContext,
		nullptr
	);
	
	Log("Hooked UObject");
}