#pragma once
#include "pch.h"

#include "FortGameSession.h"

class AFortGameSessionDedicated : public AFortGameSession {
public:
	DefineUnrealClass(AFortGameSessionDedicated);

	DefineUProperty(float, HotfixCheckTimer);
};