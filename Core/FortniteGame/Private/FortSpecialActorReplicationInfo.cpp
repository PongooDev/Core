#include "pch.h"
#include "FortniteGame/Public/FortSpecialActorReplicationInfo.h"

const FSpecialActorInitData::FLayout& FSpecialActorInitData::GetLayout()
{
	// FSlateBrush is 0x88 and FVector2D is 8 bytes on UE4, so the members pack as
	// ref / id / tag / playerstate / intervals / brush / scale / brush / scale.
	static const FLayout UE4Layout{
		/* SpecialActorRef   */ 0x00,
		/* UniqueID          */ 0x08,
		/* CategoryTag       */ 0x10,
		/* MinimapIconBrush  */ 0x28,
		/* MinimapScale      */ 0xB0,
		/* CompassIconBrush  */ 0xB8,
		/* CompassScale      */ 0x140,
		/* Size              */ 0x1D0
	};

	// UE5 widens FSlateBrush to 0xC0 and FVector2D to 16 bytes, and adds a FullScreenMap brush and scale
	// between the minimap and compass pair.
	static const FLayout UE5Layout{
		/* SpecialActorRef   */ 0x00,
		/* UniqueID          */ 0x08,
		/* CategoryTag       */ 0x0C,
		/* MinimapIconBrush  */ 0x20,
		/* MinimapScale      */ 0xE0,
		/* CompassIconBrush  */ 0x1C0,
		/* CompassScale      */ 0x280,
		/* Size              */ 0x3D0
	};

	return Version::Engine_Version >= 5.0 ? UE5Layout : UE4Layout;
}

FSpecialActorInitData* FSpecialActorInitData::Construct()
{
	if (!Finder::FindFSpecialActorInitData_Constructor())
		return nullptr;

	void* Memory = FMemory::Malloc(GetLayout().Size);
	if (!Memory)
		return nullptr;

	std::memset(Memory, 0, GetLayout().Size);

	void* (*Constructor)(void*) = decltype(Constructor)(ImageBase + Finder::FindFSpecialActorInitData_Constructor());
	Constructor(Memory);

	return (FSpecialActorInitData*)Memory;
}

void FSpecialActorInitData::Destruct()
{
	if (Finder::FindFSpecialActorInitData_Destructor()) {
		void (*Destructor)(void*) = decltype(Destructor)(ImageBase + Finder::FindFSpecialActorInitData_Destructor());
		Destructor(this);
	}

	FMemory::Free(this);
}

int32 AFortSpecialActorReplicationInfo::AddActorToReplicationList(FSpecialActorInitData* InitData)
{
	if (!InitData || !Finder::FindAFortSpecialActorReplicationInfo_AddActorToReplicationList())
		return INDEX_NONE;

	int32 (*AddActorToReplicationListInternal)(AFortSpecialActorReplicationInfo*, FSpecialActorInitData*) =
		decltype(AddActorToReplicationListInternal)(ImageBase + Finder::FindAFortSpecialActorReplicationInfo_AddActorToReplicationList());

	return AddActorToReplicationListInternal(this, InitData);
}

FName AFortSpecialActorReplicationInfo::RegisterSpecialActor(AActor* Actor, const FGameplayTag& CategoryTag,
	const FSlateBrush& MinimapIcon, const FVector2D& MinimapScale,
	const FSlateBrush& CompassIcon, const FVector2D& CompassScale)
{
	if (!Actor || !CategoryTag.IsValid())
		return FName();

	FSpecialActorInitData* InitData = FSpecialActorInitData::Construct();
	if (!InitData) {
		Log("AFortSpecialActorReplicationInfo::RegisterSpecialActor: could not construct FSpecialActorInitData");
		return FName();
	}

	const std::string IDString = CategoryTag.TagName.ToString().ToString() + "_" + Actor->GetName().ToString();

	InitData->GetSpecialActorRef() = Actor;
	InitData->GetUniqueID() = UKismetStringLibrary::Conv_StringToName(IDString);
	InitData->GetCategoryTag() = CategoryTag;
	InitData->GetMinimapScale() = MinimapScale;
	InitData->GetCompassScale() = CompassScale;

	std::memcpy(InitData->GetMinimapIconBrush(), &MinimapIcon, sizeof(FSlateBrush));
	std::memcpy(InitData->GetCompassIconBrush(), &CompassIcon, sizeof(FSlateBrush));

	FName NewID = FName();
	if (AddActorToReplicationList(InitData) != INDEX_NONE) {
		NewID = InitData->GetUniqueID();
		Log("AFortSpecialActorReplicationInfo::RegisterSpecialActor: registered " + IDString);
	}

	InitData->Destruct();
	return NewID;
}

FName AFortSpecialActorReplicationInfo::FindSpecialActorID(AActor* Actor)
{
	if (!Actor || !_HasSpecialActorRepList())
		return FName();

	TArray<FSpecialActorRepData>& List = SpecialActorRepList.SpecialActorList;

	for (int32 i = 0; i < List.Num(); i++) {
		FSpecialActorRepData& Entry = List.GetWithSize(i, FSpecialActorRepData::GetSize());

		if (Entry.SpecialActor == Actor)
			return Entry.SpecialActorID;
	}

	return FName();
}

bool AFortSpecialActorReplicationInfo::RemoveSpecialActor(const FName& SpecialActorID)
{
	if (!_HasSpecialActorRepList())
		return false;

	TArray<FSpecialActorRepData>& List = SpecialActorRepList.SpecialActorList;

	for (int32 i = 0; i < List.Num(); i++) {
		FSpecialActorRepData& Entry = List.GetWithSize(i, FSpecialActorRepData::GetSize());

		if (Entry.SpecialActorID == SpecialActorID) {
			List.RemoveAt(i, FSpecialActorRepData::GetSize());

			SpecialActorRepList.MarkArrayDirty();
			ForceNetUpdate();

			Log("AFortSpecialActorReplicationInfo::RemoveSpecialActor: removed " + SpecialActorID.ToString().ToString());
			return true;
		}
	}

	return false;
}
