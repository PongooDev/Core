#include "pch.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"

#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

UFortItem* UFortItemDefinition::CreateTemporaryItemInstanceBP(int32 Count, int32 Level) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"CreateTemporaryItemInstanceBP"));

	return const_cast<UFortItemDefinition*>(this)->Call<UFortItem*>(Func, Count, Level);
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
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"GetMaxStackSize"));

		return const_cast<UFortItemDefinition*>(this)->Call<int32>(Func);
	}
}

bool UFortItemDefinition::IsStackable() const
{
	if (Version::Fortnite_Version > 3.6) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"IsStackable"));

		return const_cast<UFortItemDefinition*>(this)->Call<bool>(Func);
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
		|| ItemType == EFortItemType::GetIngredient()
		|| ItemType == EFortItemType::GetWeaponHarvest())
		return EFortQuickBars::GetSecondary();

	return EFortQuickBars::GetPrimary();
}