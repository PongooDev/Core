#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class UFortItemDefinition;

struct FFortMcpQuestObjectiveInfo {
public:
	DefineUnrealStruct(FFortMcpQuestObjectiveInfo);

	DefineStructProperty(FName, BackendName);
	DefineStructProperty(FDataTableRowHandle, ObjectiveStatHandle);
	DefineStructProperty(TArray<FDataTableRowHandle>, AlternativeStatHandles);
	DefineStructProperty(uint8, ItemEvent);
	DefineStructProperty(TSoftObjectPtr<UFortItemDefinition>, ItemReference);
	DefineStructProperty(FString, ItemTemplateIdOverride);
	DefineStructProperty(FName, LinkSquadID);
	DefineStructProperty(int32, LinkSquadIndex);
	DefineStructProperty(uint8, LinkVaultTab);
	DefineStructProperty(uint8, LinkToItemManagement);
	DefineStructProperty(int32, Count);
	DefineStructProperty(int32, Stage);
	DefineStructProperty(bool, bHidden);
	DefineStructProperty(bool, bRequirePrimaryMissionCompletion);
	DefineStructProperty(bool, bCanProgressInZone);
	DefineStructProperty(bool, bDisplayDynamicAnnouncementUpdate);
	DefineStructProperty(bool, bDisplayDynamicStatusUpdate);
	DefineStructProperty(float, DynamicUpdateCompletionDelay);
public:
	uint8 Padding[0x110];
};