#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/UI/FortIndicator.h"

class UFortSlateHUDIndicator : public UFortIndicator {
public:
	DefineUnrealClass(UFortSlateHUDIndicator);

	DefineUProperty(bool, bClampToScreen);
	DefineUProperty(bool, bShowClampToScreenArrow);
};
