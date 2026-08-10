#include "pch.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Package.h"

DEFINE_LOG_CATEGORY(LogUObjectGlobals);

static UObject* GetTransientPackage()
{
	static UObject* TransientPackage = nullptr;
	if (!TransientPackage)
		TransientPackage = FUObjectArray::FindObject("Package /Engine/Transient");
	return TransientPackage;
}

void BeginLoad(FUObjectSerializeContext* LoadContext, const TCHAR* DebugContext) {
	void (*BeginLoadInternal)(FUObjectSerializeContext*, const TCHAR*) = decltype(BeginLoadInternal)(ImageBase + Finder::FindBeginLoad());
	BeginLoadInternal(LoadContext, DebugContext);
}

void EndLoad(FUObjectSerializeContext* LoadContext) {
	void (*EndLoadInternal)(FUObjectSerializeContext*) = decltype(EndLoadInternal)(ImageBase + Finder::FindEndLoad());
	EndLoadInternal(LoadContext);
}

UObject* StaticConstructObject_Internal(UClass* Class, UObject* Outer, FName Name, EObjectFlags SetFlags, UObject* Template)
{
	if (!Class)
		return nullptr;

	const uintptr_t Offset = Finder::FindStaticConstructObject_Internal();
	if (!Offset)
	{
		UE_LOG(LogUObjectGlobals, Error, TEXT("StaticConstructObject_Internal: engine function not resolved for this build; returning null"));
		return nullptr;
	}

	if (!Outer)
		Outer = GetTransientPackage();

	const uintptr_t Addr = ImageBase + Offset;

	if (Version::Engine_Version >= 4.26)
	{
		FStaticConstructObjectParameters Params(Class);
		Params.Outer = Outer;
		Params.Name = Name;
		Params.SetFlags = SetFlags;
		Params.Template = Template;

		UObject* (*Fn)(const FStaticConstructObjectParameters&) = decltype(Fn)(Addr);
		return Fn(Params);
	}
	else
	{
		UObject* (*Fn)(UClass*, UObject*, FName, EObjectFlags, EInternalObjectFlags, UObject*, bool, void*) = decltype(Fn)(Addr);
		return Fn(Class, Outer, Name, SetFlags, EInternalObjectFlags::None, Template, false, nullptr);
	}
}