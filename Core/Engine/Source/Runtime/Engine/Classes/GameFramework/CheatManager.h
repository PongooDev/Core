#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UCheatManager : public UObject {
public:
	DefineUnrealClass(UCheatManager);
	void DestroyTarget();
};