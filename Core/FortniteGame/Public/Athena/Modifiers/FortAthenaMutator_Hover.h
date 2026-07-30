#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Hover : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Hover);

	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
};
