#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

#include "BuildingAutoNav.h"
#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/Data/FortSearchBounceData.h"
#include "FortniteGame/Public/Interaction/InteractionType.h"

class AFortPlayerPawn;
class UStaticMesh;

class ABuildingContainer : public ABuildingAutoNav {
public:
	DefineUnrealClass(ABuildingContainer);

	DefineUProperty(UStaticMesh*, SearchedMesh);
	DefineUProperty(FVector, LootSpawnLocation);
	DefineBitfieldUProperty(bAlreadySearched);
	DefineUProperty(FName, SearchLootTierGroup);
	DefineUProperty(FVector, LootFinalLocation);
	DefineBitfieldUProperty(bDestroyContainerOnSearch);
	DefineUProperty(FFortSearchBounceData, SearchBounceData);
	DefineUProperty(int32, ReplicatedLootTier);
	DefineBitfieldUProperty(bStartAlreadySearched_Athena);

public:
	static bool SpawnLoot(ABuildingContainer* This, AFortPlayerPawn* PlayerPawn, uint8 InSourceTypeFlag, uint8 InSpawnSource);

	void OnRep_bAlreadySearched();

	void BounceContainer();

	static inline void (*PostUpdateOG)(ABuildingContainer* This, uint8 PersistantState, void* ReservedRandomValues);
	static void PostUpdate(ABuildingContainer* This, uint8 PersistantState, void* ReservedRandomValues);

	void OnSetSearched();

	static void Hook() {
		HookEveryVTableIdx(
			ABuildingContainer::StaticClass(),
			Finder::FindABuildingContainer_SpawnLootVFT(),
			SpawnLoot
		);

		HookEveryVTableIdx(
			ABuildingContainer::StaticClass(),
			Finder::FindABuildingActor_PostUpdateVFT(),
			PostUpdate,
			(LPVOID*)&PostUpdateOG
		);

		Log("ABuildingContainer Hooked!");
	}
};