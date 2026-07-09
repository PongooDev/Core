#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"

UNavigationSystem* UNavigationSystem::CreateNavigationSystem(UWorld* WorldOwner)
{
	if (Finder::FindUNavigationSystem_CreateNavigationSystem()) {
		UNavigationSystem* (*CreateNavigationSystemInternal)(UWorld*) = decltype(CreateNavigationSystemInternal)(ImageBase + Finder::FindUNavigationSystem_CreateNavigationSystem());
		return CreateNavigationSystemInternal(WorldOwner);
	}
	
	return nullptr;
}