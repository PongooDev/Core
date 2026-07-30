#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Info.h"

class UFortWorldRecord;
class UFortZoneRecord;

class AFortWorldManager : public AInfo {
public:
	DefineUnrealClass(AFortWorldManager);

	DefineUProperty(UFortWorldRecord*, CurrentWorldRecord);
	DefineUProperty(UFortZoneRecord*, CurrentZoneRecord);
};
