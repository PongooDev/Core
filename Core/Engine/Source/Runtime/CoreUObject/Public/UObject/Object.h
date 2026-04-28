#pragma once
#include "pch.h"

#include "UObjectBaseUtility.h"

class FConfigCacheIni;
class FEditPropertyChain;
class ITargetPlatform;
class ITransactionObjectAnnotation;
struct FFrame;
struct FObjectInstancingGraph;
struct FPropertyChangedChainEvent;
class UFunction;
class UWorld;

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
};