#include "pch.h"
#include "FortniteGame/Public/Capture/AthenaBarrierObjective.h"

void AAthenaBarrierObjective::OnGeneratorDestroyed()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGeneratorDestroyed");

	if (!Func) {
		Log("AAthenaBarrierObjective::OnGeneratorDestroyed: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBarrierObjective::OnRep_FoodTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_FoodTeam");

	if (!Func) {
		Log("AAthenaBarrierObjective::OnRep_FoodTeam: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBarrierObjective::OnRep_HeadRotationYaw()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_HeadRotationYaw");

	if (!Func) {
		Log("AAthenaBarrierObjective::OnRep_HeadRotationYaw: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBarrierObjective::OnRep_ObjectiveDamageState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_ObjectiveDamageState");

	if (!Func) {
		Log("AAthenaBarrierObjective::OnRep_ObjectiveDamageState: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

EBarrierFoodTeam AAthenaBarrierObjective::GetFoodTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetFoodTeam");

	if (!Func) {
		Log("AAthenaBarrierObjective::GetFoodTeam: Failed to find function!");
		return EBarrierFoodTeam::GetMAX();
	}

	return Call<EBarrierFoodTeam>(Func);
}

EBarrierObjectiveDamageState AAthenaBarrierObjective::GetObjectiveDamageState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetObjectiveDamageState");

	if (!Func) {
		Log("AAthenaBarrierObjective::GetObjectiveDamageState: Failed to find function!");
		return EBarrierObjectiveDamageState::GetMAX();
	}

	return Call<EBarrierObjectiveDamageState>(Func);
}

bool AAthenaBarrierObjective::IsFriendlyTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsFriendlyTeam");

	if (!Func) {
		Log("AAthenaBarrierObjective::IsFriendlyTeam: Failed to find function!");
		return false;
	}

	return Call<bool>(Func);
}


void AAthenaBarrierObjective::SetFoodTeam(EBarrierFoodTeam NewFoodTeam)
{
	if (Role != ROLE_Authority)
		return;

	FoodTeam = NewFoodTeam;

	if (NewFoodTeam == 3) // EBarrierFoodTeam::MAX
		return;
	OnRep_FoodTeam(); // im not gonna redo the setheadmesh internal and this onrep calls it anyway
}
