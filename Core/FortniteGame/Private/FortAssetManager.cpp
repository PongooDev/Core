#include "pch.h"
#include "FortniteGame/Public/FortAssetManager.h"

UFortAssetManager* UFortAssetManager::Get() {
	if (Finder::FindUFortAssetManager_Get()) {
		UFortAssetManager* (*GetInternal)() = decltype(GetInternal)(ImageBase + Finder::FindUFortAssetManager_Get());
		return GetInternal();
	}

	UObject* AssetManager = FUObjectArray::FindObject("Default__FortAssetManager", false);
	return AssetManager->Cast<UFortAssetManager>();
}