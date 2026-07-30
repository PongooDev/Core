#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Volume.h"

class UFortPoiCollisionComponent;

class AFortPoiVolume : public AVolume {
public:
	DefineUnrealClass(AFortPoiVolume);

	DefineUProperty(UPrimitiveComponent*, PoiCollisionComp);
public:
	static inline void (*PostInitializeComponentsOG)(AFortPoiVolume* This);
	static void PostInitializeComponents(AFortPoiVolume* This);

	static void Hook();
};
