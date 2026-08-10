#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogEngine, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogActor, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSpawn, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWorld, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogLevel, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogLevelStreaming, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGameMode, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerController, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogPlayerManagement, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogBlueprintUserMessages, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogDataTable, Log, All);

DECLARE_LOG_CATEGORY_EXTERN(LogNet, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogRep, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogDemo, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNetTraffic, Warning, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNetDormancy, Warning, All);
DECLARE_LOG_CATEGORY_EXTERN(LogNetPlayerMovement, Warning, All);
