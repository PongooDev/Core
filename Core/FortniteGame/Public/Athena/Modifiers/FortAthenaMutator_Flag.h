#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Flag : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Flag);

	DefineUProperty(FText, GameMsgText_Intro);
};
