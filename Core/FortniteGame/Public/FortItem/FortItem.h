#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

class AFortPlayerController;
class UFortItemDefinition;
class UFortAccountItemDefinition;

struct FFortItemEntry;

class UFortItem : public UObject {
public:
	DefineUnrealClass(UFortItem);
public:
	void SetOwningControllerForTemporaryItem(AFortPlayerController* InController);

	FGuid GetItemGuid() const;

	AFortPlayerController* GetOwningController() const;
};