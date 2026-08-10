#pragma once
#include "pch.h"

#include "NetDriver.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

class Error;
class FNetworkNotify;
class UDemoNetDriver;
class UDemoNetConnection;
class FRepState;

class UDemoNetDriver : public UNetDriver {
public:
	DefineUnrealClass(UDemoNetDriver);

	DefineUProperty(float, CheckpointSaveMaxMSPerFrame);
	DefineUProperty(bool, bIsLocalReplay);
public:
	static void Hook() {
		Log("Hooked UDemoNetDriver!");
	}
};