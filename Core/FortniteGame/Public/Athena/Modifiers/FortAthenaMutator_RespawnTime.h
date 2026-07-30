#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_RespawnTime : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_RespawnTime);

	DefineUProperty(float, RespawnTime);
};
