#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCore, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogInit, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogExit, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogOutputDevice, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogStreaming, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, All);

inline bool* GIsClient = (bool*)(ImageBase + ServerOffsets::GIsClient);
inline bool* GIsServer = (bool*)(ImageBase + ServerOffsets::GIsServer);

namespace CoreGlobals {
	inline void Init() {
		GIsClient = reinterpret_cast<bool*>(ImageBase + ServerOffsets::GIsClient);
		GIsServer = reinterpret_cast<bool*>(ImageBase + ServerOffsets::GIsServer);
	}
}