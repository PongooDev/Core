#pragma once
#include "pch.h"

#include "FortniteGame/Public/Pawns/FortPlayerPawn.h"

class AFortPlayerPawnAthena : public AFortPlayerPawn {
public:
	DefineUnrealClass(AFortPlayerPawnAthena);

	DefineUProperty(float, LastFallDistance);
};