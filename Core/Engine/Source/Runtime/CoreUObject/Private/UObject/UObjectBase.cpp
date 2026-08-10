#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectBase.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Class.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

DECLARE_LOG_CATEGORY_STATIC(LogUObjectBase, Log, All);

bool UObjectBase::IsValidLowLevel() const
{
	if (Finder::FindUObjectBase_IsValidLowLevel()) {
		bool (*IsValidLowLevelInternal)(const UObjectBase*) = decltype(IsValidLowLevelInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevel());
		return IsValidLowLevelInternal(this);
	}

	return true;
}

bool UObjectBase::IsValidLowLevelFast(bool bRecursive) const
{
	if (Finder::FindUObjectBase_IsValidLowLevelFast()) {
		bool (*IsValidLowLevelFastInternal)(const UObjectBase*, bool) = decltype(IsValidLowLevelFastInternal)(ImageBase + Finder::FindUObjectBase_IsValidLowLevelFast());
		return IsValidLowLevelFastInternal(this, bRecursive);
	}

	return true;
}

std::string UObjectBase::GetFullName() const
{
	if (!this)
	{
		return "None";
	}

	UClass* Class = GetClass();
	if (Class)
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
	if (!this || !ClassPrivate) {
		return false;
	}

	return (ClassPrivate->GetCastFlags() & TypeFlags);
}

bool UObjectBase::IsA(UClass* TypeClass) const
{
	if (!this) {
		return false;
	}

	if (!TypeClass) {
		return false;
	}

	if (!IsValidLowLevelFast()) {
		UE_LOG(LogUObjectBase, Warning, TEXT("IsA: Object is not valid: 0x%llX"), (uint64)(uintptr_t)this);
		return false;
	}

	if (!GetClass()) {
		UE_LOG(LogUObjectBase, Warning, TEXT("IsA: Object has no class: 0x%llX"), (uint64)(uintptr_t)this);
		return false;
	}

	return GetClass()->IsSubclassOf(TypeClass);
}

UProperty* UObjectBase::FindPropertyByName(FName InName) const
{
	return GetClass() ? GetClass()->FindPropertyByName(InName) : nullptr;
}