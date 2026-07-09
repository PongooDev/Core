#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"

UObject* FUObjectArray::FindObject(const std::string& FullName, bool bStrictCheck, EClassCastFlags Flags)
{
	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (bStrictCheck ? (objectFullName == Utils::StringToLower(FullName)) : (objectFullName.contains(Utils::StringToLower(FullName)))
			&& (Flags == EClassCastFlags::RF_NoFlags || Object->IsA(Flags)))
		{
			return Object;
		}
	}

	return nullptr;
}

UObject* FUObjectArray::FindObjectWithClass(const std::string& FullName, bool bStrictCheck, UClass* Class)
{
	for (int i = 0; i < Num(); ++i)
	{
		FUObjectItem* Item = IndexToObject(i);
		if (!Item)
			continue;

		UObject* Object = (UObject*)Item->Object;

		if (!Object)
			continue;

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (bStrictCheck ? (objectFullName == Utils::StringToLower(FullName)) : (objectFullName.contains(Utils::StringToLower(FullName)))
			&& (!Class || Object->IsA(Class)))
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

		if (Utils::StringToLower(Object->GetName().ToString()) == Utils::StringToLower(Name))
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

TArray<UObject*> FUObjectArray::FindObjects(const std::string& SearchString)
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

		std::string objectFullName = Utils::StringToLower(Object->GetFullName());
		if (objectFullName.contains(Utils::StringToLower(SearchString)))
		{
			Objects.Add(Object);
		}
	}

	return Objects;
}