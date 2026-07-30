#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/AssetManager.h"

class UFortGameData;

class UFortAssetManager final : public UAssetManager {
public:
	DefineUnrealClass(UFortAssetManager);
public:
	UFortAssetManager* Get();
};