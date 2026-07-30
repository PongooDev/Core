#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_CreativeTutorial : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_CreativeTutorial);

	DefineUProperty(FAthenaGameMessageData, GameMsg_LandOnHub);
	DefineUProperty(FAthenaGameMessageData, GameMsg_LandOnIsland);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Inventory);
	DefineUProperty(FAthenaGameMessageData, GameMsg_IslandMenu);
	DefineUProperty(FAthenaGameMessageData, GameMsg_BackToHub);
	DefineUProperty(FAthenaGameMessageData, GameMsg_ExploreFriendsIslands);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Permissions);
	DefineUProperty(FAthenaGameMessageData, ServerShutdownInitiated);
	DefineUProperty(bool, bHasSeenInventoryTutorial);
	DefineUProperty(bool, bHasSeenReturnToCreativeHubTutorial);
	DefineUProperty(bool, bHasSeenMyIslandTutorial);
	DefineUProperty(bool, bHasSeenWelcomeTutorial);
	DefineUProperty(bool, bHasSeenPermissionsTutorial);
	DefineUProperty(bool, bCanShowMyIslandMessage);
	DefineUProperty(float, ServerShutdownTimeRemaining);
};
