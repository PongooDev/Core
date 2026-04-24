#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameNetworkManager.h"

bool AGameNetworkManager::IsInLowBandwidthMode()
{
	return false;
}

void AGameNetworkManager::StandbyCheatDetected(EStandbyType StandbyType) {}