#include "pch.h"
#include "FortniteGame/Public/Building/BuildingFoundation.h"

#include "FortniteGame/Public/FortGameState.h"
#include "FortniteGame/Public/World/FortWorldManager.h"
#include "FortniteGame/Public/World/FortWorldRecord.h"
#include "FortniteGame/Public/World/FortZoneRecord.h"

void ABuildingFoundation::OnRep_ServerStreamedInLevel()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_ServerStreamedInLevel");

	if (!Func) {
		return;
	}

	Call(Func);
}

bool ABuildingFoundation::SelectAndSetupMyBuildingLevel(void* ReservedRandomValues)
{
	uintptr_t Addr = Finder::FindABuildingFoundation_SelectAndSetupMyBuildingLevel();
	if (!Addr) {
		return false;
	}

	bool (*SelectAndSetupMyBuildingLevelInternal)(ABuildingFoundation*, void*) = decltype(SelectAndSetupMyBuildingLevelInternal)(ImageBase + Addr);
	return SelectAndSetupMyBuildingLevelInternal(this, ReservedRandomValues);
}

bool ABuildingFoundation::StreamInMyBuilding(bool bOnlyAdd) {
	uintptr_t Addr = Finder::FindABuildingFoundation_StreamInMyBuilding();
	if (!Addr) {
		return false;
	}

	bool (*SelectAndSetupMyBuildingLevelInternal)(ABuildingFoundation*, bool) = decltype(SelectAndSetupMyBuildingLevelInternal)(ImageBase + Addr);
	return SelectAndSetupMyBuildingLevelInternal(this, bOnlyAdd);
}

void ABuildingFoundation::SetDynamicFoundationEnabled(bool bEnabled)
{
	auto OldEnabled = bFoundationEnabled;
	bFoundationEnabled = bEnabled;
	OnRep_FoundationEnabled(OldEnabled);

	DynamicFoundationRepData.EnabledState = bEnabled ? EDynamicFoundationEnabledState::GetEnabled() : EDynamicFoundationEnabledState::GetDisabled();
	OnRep_DynamicFoundationRepData();

	FoundationEnabledState = bEnabled ? EDynamicFoundationEnabledState::GetEnabled() : EDynamicFoundationEnabledState::GetDisabled();
	OnRep_FoundationEnabledState();

	if (!bEnabled || LevelToStream != "None") {
		return;
	}

	bServerStreamedInLevel = true;
	OnRep_ServerStreamedInLevel();

	AFortGameState* GameState = GetWorld() ? GetWorld()->GameState->Cast<AFortGameState>() : nullptr;
	if (GameState && GameState->_HasWorldManager())
	{
		AFortWorldManager* WorldManager = GameState->WorldManager;
		if (!WorldManager)
			return;

		if (WorldManager->_HasCurrentWorldRecord() && !WorldManager->CurrentWorldRecord)
			return;

		if (WorldManager->_HasCurrentZoneRecord() && !WorldManager->CurrentZoneRecord)
			return;
	}

	if (SelectAndSetupMyBuildingLevel(nullptr)) {
		StreamInMyBuilding(false);
	}
}

void ABuildingFoundation::execSetDynamicFoundationEnabled(ABuildingFoundation* Context, FFrame& Stack) {
	bool bEnabled;
	Stack.StepCompiledIn(&bEnabled);
	Stack.IncrementCode();

	Context->SetDynamicFoundationEnabled(bEnabled);
}

void ABuildingFoundation::SetDynamicFoundationTransform(FTransform& NewTransform) {
	DynamicFoundationTransform = NewTransform;
	OnRep_DynamicFoundationTransform();
}

void ABuildingFoundation::execSetDynamicFoundationTransform(ABuildingFoundation* Context, FFrame& Stack) {
	FTransform& NewTransform = Stack.StepCompiledInRef<FTransform>();
	Stack.IncrementCode();

	Context->SetDynamicFoundationTransform(NewTransform);
}

void ABuildingFoundation::EditorOnlyAddAdditionalWorld(FString& LevelPackageName)
{
	if (LevelPackageName.IsEmpty() || !LevelPackageName.IsValid()) {
		Log("ABuildingFoundation::EditorOnlyAddAdditionalWorld: LevelPackageName is empty!");
		return;
	}

	std::string PackageName = LevelPackageName.ToString();
	std::string ShortName = PackageName.substr(PackageName.find_last_of('/') + 1);
	std::string ObjectPath = PackageName + "." + ShortName;

	std::wstring WideObjectPath(ObjectPath.begin(), ObjectPath.end());
	FName AssetPathName = UKismetStringLibrary::Conv_StringToName(WideObjectPath.c_str());

	for (int i = 0; i < AdditionalWorlds.Num(); i++)
	{
		FName& ExistingPathName = AdditionalWorlds[i].ObjectID.AssetPathName;
		if (ExistingPathName.ComparisonIndex == AssetPathName.ComparisonIndex && ExistingPathName.Number == AssetPathName.Number) {
			return;
		}
	}

	TSoftObjectPtr<UWorld> World;
	World.WeakPtr.Reset();
	World.TagAtLastTest = 0;
	World.ObjectID.AssetPathName = AssetPathName;

	AdditionalWorlds.Add(World);
	Log("ABuildingFoundation::EditorOnlyAddAdditionalWorld: Added " + ObjectPath + " to " + GetName().ToString());
}

void ABuildingFoundation::execEditorOnlyAddAdditionalWorld(ABuildingFoundation* Context, FFrame& Stack) {
	FString& LevelPackageName = Stack.StepCompiledInRef<FString>();
	Stack.IncrementCode();

	Context->EditorOnlyAddAdditionalWorld(LevelPackageName);
}

void ABuildingFoundation::EditorOnlyClearAdditionalWorlds()
{
	if (!_HasAdditionalWorlds()) {
		return;
	}

	AdditionalWorlds.Free();
	Log("ABuildingFoundation::EditorOnlyClearAdditionalWorlds: Cleared AdditionalWorlds on " + GetName().ToString());
}

void ABuildingFoundation::execEditorOnlyClearAdditionalWorlds(ABuildingFoundation* Context, FFrame& Stack) {
	Stack.IncrementCode();

	Context->EditorOnlyClearAdditionalWorlds();
}

TArray<FString> ABuildingFoundation::EditorOnlyGetAdditionalWorlds()
{
	TArray<FString> Result;

	if (!_HasAdditionalWorlds()) {
		return Result;
	}

	for (auto& World : AdditionalWorlds)
	{
		std::string ObjectPath = World.ObjectID.AssetPathName.ToString().ToString();
		if (ObjectPath == "None") {
			continue;
		}

		std::string PackageName = ObjectPath.substr(0, ObjectPath.find('.'));
		std::wstring WidePackageName(PackageName.begin(), PackageName.end());

		Result.Add(FString(WidePackageName.c_str()));
	}

	return Result;
}

void ABuildingFoundation::execEditorOnlyGetAdditionalWorlds(ABuildingFoundation* Context, FFrame& Stack, TArray<FString>* Result) {
	Stack.IncrementCode();

	*Result = Context->EditorOnlyGetAdditionalWorlds();
}

bool ABuildingFoundation::EditorOnlyRemoveAdditionalWorld(FString& LevelToRemove)
{
	if (LevelToRemove.IsEmpty() || !LevelToRemove.IsValid() || !_HasAdditionalWorlds()) {
		return false;
	}

	std::string PackageName = LevelToRemove.ToString();

	bool bRemoved = false;
	for (int i = AdditionalWorlds.Num() - 1; i >= 0; i--)
	{
		std::string ObjectPath = AdditionalWorlds[i].ObjectID.AssetPathName.ToString().ToString();

		if (ObjectPath.substr(0, ObjectPath.find('.')) != PackageName) {
			continue;
		}

		AdditionalWorlds.RemoveAt(i);
		bRemoved = true;
	}

	if (bRemoved) {
		Log("ABuildingFoundation::EditorOnlyRemoveAdditionalWorld: Removed " + PackageName + " from " + GetName().ToString());
	}

	return bRemoved;
}

void ABuildingFoundation::execEditorOnlyRemoveAdditionalWorld(ABuildingFoundation* Context, FFrame& Stack, bool* Result) {
	FString& LevelToRemove = Stack.StepCompiledInRef<FString>();
	Stack.IncrementCode();

	*Result = Context->EditorOnlyRemoveAdditionalWorld(LevelToRemove);
}

bool ABuildingFoundation::EditorOnlyAddTransformToPreviewLevel(FTransform& DeltaTransform)
{
	if (!_HasDynamicFoundationTransform()) {
		return false;
	}

	FTransform NewTransform = DynamicFoundationTransform;
	NewTransform.Translation = NewTransform.Translation + DeltaTransform.Translation;
	NewTransform.Rotation = DeltaTransform.Rotation * NewTransform.Rotation;

	SetDynamicFoundationTransform(NewTransform);
	Log("ABuildingFoundation::EditorOnlyAddTransformToPreviewLevel: Moved " + GetName().ToString());
	return true;
}

void ABuildingFoundation::execEditorOnlyAddTransformToPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result) {
	FTransform& DeltaTransform = Stack.StepCompiledInRef<FTransform>();
	Stack.IncrementCode();

	*Result = Context->EditorOnlyAddTransformToPreviewLevel(DeltaTransform);
}

bool ABuildingFoundation::EditorOnlyLoadPreviewLevel(bool bCreateLevelInstance)
{
	if (!SelectAndSetupMyBuildingLevel()) {
		Log("ABuildingFoundation::EditorOnlyLoadPreviewLevel: SelectAndSetupMyBuildingLevel failed on " + GetName().ToString());
		return false;
	}

	bool bStreamed = StreamInMyBuilding(false);
	Log("ABuildingFoundation::EditorOnlyLoadPreviewLevel: StreamInMyBuilding returned " + std::string(bStreamed ? "true" : "false") + " on " + GetName().ToString());
	return bStreamed;
}

void ABuildingFoundation::execEditorOnlyLoadPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result) {
	bool bCreateLevelInstance = false;
	Stack.StepCompiledIn(&bCreateLevelInstance);
	Stack.IncrementCode();

	*Result = Context->EditorOnlyLoadPreviewLevel(bCreateLevelInstance);
}

bool ABuildingFoundation::EditorOnlyUnloadPreviewLevel()
{
	Log("ABuildingFoundation::EditorOnlyUnloadPreviewLevel: Called on " + GetName().ToString() + ", not supported.");
	return false;
}

void ABuildingFoundation::execEditorOnlyUnloadPreviewLevel(ABuildingFoundation* Context, FFrame& Stack, bool* Result) {
	Stack.IncrementCode();

	*Result = Context->EditorOnlyUnloadPreviewLevel();
}

void ABuildingFoundation::execEditorOnlyEnterBatchPreview(UObject* Context, FFrame& Stack) {
	Stack.IncrementCode();

	Log("ABuildingFoundation::EditorOnlyEnterBatchPreview called.");
}

void ABuildingFoundation::execEditorOnlyExitBatchPreview(UObject* Context, FFrame& Stack) {
	Stack.IncrementCode();

	Log("ABuildingFoundation::EditorOnlyExitBatchPreview called.");
}

void ABuildingFoundation::SetupFoundations()
{
	std::vector<const char*> FoundationPaths;

	if (Version::Fortnite_Version >= 6 && Version::Fortnite_Version < 7)
	{
		FoundationPaths.push_back(Version::Fortnite_Version <= 6.10
			? "/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_StreamingTest12"
			: (Version::Fortnite_Version <= 6.21
				? "/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Lake1"
				: "/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Lake2"));

		FoundationPaths.push_back(Version::Fortnite_Version <= 6.10
			? "/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_Athena_StreamingTest13"
			: "/Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.LF_FloatingIsland");
	}

	for (const char* Path : FoundationPaths)
	{
		ABuildingFoundation* Foundation = StaticLoadObject<ABuildingFoundation>(Path);
		if (!Foundation) {
			Log("ABuildingFoundation::SetupFoundations: not found " + std::string(Path));
			continue;
		}

		Foundation->SetDynamicFoundationEnabled(true);

		Log("Enabled foundation " + Foundation->GetName().ToString());
	}

	SetupIslandScripting();
}

void ABuildingFoundation::DumpFoundations()
{
	for (UObject* Object : FUObjectArray::FindObjects("LF_"))
	{
		ABuildingFoundation* Foundation = ::Cast<ABuildingFoundation>(Object);
		if (!Foundation) {
			continue;
		}

		std::string Worlds;
		if (Foundation->_HasAdditionalWorlds()) {
			for (auto& World : Foundation->AdditionalWorlds) {
				std::string Path = World.ObjectID.AssetPathName.ToString().ToString();
				if (Path != "None") {
					Worlds += " " + Path;
				}
			}
		}

		Log("Foundation " + Object->GetName().ToString()
			+ " LevelToStream=" + (Foundation->_HasLevelToStream() ? Foundation->LevelToStream.ToString().ToString() : "<none>")
			+ " AdditionalWorlds=" + (Worlds.empty() ? "<none>" : Worlds));
	}
}

void ABuildingFoundation::SetupIslandScripting()
{
	UClass* IslandScriptingClass = (UClass*)FUObjectArray::FindObjectFast("BP_IslandScripting_C");
	if (!IslandScriptingClass) {
		return;
	}

	UProperty* UpdateMap = IslandScriptingClass->FindPropertyByName("UpdateMap");
	UProperty* CachedTimeProp = IslandScriptingClass->FindPropertyByName("CachedTime");
	UProperty* IslandPositionProp = IslandScriptingClass->FindPropertyByName("IslandPosition");

	for (UObject* IslandScripting : FUObjectArray::GetObjectsOfClass(IslandScriptingClass))
	{
		if (!IslandScripting || IslandScripting == IslandScriptingClass->GetDefaultObject()) {
			continue;
		}

		if (UpdateMap) {
			*(bool*)((uintptr_t)IslandScripting + UpdateMap->Offset_Internal) = true;

			if (UFunction* OnRep = IslandScripting->FindFunction("OnRep_UpdateMap")) {
				IslandScripting->Call(OnRep);
			}
		}
		else if (CachedTimeProp) {
			if (UFunction* OnRep = IslandScripting->FindFunction("OnRep_CachedTime")) {
				IslandScripting->Call(OnRep);
			}

			if (UFunction* OnRep = IslandScripting->FindFunction("OnRep_IslandPosition")) {
				IslandScripting->Call(OnRep);
			}
		}
	}
}

void ABuildingFoundation::OnRep_FoundationEnabled(bool bOldEnabled) {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_FoundationEnabled");

	if (!Func) {
		return;
	}

	Call(Func, bOldEnabled);
}

void ABuildingFoundation::OnRep_DynamicFoundationRepData() {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DynamicFoundationRepData");

	if (!Func) {
		return;
	}

	Call(Func);
}

void ABuildingFoundation::OnRep_FoundationEnabledState() {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_FoundationEnabledState");

	if (!Func) {
		return;
	}

	Call(Func);
}

void ABuildingFoundation::OnRep_DynamicFoundationTransform() {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DynamicFoundationTransform");

	if (!Func) {
		return;
	}

	Call(Func);
}
