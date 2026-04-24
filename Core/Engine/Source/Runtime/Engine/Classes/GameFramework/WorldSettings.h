#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Info.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class UAssetUserData;
class UNetConnection;

struct FNetViewer
{
public:
	DefineUnrealStruct(FNetViewer);

	UNetConnection* Connection;

	/** The "controlling net object" associated with this view (typically player controller) */
	AActor* InViewer;

	/** The actor that is being directly viewed, usually a pawn.  Could also be the net actor of consequence */
	AActor* ViewTarget;

	/** Where the viewer is looking from */
	FVector ViewLocation;

	/** Direction the viewer is looking */
	FVector ViewDir;

	FNetViewer()
	{
		Connection = NULL;
		InViewer = NULL;
		ViewTarget = NULL;
		ViewLocation = FVector();
		ViewDir = FVector();
	}

	FNetViewer(UNetConnection* InConnection, float DeltaSeconds) {
		FNetViewer* (*ConstructorInternal)(FNetViewer*, UNetConnection*, float) = decltype(ConstructorInternal)(ImageBase + Finder::FindFNetViewer_Constructor());
		ConstructorInternal(this, InConnection, DeltaSeconds);
	}
};

class AWorldSettings : public AInfo {
public:
	DefineUnrealClass(AWorldSettings);

	DefineUProperty(TArray<FNetViewer>, ReplicationViewers);
};