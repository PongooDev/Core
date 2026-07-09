#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/IntVector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

class UWorld;
class AActor;
class ABrush;
class AInstancedFoliageActor;
class ALevelBounds;
class APlayerController;
class AWorldSettings;
class FSceneInterface;
class ITargetPlatform;
class UAssetUserData;
class UMapBuildDataRegistry;
class UNavigationDataChunk;
class UTexture2D;
struct FLevelCollection;
class ULevelActorContainer;
class FLevelPartitionOperationScope;
class FRegisterComponentContext;
class ALevelScriptActor;
struct FVector;
class UModel;
class UModelComponent;
class ANavigationObjectBase;
class FTickTaskLevel;
class FPrecomputedLightVolume;

class ULevel : public UObject {
public:
	DefineUnrealClass(ULevel);

	/** Array of all actors in this level, used by FActorIteratorBase and derived classes */
	DefineUProperty(TArray<AActor*>, Actors);

	/**
	 * The World that has this level in its Levels array.
	 * This is not the same as GetOuter(), because GetOuter() for a streaming level is a vestigial world that is not used.
	 * It should not be accessed during BeginDestroy(), just like any other UObject references, since GC may occur in any order.
	 */
	DefineUProperty(UWorld*, OwningWorld);

	/** Whether the level is currently visible/ associated with the world */
	DefineBitfieldUProperty(bIsVisible);

	/** The level scripting actor, created by instantiating the class from LevelScriptBlueprint.  This handles all level scripting */
	DefineUProperty(ALevelScriptActor*, LevelScriptActor);

	DefineUProperty(AWorldSettings*, WorldSettings);

	DefineBitfieldUProperty(bLocked);
public:
	bool IsPersistentLevel() const;

	bool HasVisibilityChangeRequestPending() const;

	class ALevelScriptActor* GetLevelScriptActor() const;

	bool HasVisibilityRequestPending() const;

	bool IsCurrentLevel() const;
};