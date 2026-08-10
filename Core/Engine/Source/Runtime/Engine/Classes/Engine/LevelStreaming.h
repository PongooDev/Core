#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

class ULevel;
class UWorld;

class ULevelStreaming : public UObject {
public:
	DefineUnrealClass(ULevelStreaming);

	DefineUProperty(ULevel*, LoadedLevel);
	DefineUProperty(ULevel*, PendingUnloadLevel);
	DefineUProperty(FName, PackageNameToLoad);
	DefineBitfieldUProperty(bShouldBeLoaded);
	DefineBitfieldUProperty(bShouldBeVisible);
	DefineBitfieldUProperty(bIsStatic);
	DefineBitfieldUProperty(bShouldBlockOnLoad);
public:
	FORCEINLINE ULevel* GetLoadedLevel() const { return LoadedLevel; }
	FORCEINLINE bool ShouldBeLoaded() const { return bShouldBeLoaded; }
	FORCEINLINE bool ShouldBeVisible() const { return bShouldBeVisible; }
	FORCEINLINE bool IsLevelLoaded() const { return LoadedLevel != nullptr; }
};

class ULevelStreamingKismet : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingKismet);
};

class ULevelStreamingDynamic : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingDynamic);

	static ULevelStreamingDynamic* LoadLevelInstanceBySoftObjectPtr(UObject* WorldContextObject, FSoftObjectPtr& Level, const FVector& Location, const FRotator& Rotation, bool* bOutSuccess, const FString& OptionalLevelNameOverride = FString())
	{
		static UObject* CDO = nullptr;
		static UFunction* Func = nullptr;
		static bool bSearched = false;

		if (!bSearched) {
			bSearched = true;

			if (ULevelStreamingDynamic::StaticClass()) {
				CDO = (UObject*)ULevelStreamingDynamic::GetDefaultObj();
			}
			else if (ULevelStreamingKismet::StaticClass()) {
				CDO = (UObject*)ULevelStreamingKismet::GetDefaultObj();
			}

			if (CDO) {
				Func = CDO->FindFunction("LoadLevelInstanceBySoftObjectPtr");
			}

			if (!Func) {
				UE_LOG(LogLevelStreaming, Error, TEXT("ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr: function not found"));
			}
		}

		if (!CDO || !Func) {
			return nullptr;
		}

		return CDO->Call<ULevelStreamingDynamic*>(Func, WorldContextObject, Level, Location, Rotation, bOutSuccess, OptionalLevelNameOverride);
	}
};

class ULevelStreamingAlwaysLoaded : public ULevelStreaming {
public:
	DefineUnrealClass(ULevelStreamingAlwaysLoaded);
};
