#include "pch.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

bool UFortWorldItemDefinition::ServerExecute(UFortItem* Item, AFortPlayerController* Instigator) {
	// Index 0 means the finder failed - dispatching VTable[0] would call the scalar deleting destructor on this def.
	if (!Finder::FindUFortWorldItemDefinition_ServerExecuteVFT()) {
		Log("UFortWorldItemDefinition::ServerExecute: VFT index not found, skipping!");
		return false;
	}

	bool (*&ServerExecuteInternal)(UFortWorldItemDefinition * This, UFortItem * Item, AFortPlayerController * Instigator) = decltype(ServerExecuteInternal)(VTable[Finder::FindUFortWorldItemDefinition_ServerExecuteVFT()]);
	return ServerExecuteInternal(this, Item, Instigator);
}

UFortWorldItemDefinition* UFortWorldItemDefinition::GetAmmoWorldItemDefinition_BP() const
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetAmmoWorldItemDefinition_BP");

	return const_cast<UFortWorldItemDefinition*>(this)->Call<UFortWorldItemDefinition*>(Func);
}