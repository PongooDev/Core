#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

void UObject::GetDefaultSubobjects(TArray<UObject*>& OutDefaultSubobjects)
{
	void (*GetDefaultSubobjectsInternal)(UObject*, TArray<UObject*>&) = decltype(GetDefaultSubobjectsInternal)(ImageBase + Finder::FindUObject_GetDefaultSubobjects());
	GetDefaultSubobjectsInternal(this, OutDefaultSubobjects);
}

UObject* UObject::GetDefaultSubobjectByName(FName ToFind)
{
	UObject* (*GetDefaultSubobjectByNameInternal)(UObject*, FName) = decltype(GetDefaultSubobjectByNameInternal)(ImageBase + Finder::FindUObject_GetDefaultSubobjectByName());
	return GetDefaultSubobjectByNameInternal(this, ToFind);
}

void UObject::PostInitProperties()
{
	void (*PostInitPropertiesInternal)(UObject*) = decltype(PostInitPropertiesInternal)(ImageBase + Finder::FindUObject_PostInitProperties());
	PostInitPropertiesInternal(this);
}

void UObject::PostLoad()
{
	void (*PostLoadInternal)(UObject*) = decltype(PostLoadInternal)(ImageBase + Finder::FindUObject_PostLoad());
	PostLoadInternal(this);
}

void UObject::BeginDestroy()
{
	void (*BeginDestroyInternal)(UObject*) = decltype(BeginDestroyInternal)(ImageBase + Finder::FindUObject_BeginDestroy());
	BeginDestroyInternal(this);
}

void UObject::FinishDestroy()
{
	void (*FinishDestroyInternal)(UObject*) = decltype(FinishDestroyInternal)(ImageBase + Finder::FindUObject_FinishDestroy());
	FinishDestroyInternal(this);
}

UFunction* UObject::FindFunction(FName InName) const
{
	return GetClass()->FindFunctionByName(InName, EIncludeSuperFlag::IncludeSuper);
}

UFunction* UObject::FindFunctionChecked(FName InName) const
{
	UFunction* (*FindFunctionCheckedInternal)(const UObject*, FName) = decltype(FindFunctionCheckedInternal)(ImageBase + Finder::FindUObject_FindFunctionChecked());
	return FindFunctionCheckedInternal(this, InName);
}

UObject* UObject::GetArchetypeFromRequiredInfo(UClass* Class, UObject* Outer, FName Name, EObjectFlags ObjectFlags)
{
	UObject* (*GetArchetypeFromRequiredInfoInternal)(UClass*, UObject*, FName, EObjectFlags) = decltype(GetArchetypeFromRequiredInfoInternal)(ImageBase + Finder::FindUObject_GetArchetypeFromRequiredInfo());
	return GetArchetypeFromRequiredInfoInternal(Class, Outer, Name, ObjectFlags);
}

void UObject::ProcessEvent(UFunction* Function, void* Parms)
{
	void (*ProcessEventInternal)(UObject*, UFunction*, void*) = decltype(ProcessEventInternal)(ImageBase + Finder::FindProcessEvent());
	ProcessEventInternal(this, Function, Parms);
}