#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"

class AActor;
class UNetConnection;

struct FNetworkObjectInfo
{
	AActor* Actor;

	TWeakObjectPtr<AActor> WeakActor;

	double NextUpdateTime;

	double LastNetReplicateTime;

	float OptimalNetUpdateDelta;

	float LastNetUpdateTime;

	uint32 bPendingNetUpdate : 1;

	uint32 bForceRelevantNextUpdate : 1;

	TSet<TWeakObjectPtr<UNetConnection>> DormantConnections;

	TSet<TWeakObjectPtr<UNetConnection>> RecentlyDormantConnections;

	FNetworkObjectInfo()
		: Actor(nullptr)
		, NextUpdateTime(0.0)
		, LastNetReplicateTime(0.0)
		, OptimalNetUpdateDelta(0.0f)
		, LastNetUpdateTime(0.0f)
		, bPendingNetUpdate(false)
		, bForceRelevantNextUpdate(false) {
	}

	FNetworkObjectInfo(AActor* InActor)
		: Actor(InActor)
		, WeakActor(InActor)
		, NextUpdateTime(0.0)
		, LastNetReplicateTime(0.0)
		, OptimalNetUpdateDelta(0.0f)
		, LastNetUpdateTime(0.0f)
		, bPendingNetUpdate(false)
		, bForceRelevantNextUpdate(false) {
	}
};

class FNetworkObjectList
{
public:
	typedef TSet<TSharedPtr<FNetworkObjectInfo>> FNetworkObjectSet;

	const FNetworkObjectSet& GetActiveObjects() const { return ActiveNetworkObjects; }

	TSharedPtr<FNetworkObjectInfo>* Add(AActor* const Actor, const FName NetDriverName);

	void Remove(AActor* const Actor);
public:
	FNetworkObjectSet AllNetworkObjects;
	FNetworkObjectSet ActiveNetworkObjects;
	FNetworkObjectSet ObjectsDormantOnAllConnections;

	TMap<TWeakObjectPtr<UNetConnection>, int32> NumDormantObjectsPerConnection;
};