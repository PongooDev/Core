#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/FortEnums.h"

class UFortAthenaExitCraftInfo;

class AFortAthenaExitCraft;
class AFortAthenaExitCraftSpawner;

class AFortAthenaExitCraft : public AActor {
public:
	DefineUnrealClass(AFortAthenaExitCraft);

	DefineUProperty(UFortAthenaExitCraftInfo*, ExitCraftInfo);
	DefineUProperty(EExitCraftState, CurrentState);
	DefineUProperty(FName, CraftSpecialActorID);
public:
	void OnNewState(uint8 NewState) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("OnNewState");

		if (Func)
			Call(Func, NewState);
	}

	void OnRep_CurrentState() {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("OnRep_CurrentState");

		if (Func)
			Call(Func);
	}

	void CheatDepart() {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("CheatDepart");

		if (Func)
			Call(Func);
	}
};
