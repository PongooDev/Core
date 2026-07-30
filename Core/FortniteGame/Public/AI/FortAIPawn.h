#pragma once
#include "pch.h"

#include "FortniteGame/Public/Pawns/FortPawn.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AFortAIPawn : public AFortPawn {
public:
	DefineUnrealClass(AFortAIPawn);

	DefineUProperty(AActor*, SpawnRift);
	DefineUProperty(FVector, SpawnRiftLocation);
	DefineUProperty(AActor*, SpawnSourceActor);
	DefineBitfieldUProperty(bNotifySpawnRift);
};
