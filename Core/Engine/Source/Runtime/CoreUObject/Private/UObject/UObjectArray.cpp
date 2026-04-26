#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

UObject* FUObjectArray::FindObject(const std::string& FullName, bool bStrictCheck)
{
	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		std::string objectFullName = Object->GetFullName();
		if (bStrictCheck ? (objectFullName == FullName) : (objectFullName.contains(FullName)))
		{
			return Object;
		}
	}

	return nullptr;
}

UObject* FUObjectArray::FindObjectFast(const std::string& Name)
{
	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		if (Object->GetName().ToString() == Name)
			return static_cast<UObject*>(Object);
	}

	return nullptr;
}

UObject* FUObjectArray::DefaultObjImpl(const UClass* TargetClass)
{
	for (int i = 0; i < Num(); i++)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (Object && Object->IsDefaultObject() && Object->GetClass() == TargetClass)
			return Object;
	}
	return nullptr;
}

TArray<UObject*> FUObjectArray::GetObjectsOfClass(UClass* TargetClass, std::string MustIncludeString)
{
	TArray<UObject*> Objects;
	
	for (int i = 0; i < Num(); i++)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;
		if (!Object)
			continue;

		if (Object && Object->IsA(TargetClass)
			&& (MustIncludeString.empty() || Object->GetName().ToString().contains(MustIncludeString))) {
			Objects.Add(Object);
		}
	}

	return Objects;
}