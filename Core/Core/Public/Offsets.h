#pragma once
#include "pch.h"

namespace ServerOffsets {
	inline uintptr_t ProcessEvent = 0;
	inline uintptr_t ProcessEventVFT = 0;

	inline uintptr_t GUObjectArray = 0;
	inline uintptr_t GWorld = 0;
	inline uintptr_t GEngine = 0;

	inline uintptr_t GIsClient = 0;
	inline uintptr_t GIsServer = 0;

	inline uintptr_t FMemory_Free = 0;
	inline uintptr_t FMemory_Realloc = 0;

	inline uintptr_t ChangeGameSessionId = 0;

	inline uintptr_t StaticFindObject = 0;
	inline uintptr_t StaticLoadObject = 0;

	inline uintptr_t TickFlush = 0;

	inline uintptr_t InternalServerTryActivateAbilityVFT = 0x0;

	inline uintptr_t GHandle = 0;

	inline uintptr_t AbilitySpecDefaultConstructor = 0;
	inline uintptr_t AbilitySpecClassConstructor = 0;
	inline uintptr_t AbilitySpecCDOConstructor = 0;

	inline uintptr_t ABuildingActor_OnDamageServer = 0;
	inline uintptr_t ABuildingActor_PostUpdateVFT = 0;
	inline uintptr_t ABuildingActor_ServerOnAttemptInteractVFT = 0;

	inline uintptr_t SpawnActor = 0;

	inline uintptr_t FFrame__CurrentNativeFunction = 0;
	inline uintptr_t FFrame__PropertyChainForCompiledIn = 0;
	inline uintptr_t FFrame__Next = 0;

	inline uintptr_t Step = 0;
	inline uintptr_t StepExplicitProperty = 0;

	inline uintptr_t ExecFunction = 0;

	inline uintptr_t FName_AppendString = 0;
	inline uintptr_t FName_ToStringOut = 0;
	inline uintptr_t FName_Compare = 0;
	inline uintptr_t FName_GetComparisonNameEntry = 0;
	inline uintptr_t FName_Constructor1 = 0; // const char* Str

	inline uintptr_t UObjectBase_AddObject = 0;
	inline uintptr_t UObjectBase_AtomicallyClearFlags = 0;
	inline uintptr_t UObjectBase_DeferredRegister = 0;
	inline uintptr_t UObjectBase_EmitBaseReferences = 0;
	inline uintptr_t UObjectBase_IsValidLowLevel = 0;
	inline uintptr_t UObjectBase_IsValidLowLevelFast = 0;
	inline uintptr_t UObjectBase_LowLevelRename = 0;
	inline uintptr_t UObjectBase_Register = 0;
	inline uintptr_t UObjectBase_Destructor = 0;

	inline uintptr_t UObjectBaseUtility_AddToCluster = 0;
	inline uintptr_t UObjectBaseUtility_CanBeInCluster = 0;
	inline uintptr_t UObjectBaseUtility_GetFullName = 0;
	inline uintptr_t UObjectBaseUtility_GetFullName_1 = 0;
	inline uintptr_t UObjectBaseUtility_GetInterfaceAddress = 0;
	inline uintptr_t UObjectBaseUtility_GetLinker = 0;
	inline uintptr_t UObjectBaseUtility_GetLinkerIndex = 0;
	inline uintptr_t UObjectBaseUtility_GetNativeInterfaceAddress = 0;
	inline uintptr_t UObjectBaseUtility_GetOutermost = 0;
	inline uintptr_t UObjectBaseUtility_GetPathName = 0;
	inline uintptr_t UObjectBaseUtility_GetTypedOuter = 0;
	inline uintptr_t UObjectBaseUtility_IsDefaultSubobject = 0;
	inline uintptr_t UObjectBaseUtility_IsIn = 0;
	inline uintptr_t UObjectBaseUtility_IsInA = 0;
	inline uintptr_t UObjectBaseUtility_IsTemplate = 0;
	inline uintptr_t UObjectBaseUtility_MarkPackageDirty = 0;

	inline uintptr_t UObject_BeginDestroy = 0;
	inline uintptr_t UObject_FinishDestroy = 0;
	inline uintptr_t UObject_DefaultConstructor = 0;
	inline uintptr_t UObject_GetDefaultSubobjects = 0;
	inline uintptr_t UObject_GetDefaultSubobjectByName = 0;
	inline uintptr_t UObject_PostInitProperties = 0;
	inline uintptr_t UObject_PostLoadVFT = 0;
	inline uintptr_t UObject_FindFunctionChecked = 0;
	inline uintptr_t UObject_GetWorldVFT = 0;
	inline uintptr_t UObject_NeedsLoadForClientVFT = 0;
	inline uintptr_t UObject_CanCreateInCurrentContext = 0;

	inline uintptr_t UField_GetOwnerClass = 0;
	inline uintptr_t UField_GetOwnerStruct = 0;

	inline uintptr_t UStruct_FindPropertyByName = 0;
	inline uintptr_t UStruct_IsChildOf = 0;

	inline uintptr_t UStruct__SuperStruct = 0;
	inline uintptr_t UStruct__Children = 0;
	inline uintptr_t UStruct__PropertiesSize = 0;
	inline uintptr_t UStruct__MinAlignment = 0;

	inline uintptr_t UClass_FindFunctionByName = 0;
	inline uintptr_t UClass_GetSparseClassData = 0;

	inline uintptr_t FName_GetPlainNameString1 = 0;
	inline uintptr_t FName_GetPlainNameString2 = 0;

	inline uintptr_t UProperty__Offset_Internal = 0;
	inline uintptr_t UProperty__ElementSize = 0;
	inline uintptr_t UProperty__PropertyFlags = 0;

	inline uintptr_t UField__Next = 0;
	inline uintptr_t UField__Name = 0;

	inline uintptr_t UBoolProperty__FieldMask = 0;

	inline uintptr_t UEngine_BroadcastNetworkFailure = 0;
	inline uintptr_t UEngine_BroadcastNetworkLagStateChanged = 0;
	inline uintptr_t UEngine_CancelAllPending = 0;
	inline uintptr_t UEngine_CreateGameUserSettings = 0;
	inline uintptr_t UEngine_GetFirstGamePlayer = 0;
	inline uintptr_t UEngine_GetNetMode = 0;
	inline uintptr_t UEngine_GetMaxTickRate = 0;
	inline uintptr_t UEngine_GetMaxTickRateVFT = 0;
	inline uintptr_t UEngine_CreateNetDriver = 0;
	inline uintptr_t UEngine_LoadMap = 0;
	inline uintptr_t UEngine_LoadMapVFT = 0;
	inline uintptr_t UEngine_CreateNamedNetDriver = 0;
	inline uintptr_t UEngine_GetWorldContextFromWorldChecked = 0;
	inline uintptr_t UEngine_FindNamedNetDriver = 0;
	inline uintptr_t UEngine_DestroyNamedNetDriver = 0;
	inline uintptr_t UEngine_GetWorldContextFromWorld = 0;
	inline uintptr_t UEngine_CreateNewWorldContext = 0;
	inline uintptr_t UEngine_CreateNetDriver_Local = 0;

	inline uintptr_t UWorld_AddController = 0;
	inline uintptr_t UWorld_AddLevel = 0;
	inline uintptr_t UWorld_AddNetworkActor = 0;
	inline uintptr_t UWorld_AddStreamingLevel = 0;
	inline uintptr_t UWorld_AddToWorld = 0;
	inline uintptr_t UWorld_AllowLevelLoadRequests = 0;
	inline uintptr_t UWorld_AreActorsInitialized = 0;
	inline uintptr_t UWorld_AreAlwaysLoadedLevelsLoaded = 0;
	inline uintptr_t UWorld_BeginPlay = 0;
	inline uintptr_t UWorld_CommitMapChange = 0;
	inline uintptr_t UWorld_ConditionallyCreateDefaultLevelCollections = 0;
	inline uintptr_t UWorld_ContainsActor = 0;
	inline uintptr_t UWorld_ContainsLevel = 0;
	inline uintptr_t UWorld_CreateAISystem = 0;
	inline uintptr_t UWorld_GetActiveLevelCollection = 0;
	inline uintptr_t UWorld_GetActorCount = 0;
	inline uintptr_t UWorld_GetAddressURL = 0;
	inline uintptr_t UWorld_GetDefaultGravityZ = 0;
	inline uintptr_t UWorld_GetFirstLocalPlayerFromController = 0;
	inline uintptr_t UWorld_GetFirstPlayerController = 0;
	inline uintptr_t UWorld_GetGameViewport = 0;
	inline uintptr_t UWorld_GetGravityZ = 0;
	inline uintptr_t UWorld_GetMapName = 0;
	inline uintptr_t UWorld_GetTimerManager = 0;
	inline uintptr_t UWorld_GetWorldSettings = 0;
	inline uintptr_t UWorld_K2_GetWorldSettings = 0;
	inline uintptr_t UWorld_HasBegunPlay = 0;
	inline uintptr_t UWorld_InternalGetNetMode = 0;
	inline uintptr_t UWorld_IsGameWorld = 0;
	inline uintptr_t UWorld_IsInSeamlessTravel = 0;
	inline uintptr_t UWorld_IsServer = 0;
	inline uintptr_t UWorld_MarkObjectsPendingKill = 0;
	inline uintptr_t UWorld_RemoveActor = 0;
	inline uintptr_t UWorld_RemoveController = 0;
	inline uintptr_t UWorld_RemoveNetworkActor = 0;
	inline uintptr_t UWorld_SeamlessTravel = 0;
	inline uintptr_t UWorld_ServerTravel = 0;
	inline uintptr_t UWorld_SetGameMode = 0;
	inline uintptr_t UWorld_SetGameState = 0;
	inline uintptr_t UWorld_SetNavigationSystem = 0;
	inline uintptr_t UWorld_WelcomePlayer = 0;
	inline uintptr_t UWorld_UpdateStreamingLevelShouldBeConsidered = 0;
	inline uintptr_t UWorld_FindCollectionByType = 0;
	inline uintptr_t UWorld__NextSwitchCountdown = 0;
	inline uintptr_t UWorld_ListenPatch = 0;
	inline uintptr_t UWorld_RemovePIEPrefix = 0;

	inline uintptr_t FGuid_ToString = 0;

	inline uintptr_t UGameInstance_StartGameInstance = 0;
	inline uintptr_t UGameInstance_Shutdown = 0;
	inline uintptr_t UGameInstance_GetFirstLocalPlayerController = 0;
	inline uintptr_t UGameInstance_ClientTravelToSession = 0;
	inline uintptr_t UGameInstance_CleanupGameViewport = 0;

	inline uintptr_t AActor_ActorHasTag = 0;
	inline uintptr_t AActor_CallPreReplication = 0;
	inline uintptr_t AActor_CheckStillInWorld = 0;
	inline uintptr_t AActor_ClearCrossLevelReferences = 0;
	inline uintptr_t AActor_CopyRemoteRoleFrom = 0;
	inline uintptr_t AActor_Destroy = 0;
	inline uintptr_t AActor_DisableInput = 0;
	inline uintptr_t AActor_EnableInput = 0;
	inline uintptr_t AActor_ExchangeNetRoles = 0;
	inline uintptr_t AActor_FindOrAddNetworkObjectInfo = 0;
	inline uintptr_t AActor_InternalGetNetMode = 0;
	inline uintptr_t AActor_IsPendingKillPending = 0;
	inline uintptr_t AActor_HasNetOwner = 0;
	inline uintptr_t AActor_HasLocalNetOwner = 0;
	inline uintptr_t AActor_FinishSpawning = 0;
	inline uintptr_t AActor_SetNetDormancy = 0;
	inline uintptr_t AActor_BeginPlay = 0;
	inline uintptr_t AActor_BeginPlayVFT = 0;
	inline uintptr_t AActor_ResetVFT = 0;

	inline uintptr_t AGameSession_ApproveLogin = 0;
	inline uintptr_t AGameSession_AtCapacity = 0;
	inline uintptr_t AGameSession_GetSessionJoinability = 0;
	inline uintptr_t AGameSession_KickPlayer = 0;
	inline uintptr_t AGameSession_KickPlayerVFT = 0;
	inline uintptr_t AGameSession_ReturnToMainMenuHost = 0;
	inline uintptr_t AGameSession_UnregisterPlayer = 0;
	inline uintptr_t AGameSession_UpdateSessionJoinability = 0;
	inline uintptr_t AGameSession_InitOptions = 0;
	inline uintptr_t AGameSession_RegisterPlayer = 0;
	inline uintptr_t AGameSession__NextPlayerID = 0;
	inline uintptr_t AGameSession_ProcessAutoLogin = 0;
	inline uintptr_t AGameSession_OnAutoLoginComplete = 0;
	inline uintptr_t AGameSession_UnregisterPlayer_1 = 0;
	inline uintptr_t AGameSession_UnregisterPlayers = 0;
	inline uintptr_t AGameSession_GetPlayerControllerFromNetId = 0;

	inline uintptr_t AGameModeBase_InitGame = 0;
	inline uintptr_t AGameModeBase_InitGameVFT = 0;
	inline uintptr_t AGameModeBase_InitGameState = 0;
	inline uintptr_t AGameModeBase_InitGameStateVFT = 0;
	inline uintptr_t AGameModeBase_CanServerTravel = 0;
	inline uintptr_t AGameModeBase_SpawnDefaultPawnFor = 0;
	inline uintptr_t AGameModeBase_SpawnDefaultPawnAtTransform = 0;
	inline uintptr_t AGameModeBase_RestartPlayer = 0;
	inline uintptr_t AGameModeBase_FinishRestartPlayer = 0;
	inline uintptr_t AGameModeBase_FinishRestartPlayerVFT = 0;
	inline uintptr_t AGameModeBase_HandleStartingNewPlayer = 0;
	inline uintptr_t AGameModeBase_SpawnPlayerController = 0;
	inline uintptr_t AGameModeBase_SpawnPlayerControllerVFT = 0;
	inline uintptr_t AGameModeBase_GetGameSessionClass = 0;
	inline uintptr_t AGameModeBase_GetGameSessionClassVFT = 0;
	inline uintptr_t AGameModeBase_ProcessServerTravelVFT = 0;

	inline uintptr_t UKismetStringLibrary_Conv_StringToName = 0;

	inline uintptr_t AGameMode_AddInactivePlayer = 0;
	inline uintptr_t AGameMode_AddInactivePlayerVFT = 0;
	inline uintptr_t AGameMode_Broadcast = 0;
	inline uintptr_t AGameMode_BroadcastLocalized = 0;
	inline uintptr_t AGameMode_EndMatch = 0;
	inline uintptr_t AGameMode_FindInactivePlayer = 0;
	inline uintptr_t AGameMode_GetNetworkNumber = 0;
	inline uintptr_t AGameMode_HandleMatchHasEnded = 0;
	inline uintptr_t AGameMode_HandleMatchHasStarted = 0;
	inline uintptr_t AGameMode_HandleMatchIsWaitingToStart = 0;
	inline uintptr_t AGameMode_HandleSeamlessTravelPlayer = 0;
	inline uintptr_t AGameMode_HandleStartingNewPlayer = 0;
	inline uintptr_t AGameMode_InitSeamlessTravelPlayer = 0;
	inline uintptr_t AGameMode_IsHandlingReplays = 0;
	inline uintptr_t AGameMode_OnMatchStateSet = 0;
	inline uintptr_t AGameMode_PlayerCanRestart = 0;
	inline uintptr_t AGameMode_PostSeamlessTravel = 0;
	inline uintptr_t AGameMode_ReadyToStartMatch = 0;
	inline uintptr_t AGameMode_RestartGame = 0;
	inline uintptr_t AGameMode_Say = 0;
	inline uintptr_t AGameMode_SendPlayer = 0;
	inline uintptr_t AGameMode_SetMatchState = 0;
	inline uintptr_t AGameMode_SetSeamlessTravelViewTarget = 0;
	inline uintptr_t AGameMode_StartMatch = 0;
	inline uintptr_t AGameMode_StartPlay = 0;
	inline uintptr_t AGameMode_OverridePlayerState = 0;
	inline uintptr_t AGameMode_PostLogin = 0;
	inline uintptr_t AGameMode_Logout = 0;
	inline uintptr_t AGameMode_CanServerTravel = 0;
	inline uintptr_t AGameMode_InitGame = 0;

	inline uintptr_t APlayerController_HasClientLoadedCurrentWorld = 0;
	inline uintptr_t APlayerController_ServerAcknowledgePossession = 0;

	inline uintptr_t UGameMapsSettings_GetGameModeForName = 0;

	inline uintptr_t ULevelStreaming_PostLoad = 0;
	inline uintptr_t ULevelStreaming_DetermineTargetState = 0;
	inline uintptr_t ULevelStreaming_OnLevelAdded = 0;
	inline uintptr_t ULevelStreaming_IsDesiredLevelLoaded = 0;
	inline uintptr_t ULevelStreaming_SetLevelLODIndex = 0;
	inline uintptr_t ULevelStreaming_SetPriority = 0;
	inline uintptr_t ULevelStreaming_SetWorldAsset = 0;
	inline uintptr_t ULevelStreaming_GetWorldAssetPackageName = 0;
	inline uintptr_t ULevelStreaming_GetWorldAssetPackageFName = 0;
	inline uintptr_t ULevelStreaming_RenameForPIE = 0;
	inline uintptr_t ULevelStreaming_ShouldBeVisible = 0;
	inline uintptr_t ULevelStreaming_IsStreamingStatePending = 0;
	inline uintptr_t ULevelStreaming_CreateInstance = 0;
	inline uintptr_t ULevelStreaming_RequestLevel = 0;
	inline uintptr_t ULevelStreaming_SetLoadedLevel = 0;
	inline uintptr_t ULevelStreaming_DiscardPendingUnloadLevel = 0;

	inline uintptr_t ULevel_SortActorList = 0;
	inline uintptr_t ULevel_InitializeNetworkActors = 0;
	inline uintptr_t ULevel_ClearActorsSeamlessTraveledFlag = 0;
	inline uintptr_t ULevel_SetWorldSettings = 0;
	inline uintptr_t ULevel_IsNetActor = 0;
	inline uintptr_t ULevel_PostInitProperties = 0;
	inline uintptr_t ULevel_BeginDestroy = 0;
	inline uintptr_t ULevel_FinishDestroy = 0;
	inline uintptr_t ULevel_PostLoad = 0;
	inline uintptr_t ULevel_CreateCluster = 0;
	inline uintptr_t ULevel_ClearLevelComponents = 0;
	inline uintptr_t ULevel_UpdateLevelComponents = 0;
	inline uintptr_t ULevel_IncrementalUpdateComponents = 0;
	inline uintptr_t ULevel_IncrementalUnregisterComponents = 0;
	inline uintptr_t ULevel_RouteActorInitialize = 0;
	inline uintptr_t ULevel_ApplyWorldOffset = 0;
	inline uintptr_t ULevel_RegisterActorForAutoReceiveInput = 0;
	inline uintptr_t ULevel_PushPendingAutoReceiveInput = 0;

	inline uintptr_t FURL_IsInternal = 0;
	inline uintptr_t FURL_IsLocalInternal = 0;
	inline uintptr_t FURL_HasOption = 0;
	inline uintptr_t FURL_GetOption = 0;
	inline uintptr_t FURL_AddOption = 0;
	inline uintptr_t FURL_RemoveOption = 0;
	inline uintptr_t FURL_ToString = 0;
	inline uintptr_t FURL_GetHostPortString = 0;

	inline uintptr_t FWorldContext_SetCurrentWorld = 0;
	inline uintptr_t FWorldContext__ThisCurrentWorld = 0;

	inline uintptr_t UNetDriver_InitListen = 0;
	inline uintptr_t UNetDriver_InitListenVFT = 0;
	inline uintptr_t UNetDriver_SetWorld = 0;
	inline uintptr_t UNetDriver_SetWorldVFT = 0;
	inline uintptr_t UNetDriver_TickFlush = 0;
	inline uintptr_t UNetDriver_IsServerVFT = 0;
	inline uintptr_t UNetDriver_IsLevelInitializedForActorVFT = 0;

	inline uintptr_t FParse_Param = 0;

	inline uintptr_t AFortGameMode_OnInitWorldRecordComplete = 0;
	inline uintptr_t AFortGameMode_OnEndOfDay = 0;
	inline uintptr_t AFortGameMode_SetCurrentPlaylistName = 0;
	inline uintptr_t AFortGameMode_InitGame = 0;
	inline uintptr_t AFortGameMode_RestartPlayer = 0;
	inline uintptr_t AFortGameMode_FinishRestartPlayer = 0;
	inline uintptr_t AFortGameMode_SpawnDefaultPawnFor = 0;
	inline uintptr_t AFortGameMode_SetCurrentPlaylistId = 0;
	inline uintptr_t AFortGameMode_FinishWorldInitializationVFT = 0;
	inline uintptr_t AFortGameMode_PickTeam = 0;
	inline uintptr_t AFortGameMode_PickTeamVFT = 0;
	inline uintptr_t AFortGameMode_ProcessServerTravelPatch1 = 0;
	inline uintptr_t AFortGameMode_InitializeTeams = 0;
	inline uintptr_t AFortGameMode_InitializeTeamsVFT = 0;
	inline uintptr_t AFortGameMode_PlacePlayerOnTeamVFT = 0;

	inline uintptr_t AFortWorldManager_HandleWorldQueryComplete = 0;

	inline uintptr_t AFortGameModeZone_CheckForAllPlayersIn = 0;
	inline uintptr_t AFortGameModeZone_RemoveKnownPlayer = 0;
	inline uintptr_t AFortGameModeZone_UpdateSessionForMatchmaking = 0;
	inline uintptr_t AFortGameModeZone_NotifyPendingConnectionLost = 0;
	inline uintptr_t AFortGameModeZone_OnZoneEnd = 0;
	inline uintptr_t AFortGameModeZone_DoLogoutAnalytics = 0;
	inline uintptr_t AFortGameModeZone_HandleMatchHasStarted = 0;
	inline uintptr_t AFortGameModeZone_HandleMatchHasEnded = 0;
	inline uintptr_t AFortGameModeZone_DoInitialRestartAnalytics = 0;
	inline uintptr_t AFortGameModeZone_InitGame = 0;
	inline uintptr_t AFortGameModeZone_InitGameState = 0;
	inline uintptr_t AFortGameModeZone_ChoosePlayerStart = 0;
	inline uintptr_t AFortGameModeZone_CreateAIDirector = 0;
	inline uintptr_t AFortGameModeZone_CreateAIDirectorVFT = 0;
	inline uintptr_t AFortGameModeZone_CreateAIGoalManager = 0;
	inline uintptr_t AFortGameModeZone_CreateAIGoalManagerVFT = 0;

	inline uintptr_t AFortGameModeAthena_SetCurrentPlaylistId = 0;
	inline uintptr_t AFortGameModeAthena_SetTimeOfDayManagerGameplayOverride = 0;
	inline uintptr_t AFortGameModeAthena_OnPlaylistDataLoaded = 0;
	inline uintptr_t AFortGameModeAthena_ApplyPlaylistDataTables = 0;
	inline uintptr_t AFortGameModeAthena_OnDedicatedServerRestarting = 0;
	inline uintptr_t AFortGameModeAthena_SpawnInitialSafeZone = 0;
	inline uintptr_t AFortGameModeAthena_AttemptChangeTeam = 0;
	inline uintptr_t AFortGameModeAthena_PickTeam = 0;
	inline uintptr_t AFortGameModeAthena_HandleMatchHasStarted = 0;
	inline uintptr_t AFortGameModeAthena_FinishRestartPlayer = 0;
	inline uintptr_t AFortGameModeAthena_StartMatchAbandonProcess = 0;
	inline uintptr_t AFortGameModeAthena_OnMatchAbandonTimeout = 0;
	inline uintptr_t AFortGameModeAthena_CheckWinConditionLastManStanding = 0;
	inline uintptr_t AFortGameModeAthena_GrantVictoryToAllRemainingTeams = 0;
	inline uintptr_t AFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame = 0;
	inline uintptr_t AFortGameModeAthena_GrantLossToAllRemainingTeams = 0;
	inline uintptr_t AFortGameModeAthena_GrantLossToAllTeams = 0;
	inline uintptr_t AFortGameModeAthena_GrantLosses = 0;
	inline uintptr_t AFortGameModeAthena_ChoosePlayerStart_Creative = 0;
	inline uintptr_t AFortGameModeAthena_StartWarmupPhase = 0;
	inline uintptr_t AFortGameModeAthena_UpdateWarmupPhase = 0;
	inline uintptr_t AFortGameModeAthena_StartAircraftPhase = 0;
	inline uintptr_t AFortGameModeAthena_StartSafeZonesPhase = 0;
	inline uintptr_t AFortGameModeAthena_IsSafeZoneAllowed = 0;
	inline uintptr_t AFortGameModeAthena_StartEndGamePhase = 0;
	inline uintptr_t AFortGameModeAthena_StartEndGamePhaseTeam = 0;
	inline uintptr_t AFortGameModeAthena_CreateAIDirector = 0;
	inline uintptr_t AFortGameModeAthena_PostReloadConfig = 0;
	inline uintptr_t AFortGameModeAthena_ReadyToStartMatch = 0;
	inline uintptr_t AFortGameModeAthena_InitGame = 0;
	inline uintptr_t AFortGameModeAthena_InitGameState = 0;
	inline uintptr_t AFortGameModeAthena_SpawnDefaultPawnFor = 0;
	inline uintptr_t AFortGameModeAthena_RestartPlayer = 0;
	inline uintptr_t AFortGameModeAthena_FinishWorldInitialization = 0;
	inline uintptr_t AFortGameModeAthena_AddToAlivePlayers = 0;
	inline uintptr_t AFortGameModeAthena_RemoveFromAlivePlayers = 0;
	inline uintptr_t AFortGameModeAthena_PlacePlayerOnTeam = 0;

	inline uintptr_t AFortGameStateAthena_OnRep_CurrentPlaylistInfo = 0;
	inline uintptr_t AFortGameStateAthena_OnPlaylistDataLoadCompleted = 0;
	inline uintptr_t AFortGameStateAthena_UpdatePlaylistDependentData = 0;
	inline uintptr_t AFortGameStateAthena_SetCurrentPlaylistId = 0;

	inline uintptr_t UFortPlaylistManager_InitializePlaylists = 0;

	inline uintptr_t UReplicationDriver_ServerReplicateActors = 0;
	inline uintptr_t UReplicationDriver_ServerReplicateActorsVFT = 0;

	inline uintptr_t UAbilitySystemComponent_ClientActivateAbilityFailed = 0;
	inline uintptr_t UAbilitySystemComponent_InternalTryActivateAbility = 0;
	inline uintptr_t UAbilitySystemComponent_GiveAbility = 0;
	inline uintptr_t UAbilitySystemComponent_GiveAbilityAndActivateOnce = 0;
	inline uintptr_t UAbilitySystemComponent_FindAbilitySpecFromHandle = 0;

	inline uintptr_t ULevelStreamingDynamic_LoadLevelInstance = 0;
	inline uintptr_t ULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr = 0;

	inline uintptr_t UGameplayStatics_BeginDeferredActorSpawnFromClass = 0;

	inline uintptr_t UFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid = 0;
	inline uintptr_t UFortKismetLibrary_GetWeaponStatsRow = 0;

	inline uintptr_t FRotator_Quaternion = 0;

	inline uintptr_t ABuildingSMActor_ReplaceBuildingActor = 0;
	inline uintptr_t ABuildingSMActor_ReplaceBuildingActorVFT = 0;
	inline uintptr_t ABuildingSMActor_PostUpdate = 0;
	inline uintptr_t ABuildingSMActor_SetEditingPlayer = 0;
	inline uintptr_t ABuildingSMActor_SetEditingPlayerVFT = 0;
	inline uintptr_t ABuildingSMActor_PostLoad = 0;

	inline uintptr_t AFortPickup_FinishedTargetSpline = 0;
	inline uintptr_t AFortPickup_ForceFinishedTargetSpline = 0;
	inline uintptr_t AFortPickup_SetPickupTarget = 0;
	inline uintptr_t AFortPickup_UpdateSpecialActorStat = 0;
	inline uintptr_t AFortPickup_GivePickupTo = 0;
	inline uintptr_t AFortPickup_GivePickupToVFT = 0;
	inline uintptr_t AFortPickup_SetupForMovementCompToss = 0;
	inline uintptr_t AFortPickup_SetPickupItems = 0;
	inline uintptr_t AFortPickup_SetPickupItemsVFT = 0;

	inline uintptr_t AFortPlayerPawn_ForceFinishIncomingPickups = 0;
	inline uintptr_t AFortPlayerPawn_ForceFinishIncomingPickupsVFT = 0;

	inline uintptr_t AFortQuickBars_GetFocusedQuickBar = 0;

	inline uintptr_t ABuildingContainer_SpawnLootVFT = 0;
	inline uintptr_t ABuildingContainer_ServerOnAttemptInteract = 0;

	inline uintptr_t AFortInventory_Destroyed = 0;
	inline uintptr_t AFortInventory_DumpInventoryToLog = 0;
	inline uintptr_t AFortInventory_GetInventoryCapacity = 0;
	inline uintptr_t AFortInventory_GetOverflowFromAddingItem = 0;
	inline uintptr_t AFortInventory_GetOverflowFromAddingItemInNewStack = 0;
	inline uintptr_t AFortInventory_GetInventoryUsed = 0;
	inline uintptr_t AFortInventory_GetOverflowFromAddingItemInExistingStack = 0;
	inline uintptr_t AFortInventory_InitializeExistingItem = 0;
	inline uintptr_t AFortInventory_GetMaxStackSize = 0;

	inline uintptr_t UKismetSystemLibrary_GetEngineVersion = 0;
	inline uintptr_t UKismetSystemLibrary_ExecuteConsoleCommand = 0;

	inline uintptr_t UPlayer_GetPlayerController = 0;
	inline uintptr_t UPlayer_SwitchController = 0;

	inline uintptr_t UNetConnection_IsNetReady = 0;
	inline uintptr_t UNetConnection_IsNetReadyVFT = 0;
	inline uintptr_t UNetConnection_GetUChildConnectionVFT = 0;
	inline uintptr_t UNetConnection_FindActorChannelRef = 0;

	inline uintptr_t UNetDriver__ReplicationFrame = 0;
	inline uintptr_t UNetDriver_GetNetMode = 0;
	inline uintptr_t UNetDriver__NetworkObjects = 0;
	inline uintptr_t UNetDriver_IsAdaptiveNetUpdateFrequencyEnabled = 0;
	inline uintptr_t UNetDriver__NetTag = 0;
	inline uintptr_t UNetDriver__DestroyedStartupOrDormantActors = 0;
	inline uintptr_t UNetDriver_IsLevelInitializedForActor = 0;
	inline uintptr_t UNetDriver__DebugRelevantActors = 0;
	inline uintptr_t UNetDriver__LastPrioritizedActors = 0;
	inline uintptr_t UNetDriver__LastRelevantActors = 0;
	inline uintptr_t UNetDriver__LastSentActors = 0;
	inline uintptr_t UNetDriver__LastNonRelevantActors = 0;
	inline uintptr_t UNetDriver__GuidCache = 0;
	inline uintptr_t UNetDriver_TickDispatch = 0;
	inline uintptr_t UNetDriver__SendCycles = 0;
	inline uintptr_t UNetDriver__RecvCycles = 0;
	inline uintptr_t UNetDriver__LastTickDispatchRealtime = 0;
	inline uintptr_t UNetDriver__bIsStandbyCheckingEnabled = 0;
	inline uintptr_t UNetDriver__bHasStandbyCheatTriggered = 0;
	inline uintptr_t UNetDriver__JoinInProgressStandbyWaitTime = 0;
	inline uintptr_t UNetDriver__StandbyRxCheatTime = 0;
	inline uintptr_t UNetDriver__StandbyTxCheatTime = 0;
	inline uintptr_t UNetDriver__BadPingThreshold = 0;
	inline uintptr_t UNetDriver__PercentMissingForRxStandby = 0;
	inline uintptr_t UNetDriver__PercentMissingForTxStandby = 0;
	inline uintptr_t UNetDriver__PercentForBadPing = 0;

	inline uintptr_t APlayerController_GetViewTarget = 0;
	inline uintptr_t APlayerController_SendClientAdjustment = 0;
	inline uintptr_t APlayerController_SendClientAdjustmentVFT = 0;

	inline uintptr_t FNetworkObjectList_Remove = 0;
	inline uintptr_t FNetworkObjectList_ClearRecentlyDormantConnection = 0;
	inline uintptr_t FNetworkObjectList_Add = 0;

	inline uintptr_t UNetConnection__ActorChannels = 0;
	inline uintptr_t UNetConnection__TimeSensitive = 0;
	inline uintptr_t UNetConnection__TickCount = 0;
	inline uintptr_t UNetConnection__State = 0;
	inline uintptr_t UNetConnection__DestroyedStartupOrDormantActors = 0;
	inline uintptr_t UNetConnection__ClientWorldPackageName = 0;
	inline uintptr_t UNetConnection__ClientVisibleLevelNames = 0;
	inline uintptr_t UNetConnection_CreateChannel = 0;
	inline uintptr_t UNetConnection_CleanUp = 0;
	inline uintptr_t UNetConnection_CleanUpVFT = 0;
	inline uintptr_t UNetConnection__LastRecvAckTime = 0;

	inline uintptr_t UActorChannel__LastUpdateTime = 0;
	inline uintptr_t UActorChannel__RelevantTime = 0;
	inline uintptr_t UActorChannel_Close = 0;
	inline uintptr_t UActorChannel_StartBecomingDormant = 0;
	inline uintptr_t UActorChannel_SetChannelActorForDestroy = 0;
	inline uintptr_t UActorChannel_SetChannelActor = 0;
	inline uintptr_t UActorChannel_ReplicateActor = 0;

	inline uintptr_t FNetViewer_Constructor = 0;

	inline uintptr_t AActor_GetNetPriority = 0;
	inline uintptr_t AActor_GetNetPriorityVFT = 0;
	inline uintptr_t AActor_IsRelevancyOwnerFor = 0;
	inline uintptr_t AActor_IsRelevancyOwnerForVFT = 0;
	inline uintptr_t AActor_IsNetRelevantFor = 0;
	inline uintptr_t AActor_IsNetRelevantForVFT = 0;
	inline uintptr_t AActor_GetWorld = 0;
	inline uintptr_t AActor__CreationTime = 0;
	inline uintptr_t AActor_PreInitializeComponents = 0;
	inline uintptr_t AActor_PreInitializeComponentsVFT = 0;

	inline uintptr_t FDebug_AssertFailed = 0;

	inline uintptr_t UWorld__TimeSeconds = 0;

	inline uintptr_t FOutBunch_Constructor = 0;

	inline uintptr_t FNetGUIDCache_SupportsObject = 0;

	inline uintptr_t UObject_GetArchetypeFromRequiredInfo = 0;

	inline uintptr_t UChannel__NumInRec = 0;
	inline uintptr_t UChannel__NumOutRec = 0;
	inline uintptr_t UChannel_SendBunch = 0;
	inline uintptr_t UChannel_SendBunchVFT = 0;
	inline uintptr_t UChannel_Close = 0;
	inline uintptr_t UChannel_CloseVFT = 0;
	inline uintptr_t UChannel_StartBecomingDormantVFT = 0;

	inline uintptr_t AFortGameSession_ValidatePlayer = 0;
	inline uintptr_t AFortGameSession_CreateServerGame = 0;

	inline uintptr_t UPackageMap_WriteObject = 0;

	inline uintptr_t CollectGarbage = 0;
	inline uintptr_t CollectGarbageInternal = 0;

	inline uintptr_t AFortPlayerControllerAthena_ClientRestart = 0;
	inline uintptr_t AFortPlayerControllerAthena_SpawnQuickBars = 0;
	inline uintptr_t AFortPlayerControllerAthena_SetupQuickBars = 0;

	inline uintptr_t AFortGameState_ApplyHomebaseEffectsOnPlayerSetup = 0;
	inline uintptr_t AFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT = 0;

	inline uintptr_t UMcpProfileGroup_SendRequestNow = 0;

	inline uintptr_t AFortPlayerPawn_ServerChoosePart = 0;

	inline uintptr_t AFortPlayerState_InitializeHero = 0;
	inline uintptr_t AFortPlayerState_RefreshCharacterCustomization = 0;
	inline uintptr_t AFortPlayerState_ApplyCharacterCustomization = 0;

	inline uintptr_t FString_Append = 0;
	inline uintptr_t FString_AppendInt = 0;
	inline uintptr_t FString_ToBool = 0;

	inline uintptr_t AFortPlayerController_OnReadyToStartMatch = 0;
	inline uintptr_t AFortPlayerController_OnReadyToStartMatchVFT = 0;
	inline uintptr_t AFortPlayerController_SpawnQuickBarsVFT = 0;
	inline uintptr_t AFortPlayerController_SetupQuickBarsVFT = 0;
	inline uintptr_t AFortPlayerController_RemoveInventoryItem = 0;
	inline uintptr_t AFortPlayerController_RemoveInventoryItemVFT = 0;
	inline uintptr_t AFortPlayerController_CanAffordToPlaceBuildableClass = 0;
	inline uintptr_t AFortPlayerController_CanAffordToPlaceBuildableClassVFT = 0;
	inline uintptr_t AFortPlayerController_PayBuildableClassPlacementCost = 0;
	inline uintptr_t AFortPlayerController_PayBuildableClassPlacementCostVFT = 0;
	inline uintptr_t AFortPlayerController_PayBuildingRepairCost = 0;
	inline uintptr_t AFortPlayerController_GetPlayerViewPoint = 0;

	inline uintptr_t AFortPlayerControllerZone_OnReadyToStartMatch = 0;
	inline uintptr_t AFortPlayerControllerZone_ClientOnPawnDied = 0;

	inline uintptr_t AFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup = 0;

	inline uintptr_t AActor_GetNetOwnerVFT = 0;

	inline uintptr_t UFortWorldItemDefinition_ServerExecuteVFT = 0;

	inline uintptr_t UFortWeaponItemDefinition_GetMaxDurability = 0;
	inline uintptr_t UFortWeaponItemDefinition_GetMaxDurabilityVFT = 0;

	inline uintptr_t UFortWorldItem_SetOwningInventory = 0;
	inline uintptr_t UFortWorldItem_SetOwningInventoryVFT = 0;
	inline uintptr_t UFortWorldItem_SetLoadedAmmoVFT = 0;
	inline uintptr_t UFortWorldItem_SetPhantomReserveAmmoVFT = 0;
	inline uintptr_t UFortWorldItem_SetInInventoryOverflowVFT = 0;
	inline uintptr_t UFortWorldItem_SetDurabilityVFT = 0;

	inline uintptr_t UFortControllerComponent_Aircraft_EnterAircraft = 0;

	inline uintptr_t AController_InitPlayerState = 0;
	inline uintptr_t AController_InitPlayerStateVFT = 0;
	inline uintptr_t AController_GetPlayerViewPointVFT = 0;
	inline uintptr_t AController_Reset = 0;

	inline uintptr_t UFortAnalytics_SetGameSessionID = 0;
	inline uintptr_t UFortAnalytics_SetGameStateClassName = 0;

	inline uintptr_t AFortGameSessionDedicated_FinalizeCreation = 0;
	inline uintptr_t AFortGameSessionDedicated_FinalizeCreationPatch1 = 0;
	inline uintptr_t AFortGameSessionDedicated_FinalizeCreationVFT = 0;
	inline uintptr_t AFortGameSessionDedicated_OnUpdateComplete = 0;
	inline uintptr_t AFortGameSessionDedicated_OnAllPlayersUnregistered = 0;
	inline uintptr_t AFortGameSessionDedicated_OnServerConfigurationRequest = 0;
	inline uintptr_t AFortGameSessionDedicated_OnServerConfigurationRequestVFT = 0;

	inline uintptr_t FFortBaseWeaponStats_GetDurability = 0;

	inline uintptr_t UNavigationSystem_CreateNavigationSystem = 0;

	inline uintptr_t FDedicatedServerUrlContext_Constructor = 0;

	inline uintptr_t UFortQuestManager_SendCustomStatEvent = 0;
	inline uintptr_t UFortQuestManager_QueueStatEvent = 0;
	inline uintptr_t UFortQuestManager_SendStatEvent = 0;
	inline uintptr_t UFortQuestManager_ProcessPendingStatEvents = 0;

	inline uintptr_t BeginLoad = 0;
	inline uintptr_t EndLoad = 0;

	inline uintptr_t UGameplayAbility_CanActivateAbility = 0;
	inline uintptr_t UGameplayAbility_CanActivateAbilityVFT = 0;

	inline uintptr_t UFortGameInstance_ServerTravel = 0;
	
	inline uintptr_t FMsg_Logf = 0;

	inline uintptr_t UFortAssetManager_Get = 0;

	inline uintptr_t UDemoNetDriver_TickFlushInternal = 0;

	inline uintptr_t AFortAthenaMapInfo_PickSupplyDropLocation = 0;

	inline uintptr_t AFortDecoTool_ShouldAllowServerSpawnDecoVFT = 0;
	inline uintptr_t AFortDecoTool_SpawnDecoVFT = 0;
}