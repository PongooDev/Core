#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

UClass* UField::GetOwnerClass() const
{
	UClass* (*GetOwnerClassInternal)(const UField*) = decltype(GetOwnerClassInternal)(ImageBase + Finder::FindUField_GetOwnerClass());
	return GetOwnerClassInternal(this);
}

UStruct* UField::GetOwnerStruct() const
{
	UStruct* (*GetOwnerStructInternal)(const UField*) = decltype(GetOwnerStructInternal)(ImageBase + Finder::FindUField_GetOwnerStruct());
	return GetOwnerStructInternal(this);
}

UProperty* UStruct::FindPropertyByName(FName InName) const
{
	UProperty* (*FindPropertyByNameInternal)(const UStruct*, FName) = decltype(FindPropertyByNameInternal)(ImageBase + Finder::FindUStruct_FindPropertyByName());
	return FindPropertyByNameInternal(this, InName);
}

UProperty* UStruct::FindPropertyByName(std::string InName) const
{
	if (Finder::FindUStruct_FindPropertyByName()) {
		FString PropertyNameString = FString(std::wstring(InName.begin(), InName.end()).c_str());
		FName PropertyName = UKismetStringLibrary::Conv_StringToName(PropertyNameString);
		UProperty* Prop = FindPropertyByName(PropertyName);
		if (!Prop) {
			//Log("Failed to find property for object: " + GetFName().ToString().ToString() + " with name: " + InName);
			return nullptr;
		}
		return Prop;
	}
	else {
		for (UField* Field = GetChildrenOrChildProperties(); Field; Field = Field->Next)
		{
			if (!Field) continue;
			UProperty* Prop = (UProperty*)Field;

			if (Prop && Prop->GetName().ToString() == InName) {
				return Prop;
			}
		}

		if (SuperStruct) {
			return SuperStruct->FindPropertyByName(InName);
		}
	}

	return nullptr;
}

uintptr_t UStruct::GetPropertyOffset(std::string InName) const
{
	FString PropertyNameString = FString(std::wstring(InName.begin(), InName.end()).c_str());
	FName PropertyName = UKismetStringLibrary::Conv_StringToName(PropertyNameString);
	UProperty* Prop = FindPropertyByName(PropertyName);
	if (!Prop) {
		return 0;
	}
	return Prop->Offset_Internal;
}

bool UStruct::IsChildOf(const UStruct* SomeBase) const {
	bool (*IsChildOfInternal)(const UStruct*, const UStruct*) = decltype(IsChildOfInternal)(ImageBase + Finder::FindUStruct_IsChildOf());
	return IsChildOfInternal(this, SomeBase);
}

UFunction* UClass::FindFunctionByName(FName InName, EIncludeSuperFlag::Type IncludeSuper) const
{
	UFunction* (*FindFunctionByNameInternal)(const UClass*, FName, EIncludeSuperFlag::Type) = decltype(FindFunctionByNameInternal)(ImageBase + Finder::FindUClass_FindFunctionByName());
	return FindFunctionByNameInternal(this, InName, IncludeSuper);
}

UFunction* UClass::GetFunction(const std::string& ClassName, const std::string& FuncName) const
{
	for (const UStruct* Clss = this; Clss; Clss = Clss->SuperStruct)
	{
		if (Clss->GetFName().ToString().ToString() != ClassName)
			continue;

		for (UField* Field = Clss->Children; Field; Field = Field->Next)
		{
			if (Field->IsA(EClassCastFlags::CASTCLASS_UFunction) && Field->GetFName().ToString().ToString() == FuncName)
				return static_cast<class UFunction*>(Field);
		}
	}

	return nullptr;
}

UFunction* UClass::GetFunction(const std::string& FullName, bool bSilent) const
{
	UObject* Function = FUObjectArray::FindObject(FullName);
	if (!Function) {
		if (!bSilent) {
			Log("Failed to find function with full name: " + FullName);
		}
		return nullptr;
	}
	return (UFunction*)Function;
}