#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class EUpdateCompletionStatus {
public:
	DefineUnrealEnum(EUpdateCompletionStatus);

	DefineEnumProperty(UpdateUnknown);
	DefineEnumProperty(UpdateSuccess);
	DefineEnumProperty(UpdateSuccess_NoChange);
	DefineEnumProperty(UpdateSuccess_NeedsReload);
	DefineEnumProperty(UpdateSuccess_NeedsRelaunch);
	DefineEnumProperty(UpdateSuccess_NeedsPatch);
	DefineEnumProperty(UpdateFailure_PatchCheck);
	DefineEnumProperty(UpdateFailure_HotfixCheck);
	DefineEnumProperty(UpdateFailure_NotLoggedIn);
};

class UMcpProfile : public UObject {
public:
	DefineUnrealClass(UMcpProfile);
};
