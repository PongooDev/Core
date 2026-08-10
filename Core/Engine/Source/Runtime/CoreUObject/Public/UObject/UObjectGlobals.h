#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUObjectGlobals, Log, All);

class FArchive;
class FCbWriter;
class FLinkerInstancingContext;
class FObjectPreSaveContext;
class FOutputDevice;
class FPackagePath;
class ITargetPlatform;
class UClass;
class UEnum;
class UFunction;
class UObject;
class UObjectBase;
class UPackage;
class UPackageMap;
class UScriptStruct;
class UWorld;
struct FCustomPropertyListNode;
struct FGuid;
struct FObjectInstancingGraph;
struct FObjectPostCDOCompiledContext;
struct FObjectPtr;
struct FPrimaryAssetId;
struct FStaticConstructObjectParameters;
struct FUObjectSerializeContext;
struct FWorldContext;

void BeginLoad(FUObjectSerializeContext* LoadContext, const TCHAR* DebugContext = nullptr);
void EndLoad(FUObjectSerializeContext* LoadContext);

struct FStaticConstructObjectParameters
{
	const UClass* Class = nullptr;
	UObject* Outer = nullptr;
	FName Name;
	EObjectFlags SetFlags = (EObjectFlags)0;
	EInternalObjectFlags InternalSetFlags = EInternalObjectFlags::None;
	bool bCopyTransientsFromClassDefaults = false;
	bool bAssumeTemplateIsArchetype = false;
	UObject* Template = nullptr;
	void* InstanceGraph = nullptr;
	UPackage* ExternalPackage = nullptr;

	FStaticConstructObjectParameters() = default;
	explicit FStaticConstructObjectParameters(const UClass* InClass) : Class(InClass) {}
};

UObject* StaticConstructObject_Internal(
	UClass* Class,
	UObject* Outer = nullptr,
	FName Name = FName(),
	EObjectFlags SetFlags = (EObjectFlags)0,
	UObject* Template = nullptr);

template<typename T>
FORCEINLINE T* NewObject(UObject* Outer, UClass* Class, FName Name = FName(), EObjectFlags Flags = (EObjectFlags)0, UObject* Template = nullptr)
{
	return (T*)StaticConstructObject_Internal(Class, Outer, Name, Flags, Template);
}

template<typename T>
FORCEINLINE T* NewObject(UObject* Outer = nullptr)
{
	return (T*)StaticConstructObject_Internal(T::StaticClass(), Outer);
}