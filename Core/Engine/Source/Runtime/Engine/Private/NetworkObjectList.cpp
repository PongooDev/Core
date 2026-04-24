#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetworkObjectList.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

TSharedPtr<FNetworkObjectInfo>* FNetworkObjectList::Add(AActor* const Actor, const FName NetDriverName)
{
	TSharedPtr<FNetworkObjectInfo>* (*AddInternal)(FNetworkObjectList*, AActor* const, const FName) = decltype(AddInternal)(ImageBase + Finder::FindFNetworkObjectList_Add());
	return AddInternal(this, Actor, NetDriverName);
}

void FNetworkObjectList::Remove(AActor* const Actor)
{
	void (*RemoveInternal)(FNetworkObjectList*, AActor* const) = decltype(RemoveInternal)(ImageBase + Finder::FindFNetworkObjectList_Remove());
	RemoveInternal(this, Actor);
}