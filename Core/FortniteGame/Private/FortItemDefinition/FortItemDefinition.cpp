#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

class UFortItem* UFortItemDefinition::CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"CreateTemporaryItemInstanceBP"));

	struct FortItemDefinition_CreateTemporaryItemInstanceBP
	{
	public:
		int32 Count;
		int32 Level;
		UFortItem* ReturnValue;
	};

	FortItemDefinition_CreateTemporaryItemInstanceBP Parms{};

	Parms.Count = Count;
	Parms.Level = Level;

	const_cast<UFortItemDefinition*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

int32 UFortItemDefinition::GetMaxStackSize() const
{
	static UProperty* Prop = StaticClass()->FindPropertyByName("MaxStackSize");
	if (Prop)
	{
		int32 Size = Prop->ElementSize;
		int32 Offset = Prop->Offset_Internal;
		if (Size == 4) {
			return GetFromOffset<int32>(this, Offset);
		}

		return (int32)MaxStackSize.Evaluate();
	}
	else
	{
		static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetMaxStackSize"));
		if (Function) {
			static uintptr_t VTableIdx = GetVTableIndex(Function);

			int32 (*&GetMaxStackSizeInternal)(const UFortItemDefinition*) = decltype(GetMaxStackSizeInternal)(VTable[VTableIdx]);
			return GetMaxStackSizeInternal(this);
		}

		return -1;
	}
}

bool UFortItemDefinition::IsStackable() const
{
	if (Version::Fortnite_Version >= 1.8) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"IsStackable"));

		struct FortItemDefinition_IsStackable final
		{
		public:
			bool ReturnValue;
		};

		FortItemDefinition_IsStackable Parms{};

		const_cast<UFortItemDefinition*>(this)->ProcessEvent(Func, &Parms);

		return Parms.ReturnValue;
	}
	else {
		if (GetMaxStackSize() > 1)
			return true;
	}

	return false;
}

uint8 UFortItemDefinition::GetQuickBarForItem() {
	if (ItemType == EFortItemType::GetWeaponHarvest())
		return EFortQuickBars::GetMax_None();

	if (ItemType == EFortItemType::GetWorldResource() 
		|| ItemType == EFortItemType::GetAmmo()
		|| ItemType == EFortItemType::GetTrap()
		|| ItemType == EFortItemType::GetBuildingPiece()
		|| ItemType == EFortItemType::GetEditTool()
		|| ItemType == EFortItemType::GetIngredient())
		return EFortQuickBars::GetSecondary();

	return EFortQuickBars::GetPrimary();
}