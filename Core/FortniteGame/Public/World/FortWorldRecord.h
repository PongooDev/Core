#pragma once
#include "pch.h"

#include "FortniteGame/Public/World/FortGenericRecord.h"

class UFortWorldRecord : public UFortGenericRecord {
public:
	DefineUnrealClass(UFortWorldRecord);

	DefineUProperty(int32, CurrentZoneIndex);
	DefineUProperty(FString, WUID);
};
