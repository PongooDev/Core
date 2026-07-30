#include "pch.h"
#include "FortniteGame/Public/World/FortPoiVolume.h"

#include "Engine/Source/Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "Engine/Source/Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"

void AFortPoiVolume::PostInitializeComponents(AFortPoiVolume* This) {
	if (This && !This->BrushComponent) {
		UBrushComponent* NewBrush = NewObject<UBrushComponent>(This);
		if (NewBrush) {
			NewBrush->Brush = This->Brush;
			NewBrush->BrushBodySetup = NewObject<UBodySetup>(NewBrush);
			NewBrush->RegisterComponent();

			This->BrushComponent = NewBrush;
		}
	}

	PostInitializeComponentsOG(This);
}

void AFortPoiVolume::Hook() {
	MH_CreateHook(
		(LPVOID)(GetOffsetFromVTable(AFortPoiVolume::GetDefaultObj(), Finder::FindAActor_PostInitializeComponentsVFT())),
		PostInitializeComponents,
		(LPVOID*)&PostInitializeComponentsOG
	);

	Log("Hooked AFortPoiVolume");
}
