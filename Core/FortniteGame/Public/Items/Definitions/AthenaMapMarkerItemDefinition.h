#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UAthenaMapMarkerItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaMapMarkerItemDefinition);

	DefineUProperty(TSoftClassPtr<UClass>, TopperActorClass);
};
