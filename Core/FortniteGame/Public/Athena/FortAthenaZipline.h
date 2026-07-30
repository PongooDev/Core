#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

class AFortAthenaZipline : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AFortAthenaZipline);

	DefineUProperty(FVector, StartPosition);
	DefineUProperty(FVector, EndPosition);
	DefineUProperty(bool, bInitialized);
public:
	void Initialize(const FVector& NewStartPosition, const FVector& NewEndPosition);

	bool UseInteractToEnterZipline();
};
