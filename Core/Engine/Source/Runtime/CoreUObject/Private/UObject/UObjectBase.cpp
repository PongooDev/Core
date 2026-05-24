#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"

bool UObjectBase::IsValidLowLevel() const
{
	bool (*IsValidLowLevelInternal)(const UObjectBase*) = decltype(IsValidLowLevelInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevel());
	return IsValidLowLevelInternal(this);
}

bool UObjectBase::IsValidLowLevelFast(bool bRecursive) const
{
	bool (*IsValidLowLevelFastInternal)(const UObjectBase*, bool) = decltype(IsValidLowLevelFastInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevelFast());
	return IsValidLowLevelFastInternal(this, bRecursive);
}

std::string UObjectBase::GetFullName() const
{
	UClass* Class = GetClass();
	if (this && Class)
	{
		std::string Temp;

		FName ObjectName = GetFName();
		FString ObjectFString = ObjectName.ToString();
		std::string ObjectString = ObjectFString.ToString();

		for (UObjectBase* NextOuter = OuterPrivate; NextOuter; NextOuter = NextOuter->OuterPrivate)
		{
			FName OuterFName = NextOuter->GetFName();
			FString OuterFString = OuterFName.ToString();
			std::string OuterString = OuterFString.ToString();
			Temp = OuterString + "." + Temp;
		}

		FName ClassName = Class->GetFName();
		FString ClassString = ClassName.ToString();
		std::string Name = ClassString.ToString();
		Name += " ";
		Name += Temp;
		Name += ObjectString;

		return Name;
	}

	return "None";
}

bool UObjectBase::IsA(EClassCastFlags TypeFlags) const
{
	return (ClassPrivate->GetCastFlags() & TypeFlags);
}

bool UObjectBase::IsA(UClass* TypeClass) const
{
	if (!this || !TypeClass)
		return false;
	if (!IsValidLowLevelFast())
		return false;

	if (!GetClass())
		return false;

	return GetClass()->IsSubclassOf(TypeClass);
}

UProperty* UObjectBase::FindPropertyByName(std::string InName) const
{
	return GetClass() ? GetClass()->FindPropertyByName(InName) : nullptr;
}