#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPawn.h"

#include "FortniteGame/Public/FortSet/FortHealthSet.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"

AFortWeapon* AFortPawn::EquipWeaponDefinition(const UFortWeaponItemDefinition* WeaponData, const FGuid& ItemEntryGuid)
{
	if (!this) {
		Log("EquipWeaponDefinition: This is null!");
		return nullptr;
	}

	if (!WeaponData) {
		Log("EquipWeaponDefinition: WeaponData is null!");
		return nullptr;
	}

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"EquipWeaponDefinition"));

	return const_cast<AFortPawn*>(this)->Call<AFortWeapon*>(Func, WeaponData, ItemEntryGuid);
}

void AFortPawn::OnRep_CurrentWeapon(AFortWeapon* OldWeapon)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentWeapon");

	if (!Func) {
		Log("OnRep_CurrentWeapon: Failed to find function!");
		return;
	}

	return const_cast<AFortPawn*>(this)->Call<void>(Func, OldWeapon);
}

void AFortPawn::SetHealth(float NewHealthVal)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetHealth");

	return const_cast<AFortPawn*>(this)->Call<void>(Func, NewHealthVal);
}

void AFortPawn::SetMaxHealth(float NewHealthVal)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SetMaxHealth");

	return const_cast<AFortPawn*>(this)->Call<void>(Func, NewHealthVal);
}

void AFortPawn::SetShield(float NewShieldValue)
{
	if (Version::Fortnite_Version > 3.6) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("SetShield");

		return const_cast<AFortPawn*>(this)->Call<void>(Func, NewShieldValue);
	}
	else {
		if (HealthSet) {
			HealthSet->CurrentShield.BaseValue = NewShieldValue;
			HealthSet->CurrentShield.CurrentValue = NewShieldValue;
			HealthSet->OnRep_CurrentShield();
			ForceNetUpdate();
		}
		else {
			Log("SetShield: HealthSet is null!");
		}
	}
}

void AFortPawn::SetMaxShield(float NewValue)
{
	if (Version::Fortnite_Version > 3.0) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("SetMaxShield");

		return const_cast<AFortPawn*>(this)->Call<void>(Func, NewValue);
	}
	else {
		if (HealthSet) {
			HealthSet->Shield.BaseValue = NewValue;
			HealthSet->Shield.CurrentValue = NewValue;
			HealthSet->Shield.Maximum = NewValue;
			HealthSet->OnRep_Shield();
			ForceNetUpdate();
		}
		else {
			Log("SetMaxShield: HealthSet is null!");
		}
	}
}

float AFortPawn::GetMaxHealth() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMaxHealth");

	return const_cast<AFortPawn*>(this)->Call<float>(Func);
}

float AFortPawn::GetMaxShield() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMaxShield");

	return const_cast<AFortPawn*>(this)->Call<float>(Func);
}

AFortWeapon* AFortPawn::FindWeapon(UFortItemDefinition* ItemDef)
{
	if (!ItemDef) {
		Log("FindWeapon: ItemDef is null!");
		return nullptr;
	}

	if (!CurrentWeaponList.IsValid()) {
		Log("FindWeapon: CurrentWeaponList is not valid!");
		return nullptr;
	}

	for (AFortWeapon* Weapon : CurrentWeaponList) {
		if (Weapon && Weapon->WeaponData && Weapon->WeaponData == ItemDef) {
			return Weapon;
		}
	}

	return nullptr;
}

AFortWeapon* AFortPawn::FindWeapon(FGuid Guid)
{
	if (!Guid.IsValid()) {
		Log("FindWeapon: Guid is not valid!");
		return nullptr;
	}

	if (!CurrentWeaponList.IsValid()) {
		Log("FindWeapon: CurrentWeaponList is not valid!");
		return nullptr;
	}

	for (AFortWeapon* Weapon : CurrentWeaponList) {
		if (Weapon && Weapon->ItemEntryGuid == Guid) {
			return Weapon;
		}
	}

	return nullptr;
}

void AFortPawn::OnRep_IsDBNO()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_IsDBNO");

	return const_cast<AFortPawn*>(this)->Call<void>(Func);
}

bool AFortPawn::IsDBNO() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsDBNO");

	if (!Func) {
		return bIsDBNO;
	}

	return const_cast<AFortPawn*>(this)->Call<bool>(Func);
}