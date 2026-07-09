#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
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
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

#include "FortWeapon.h"
#include "FortniteGame/Public/FortEnums.h"

class ABuildingActor;
class ABuildingSMActor;
class UFortItemDefinition;
class AFortDecoHelper;

class AFortDecoTool : public AFortWeapon {
public:
	DefineUnrealClass(AFortDecoTool);

	DefineUProperty(bool, bButtonDown);
	DefineUProperty(UFortItemDefinition*, ItemDefinition);
	DefineUProperty(AFortDecoHelper*, DecoHelper);
	DefineUProperty(AActor*, CarriedActor);
	DefineUProperty(bool, bPlaceCarriedActor);
public:
	void ServerSpawnDeco(FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType);
	static inline void (*execServerSpawnDecoOG)(AFortDecoTool* Context, FFrame& Stack);
	static void execServerSpawnDeco(AFortDecoTool* Context, FFrame& Stack);

	bool ShouldAllowServerSpawnDeco(FVector& InLocation, FRotator& InRotation, ABuildingSMActor* AttachedActor, uint8 InBuildingAttachmentType);

	ABuildingActor* SpawnDeco(
		UClass* SpawnClass,
		FVector& Location,
		FRotator& Rotation,
		ABuildingSMActor* AttachedActor,
		uint8 InBuildingAttachmentType,
		int32 PlacementReason
	);

	static void Hook() {
		/*HookEveryVTable(
			AFortDecoTool::StaticClass(),
			AFortDecoTool::StaticClass()->GetFunction("Function /Script/FortniteGame.FortDecoTool.ServerSpawnDeco"),
			ServerSpawnDeco,
			(LPVOID*)&ServerSpawnDecoOG
		);*/
		ExecHook("Function /Script/FortniteGame.FortDecoTool.ServerSpawnDeco", execServerSpawnDeco, execServerSpawnDecoOG);

		Log("Hooked AFortDecoTool");
	}
};