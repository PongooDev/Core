#pragma once
#include "pch.h"

#include "UObjectBaseUtility.h"
#include "UObjectArray.h"

class FConfigCacheIni;
class FEditPropertyChain;
class ITargetPlatform;
class ITransactionObjectAnnotation;
struct FFrame;
struct FObjectInstancingGraph;
struct FPropertyChangedChainEvent;
class UFunction;
class UWorld;
class UClass;

class UObject : public UObjectBaseUtility {
public:
	UFunction* FindFunction(FName InName) const;

	static UObject* GetArchetypeFromRequiredInfo(UClass* Class, UObject* Outer, FName Name, EObjectFlags ObjectFlags);

	UObject* GetArchetype() const
	{
		return GetArchetypeFromRequiredInfo(GetClass(), GetOuter(), GetFName(), ObjectFlags);
	}

	void ProcessEvent(UFunction* Function, void* Parms);

	UWorld* GetWorld() const;

	static bool NeedsLoadForClient(UObject* This);

	static bool CanCreateInCurrentContext(UObject* Template);

	template <typename Ret = void, typename... Args>
	Ret Call(UFunction* Function, Args&&... args);

	static void Hook();

	static UClass* StaticClass() {
		static UClass* Class = (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Object");
		return Class;
	}
};