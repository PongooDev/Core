#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Info.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"

struct FSpecialActorInitData {
public:
	struct FLayout {
		int32 SpecialActorRef;
		int32 UniqueID;
		int32 CategoryTag;
		int32 MinimapIconBrush;
		int32 MinimapScale;
		int32 CompassIconBrush;
		int32 CompassScale;
		int32 Size;
	};

	static const FLayout& GetLayout();

	static FSpecialActorInitData* Construct();
	void Destruct();
public:
	AActor*& GetSpecialActorRef() { return GetFromOffset<AActor*>(this, GetLayout().SpecialActorRef); }
	FName& GetUniqueID() { return GetFromOffset<FName>(this, GetLayout().UniqueID); }
	FGameplayTag& GetCategoryTag() { return GetFromOffset<FGameplayTag>(this, GetLayout().CategoryTag); }
	FVector2D& GetMinimapScale() { return GetFromOffset<FVector2D>(this, GetLayout().MinimapScale); }
	FVector2D& GetCompassScale() { return GetFromOffset<FVector2D>(this, GetLayout().CompassScale); }

	void* GetMinimapIconBrush() { return (uint8*)this + GetLayout().MinimapIconBrush; }
	void* GetCompassIconBrush() { return (uint8*)this + GetLayout().CompassIconBrush; }
};

struct FSpecialActorRepData : public FFastArraySerializerItem {
public:
	DefineUnrealStruct(FSpecialActorRepData);

	DefineStructProperty(FName, SpecialActorID);
	DefineStructProperty(AActor*, SpecialActor);
public:
	uint8 Padding[0x200];
};

struct FSpecialActorRepDataArray : public FFastArraySerializer {
public:
	DefineUnrealStruct(FSpecialActorRepDataArray);

	DefineStructProperty(TArray<FSpecialActorRepData>, SpecialActorList);
public:
	uint8 Padding[0xC0];
};

class AFortSpecialActorReplicationInfo : public AInfo {
public:
	DefineUnrealClass(AFortSpecialActorReplicationInfo);

	DefineUProperty(FSpecialActorRepDataArray, SpecialActorRepList);
public:
	int32 AddActorToReplicationList(FSpecialActorInitData* InitData);

	FName RegisterSpecialActor(AActor* Actor, const FGameplayTag& CategoryTag,
		const FSlateBrush& MinimapIcon, const FVector2D& MinimapScale,
		const FSlateBrush& CompassIcon, const FVector2D& CompassScale);

	FName FindSpecialActorID(AActor* Actor);

	bool RemoveSpecialActor(const FName& SpecialActorID);
};
