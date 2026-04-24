#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

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

	bool GetbIsAssociatingLevel() const
	{
		if (Version::Engine_Version == 4.16)
		{
			static UProperty* Prop = FindPropertyByName("bLocked");
			if (Prop)
			{
				int32 Offset = Prop->Offset_Internal;

				uint8 NextByte = *((uint8*)this + Offset + 1);

				return (NextByte & 0x01) != 0;
			}
		}

		return false;
	}

	void SetbIsAssociatingLevel(bool bInValue)
	{
		if (Version::Engine_Version == 4.16)
		{
			static UProperty* Prop = FindPropertyByName("bLocked");
			if (Prop)
			{
				int32 Offset = Prop->Offset_Internal;
				uint8* BytePtr = (uint8*)this + Offset + 1;
				if (bInValue)
				{
					*BytePtr |= 0x01;
				}
				else
				{
					*BytePtr &= ~0x01;
				}
			}
		}
	}
};