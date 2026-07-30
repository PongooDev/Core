#include "pch.h"
#include "FortniteGame/Public/Capture/AthenaBarrierFlag.h"

#include "FortniteGame/Public/Capture/AthenaBarrierObjective.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/ChildActorComponent.h"

void AAthenaBarrierFlag::OnRep_CurrentState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentState");

	if (!Func) {
		Log("AAthenaBarrierFlag::OnRep_CurrentState: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

void AAthenaBarrierFlag::OnRep_FoodTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_FoodTeam");

	if (!Func) {
		Log("AAthenaBarrierFlag::OnRep_FoodTeam: Failed to find function!");
		return;
	}

	return Call<void>(Func);
}

UStaticMesh* AAthenaBarrierFlag::GetHeadMesh()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetHeadMesh");

	if (!Func) {
		Log("AAthenaBarrierFlag::GetHeadMesh: Failed to find function!");
		return nullptr;
	}

	return Call<UStaticMesh*>(Func);
}

FVector AAthenaBarrierFlag::GetMeshScale()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMeshScale");

	if (!Func) {
		Log("AAthenaBarrierFlag::GetMeshScale: Failed to find function!");
		return FVector();
	}

	return Call<FVector>(Func);
}

void AAthenaBarrierFlag::SetFoodTeam(EBarrierFoodTeam NewFoodTeam)
{
	if (Role != ROLE_Authority)
		return;

	FoodTeam = NewFoodTeam;

	if (NewFoodTeam == EBarrierFoodTeam::GetMAX())
		return;

	OnRep_FoodTeam();
}

AAthenaBarrierObjective* AAthenaBarrierFlag::GetObjectiveActor(AAthenaBarrierFlag* This)
{
	if (!This)
		return nullptr;

	TArray<UActorComponent*> Components;
	This->GetComponents(Components);

	AAthenaBarrierObjective* Result = nullptr;
	int32 ChildActorComponents = 0;

	for (int32 i = 0; i < Components.Num(); i++)
	{
		UChildActorComponent* ChildActorComponent = Components[i] ? Components[i]->Cast<UChildActorComponent>() : nullptr;
		if (!ChildActorComponent || !ChildActorComponent->_HasChildActor())
			continue;

		ChildActorComponents++;

		if (AAthenaBarrierObjective* Objective = ChildActorComponent->ChildActor->Cast<AAthenaBarrierObjective>())
		{
			Result = Objective;
			break;
		}
	}

	Components.Free();

	return Result;
}

void AAthenaBarrierFlag::Hook()
{
	HookEveryVTable(
		AAthenaBarrierFlag::StaticClass(),
		AAthenaBarrierFlag::StaticClass()->GetFunction("Function /Script/FortniteGame.AthenaBarrierFlag.GetObjectiveActor"),
		(void*)GetObjectiveActor,
		(LPVOID*)&GetObjectiveActorOG
	);

	Log("AAthenaBarrierFlag Hooked!");
}
