#pragma once
#include "pch.h"

#include "NetDriver.h"

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
	static inline void (*TickFlushInternalOG)(UDemoNetDriver* This, float DeltaSeconds);
	static void TickFlushInternal(UDemoNetDriver* This, float DeltaSeconds);

	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUDemoNetDriver_TickFlushInternal()), TickFlushInternal, (LPVOID*)&TickFlushInternalOG);

		Log("Hooked UDemoNetDriver!");
	}
};