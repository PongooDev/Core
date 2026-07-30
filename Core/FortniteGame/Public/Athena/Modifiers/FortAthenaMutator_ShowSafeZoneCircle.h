#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_ShowSafeZoneCircle : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_ShowSafeZoneCircle);

	DefineUProperty(bool, bShowSafeZoneCircle);
};
