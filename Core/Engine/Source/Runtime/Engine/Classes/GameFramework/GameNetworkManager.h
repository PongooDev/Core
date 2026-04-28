#pragma once
#include "pch.h"

#include "Info.h"

enum EStandbyType
{
	STDBY_Rx,
	STDBY_Tx,
	STDBY_BadPing,
	STDBY_MAX,
};

class AGameNetworkManager : public AInfo {
public:
	DefineUnrealClass(AGameNetworkManager);
public:
	// impl vtable calls for these
	bool IsInLowBandwidthMode();

	void StandbyCheatDetected(EStandbyType StandbyType);
};	