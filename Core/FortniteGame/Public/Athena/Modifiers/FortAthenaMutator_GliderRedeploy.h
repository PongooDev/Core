#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_GliderRedeploy : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GliderRedeploy);

	DefineUProperty(bool, bGliderRedeploy);
};
