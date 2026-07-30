#include "pch.h"
#include "FortniteGame/Public/Building/BuildingRift.h"

#include "FortniteGame/Public/AI/FortAIPawn.h"

void ABuildingRift::BroadcastRiftSpawningAI()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("BroadcastRiftSpawningAI");

	if (!Func) {
		Log("ABuildingRift::BroadcastRiftSpawningAI: Failed to find function!");
		return;
	}

	return const_cast<ABuildingRift*>(this)->Call<void>(Func);
}

void ABuildingRift::OnRep_CosmeticState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CosmeticState");

	if (!Func) {
		Log("ABuildingRift::OnRep_CosmeticState: Failed to find function!");
		return;
	}

	return const_cast<ABuildingRift*>(this)->Call<void>(Func);
}

void ABuildingRift::OnRep_RiftIsActive()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_RiftIsActive");

	if (!Func) {
		Log("ABuildingRift::OnRep_RiftIsActive: Failed to find function!");
		return;
	}

	return const_cast<ABuildingRift*>(this)->Call<void>(Func);
}

void ABuildingRift::OnRep_RiftIsVisible()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_RiftIsVisible");

	if (!Func) {
		Log("ABuildingRift::OnRep_RiftIsVisible: Failed to find function!");
		return;
	}

	return const_cast<ABuildingRift*>(this)->Call<void>(Func);
}

void ABuildingRift::Hook()
{
	Log("Hooked ABuildingRift");
}
