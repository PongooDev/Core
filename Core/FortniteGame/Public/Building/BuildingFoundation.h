#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingSMActor.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/OnlineReplStructs.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Box.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
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
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/WorldSettings.h"
#include "Engine\Source\Runtime\GameplayTags\Classes\GameplayTagAssetInterface.h"

struct FDynamicBuildingFoundationRepData
{
public:
	DefineUnrealStruct(FDynamicBuildingFoundationRepData);

	DefineStructProperty(FRotator, Rotation);
	DefineStructProperty(FVector, Translation);
	DefineStructProperty(EDynamicFoundationEnabledState, EnabledState);
public:
	uint8 Padding[0x40];
};

struct FBuildingFoundationStreamingData {
public:
	DefineUnrealStruct(FBuildingFoundationStreamingData);

	DefineStructProperty(FName, FoundationName);
	DefineStructProperty(FVector, FoundationLocation);
	DefineStructProperty(FBox, BoundingBox);
	DefineStructProperty(TArray<int32>, ChildStreamingDataIndices);
	DefineStructProperty(uint8, PersistentHLODLevelIndex);
};

class ABuildingFoundation : public ABuildingSMActor {
public:
	DefineUnrealClass(ABuildingFoundation);

	DefineBitfieldUProperty(bConditionalFoundation);
	DefineBitfieldUProperty(bServerStreamedInLevel);
	DefineBitfieldUProperty(bFoundationEnabled);

	DefineUProperty(FName, LevelToStream);
	DefineUProperty(ULevelStreaming*, LevelStreamInfo);
	DefineUProperty(TArray<TSoftObjectPtr<UWorld>>, AdditionalWorlds);
	DefineUProperty(FDynamicBuildingFoundationRepData, DynamicFoundationRepData);
	DefineUProperty(EDynamicFoundationEnabledState, FoundationEnabledState);
	DefineUProperty(FBuildingFoundationStreamingData, StreamingData);
	DefineUProperty(FTransform, DynamicFoundationTransform);

	void OnRep_ServerStreamedInLevel();

	void SetDynamicFoundationEnabled(bool bEnabled);
	static void execSetDynamicFoundationEnabled(ABuildingFoundation* Context, FFrame& Stack);

	void SetDynamicFoundationTransform(FTransform& NewTransform);
	static void execSetDynamicFoundationTransform(ABuildingFoundation* Context, FFrame& Stack);

	void EditorOnlyAddAdditionalWorld(FString& LevelPackageName);
	static void execEditorOnlyAddAdditionalWorld(ABuildingFoundation* Context, FFrame& Stack);

	void EditorOnlyClearAdditionalWorlds();
	static void execEditorOnlyClearAdditionalWorlds(ABuildingFoundation* Context, FFrame& Stack);

	TArray<FString> EditorOnlyGetAdditionalWorlds();
	static void execEditorOnlyGetAdditionalWorlds(ABuildingFoundation* Context, FFrame& Stack, TArray<FString>* Result);

	bool EditorOnlyRemoveAdditionalWorld(FString& LevelToRemove);
	static void execEditorOnlyRemoveAdditionalWorld(ABuildingFoundation* Context, FFrame& Stack, bool* Result);

	bool EditorOnlyAddTransformToPreviewLevel(FTransform& DeltaTransform);
	static void execEditorOnlyAddTransformToPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result);

	bool EditorOnlyLoadPreviewLevel(bool bCreateLevelInstance);
	static void execEditorOnlyLoadPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result);

	bool EditorOnlyUnloadPreviewLevel();
	static void execEditorOnlyUnloadPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result);

	static void execEditorOnlyEnterBatchPreview(UObject* Context, FFrame& Stack);
	static void execEditorOnlyExitBatchPreview(UObject* Context, FFrame& Stack);

	bool SelectAndSetupMyBuildingLevel(void* ReservedRandomValues = nullptr);

	bool StreamInMyBuilding(bool bOnlyAdd);

	static void SetupFoundations();
	static void SetupIslandScripting();

	static void DumpFoundations();

	void OnRep_FoundationEnabled(bool bOldEnabled);

	void OnRep_DynamicFoundationRepData();

	void OnRep_FoundationEnabledState();

	void OnRep_DynamicFoundationTransform();

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.SetDynamicFoundationEnabled", execSetDynamicFoundationEnabled);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.SetDynamicFoundationTransform", execSetDynamicFoundationTransform);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyAddAdditionalWorld", execEditorOnlyAddAdditionalWorld);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyClearAdditionalWorlds", execEditorOnlyClearAdditionalWorlds);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyGetAdditionalWorlds", execEditorOnlyGetAdditionalWorlds);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyRemoveAdditionalWorld", execEditorOnlyRemoveAdditionalWorld);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyAddTransformToPreviewLevel", execEditorOnlyAddTransformToPreviewLevel);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyLoadPreviewLevel", execEditorOnlyLoadPreviewLevel);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyUnloadPreviewLevel", execEditorOnlyUnloadPreviewLevel);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyEnterBatchPreview", execEditorOnlyEnterBatchPreview);
		ExecHook("Function /Script/FortniteGame.BuildingFoundation.EditorOnlyExitBatchPreview", execEditorOnlyExitBatchPreview);

		Log("ABuildingFoundation Hooked");
	}
};
