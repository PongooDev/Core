#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Building/BuildingActor.h"

class AFortDecoPreview;
class AFortDecoTool;
class UFortDecoItemDefinition;

class EFortDecoPlacementQueryResults {
public:
	DefineUnrealEnum(EFortDecoPlacementQueryResults);

	DefineEnumProperty(CanAdd);
	DefineEnumProperty(ExistingTrap);
	DefineEnumProperty(ExistingObject);
	DefineEnumProperty(Obstructed);
	DefineEnumProperty(NoLocation);
	DefineEnumProperty(WrongType);
	DefineEnumProperty(WrongShape);
	DefineEnumProperty(BeingModified);
	DefineEnumProperty(WrongTeam);
	DefineEnumProperty(BlueprintFailure);
	DefineEnumProperty(AbilityFailure);
	DefineEnumProperty(RequiresPlayerBuildableActor);
	DefineEnumProperty(NoEditPermission);
	DefineEnumProperty(WrongZone);
};
class EPlacementType {
public:
	DefineUnrealEnum(EPlacementType);

	DefineEnumProperty(Free);
	DefineEnumProperty(Grid);
	DefineEnumProperty(None);
};

class AFortDecoHelper : public AActor {
public:
	DefineUnrealClass(AFortDecoHelper);

	DefineUProperty(AFortDecoPreview*, DecoPreview);
	DefineUProperty(TSubclassOf<ABuildingActor>, PlacementActorClass);
	DefineUProperty(TWeakObjectPtr<AFortDecoTool>, DecoTool);
	DefineUProperty(float, DecoToolReach);
	DefineUProperty(EBuildingAttachmentType, CurrentAttachmentType);
	DefineUProperty(EPlacementType, CurrentPlacementType);
	DefineUProperty(UFortDecoItemDefinition*, DecoItemDefinition);
	DefineUProperty(bool, bInFallbackPosition);
	DefineUProperty(float, FallbackTinyScale);
	DefineUProperty(EFortDecoPlacementQueryResults, CanPlaceState);
	DefineUProperty(FText, CustomPlacementText);
	DefineUProperty(float, DesiredRelativeRotation);
	DefineUProperty(FVector, DirectionChange);
	DefineUProperty(float, GridSnapSize);
	DefineUProperty(FRotator, FreeFloorPlacementRelativeRotationOffset);
};
