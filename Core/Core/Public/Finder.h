#pragma once
#include "pch.h"

class Finder {
public:
	static uintptr_t FindGUObjectArray();

	static uintptr_t FindGEngine();

	static uintptr_t FindGWorld();

	static uintptr_t FindProcessEventVFT();

	static uintptr_t FindProcessEvent();

	static uintptr_t FindGIsClient();

	static uintptr_t FindGIsServer();

	static uintptr_t FindStaticFindObject();

	static uintptr_t FindStaticLoadObject();

	static uintptr_t FindUEngine_CreateNetDriver();

	static uintptr_t FindUNetDriver_SetWorldVFT();

	static uintptr_t FindUNetDriver_SetWorld();

	static uintptr_t FindUNetDriver_InitListen();

	static uintptr_t FindUNetDriver_InitListenVFT();

	static uintptr_t FindChangeGameSessionId();

	static uintptr_t FindInternalServerTryActivateAbilityVFT();

	static uintptr_t FindUAbilitySystemComponent_GiveAbility();

	static uintptr_t FindGHandle();

	static uintptr_t FindAbilitySpecDefaultConstructor();

	static uintptr_t FindAbilitySpecClassConstructor();

	static uintptr_t FindAbilitySpecCDOConstructor();

	static uintptr_t FindUAbilitySystemComponent_InternalTryActivateAbility();

	static uintptr_t FindABuildingActor_OnDamageServer();

	static uintptr_t FindSpawnActor();

	static uintptr_t FindStep();

	static uintptr_t FindStepExplicitProperty();

	static uintptr_t FindFMemory_Free();

	static uintptr_t FindFMemory_Realloc();

	static uintptr_t FindFName_AppendString();

	static uintptr_t FindFName_ToStringOut();

	static uintptr_t FindFName_Compare();

	static uintptr_t FindUObjectBase_AddObject();

	static uintptr_t FindUObjectBase_AtomicallyClearFlags();

	static uintptr_t FindUObjectBase_DeferredRegister();

	static uintptr_t FindUObjectBase_EmitBaseReferences();

	static uintptr_t FindUObjectBase_IsValidLowLevel();

	static uintptr_t FindUObjectBase_IsValidLowLevelFast();

	static uintptr_t FindUObjectBase_LowLevelRename();

	static uintptr_t FindUObjectBase_Register();

	static uintptr_t FindUObjectBase_Destructor();

	static uintptr_t FindUObjectBaseUtility_AddToCluster();

	static uintptr_t FindUObjectBaseUtility_CanBeInCluster();

	static uintptr_t FindUObjectBaseUtility_GetFullName();

	static uintptr_t FindUObjectBaseUtility_GetFullName_1();

	static uintptr_t FindUObjectBaseUtility_GetInterfaceAddress();

	static uintptr_t FindUObjectBaseUtility_GetLinker();

	static uintptr_t FindUObjectBaseUtility_GetLinkerIndex();

	static uintptr_t FindUObjectBaseUtility_GetNativeInterfaceAddress();

	static uintptr_t FindUObjectBaseUtility_GetOutermost();

	static uintptr_t FindUObjectBaseUtility_GetPathName();

	static uintptr_t FindUObjectBaseUtility_GetTypedOuter();

	static uintptr_t FindUObjectBaseUtility_IsDefaultSubobject();

	static uintptr_t FindUObjectBaseUtility_IsIn();

	static uintptr_t FindUObjectBaseUtility_IsInA();

	static uintptr_t FindUObjectBaseUtility_IsTemplate();

	static uintptr_t FindUObjectBaseUtility_MarkPackageDirty();

	static uintptr_t FindUObject_BeginDestroy();

	static uintptr_t FindUObject_FinishDestroy();

	static uintptr_t FindUObject_DefaultConstructor();

	static uintptr_t FindUObject_GetDefaultSubobjects();

	static uintptr_t FindUObject_GetDefaultSubobjectByName();

	static uintptr_t FindUObject_PostInitProperties();

	static uintptr_t FindUObject_PostLoad();

	static uintptr_t FindUObject_FindFunctionChecked();

	static uintptr_t FindUField_GetOwnerClass();

	static uintptr_t FindUField_GetOwnerStruct();

	static uintptr_t FindUStruct_FindPropertyByName();

	static uintptr_t FindUStruct_IsChildOf();

	static uintptr_t FindUClass_FindFunctionByName();

	static uintptr_t FindFName_GetPlainNameString1();

	static uintptr_t FindFName_GetPlainNameString2();

	static uintptr_t FindUEngine_BroadcastNetworkFailure();

	static uintptr_t FindUEngine_BroadcastNetworkLagStateChanged();

	static uintptr_t FindUEngine_CancelAllPending();

	static uintptr_t FindUEngine_CreateGameUserSettings();

	static uintptr_t FindUEngine_GetFirstGamePlayer();

	static uintptr_t FindUEngine_GetNetMode();

	static uintptr_t FindUEngine_GetMaxTickRate();

	static uintptr_t FindUEngine_GetMaxTickRateVFT();

	static uintptr_t FindUWorld_AddController();

	static uintptr_t FindUWorld_AddLevel();

	static uintptr_t FindUWorld_AddNetworkActor();

	static uintptr_t FindUWorld_AddStreamingLevel();

	static uintptr_t FindUWorld_AddToWorld();

	static uintptr_t FindUWorld_AllowLevelLoadRequests();

	static uintptr_t FindUWorld_AreActorsInitialized();

	static uintptr_t FindUWorld_AreAlwaysLoadedLevelsLoaded();

	static uintptr_t FindUWorld_BeginPlay();

	static uintptr_t FindUWorld_CommitMapChange();

	static uintptr_t FindUWorld_ConditionallyCreateDefaultLevelCollections();

	static uintptr_t FindUWorld_ContainsActor();

	static uintptr_t FindUWorld_ContainsLevel();

	static uintptr_t FindUWorld_CreateAISystem();

	static uintptr_t FindUWorld_GetActiveLevelCollection();

	static uintptr_t FindUWorld_GetActorCount();

	static uintptr_t FindUWorld_GetAddressURL();

	static uintptr_t FindUWorld_GetDefaultGravityZ();

	static uintptr_t FindUWorld_GetFirstLocalPlayerFromController();

	static uintptr_t FindUWorld_GetFirstPlayerController();

	static uintptr_t FindUWorld_GetGameViewport();

	static uintptr_t FindUWorld_GetGravityZ();

	static uintptr_t FindUWorld_GetMapName();

	static uintptr_t FindUWorld_GetTimerManager();

	static uintptr_t FindUWorld_GetWorldSettings();

	static uintptr_t FindUWorld_K2_GetWorldSettings();

	static uintptr_t FindUWorld_HasBegunPlay();

	static uintptr_t FindUWorld_InternalGetNetMode();

	static uintptr_t FindUWorld_IsGameWorld();

	static uintptr_t FindUWorld_IsInSeamlessTravel();

	static uintptr_t FindUWorld_IsServer();

	static uintptr_t FindUWorld_MarkObjectsPendingKill();

	static uintptr_t FindUWorld_RemoveActor();

	static uintptr_t FindUWorld_RemoveController();

	static uintptr_t FindUWorld_RemoveNetworkActor();

	static uintptr_t FindUWorld_SeamlessTravel();

	static uintptr_t FindUWorld_ServerTravel();

	static uintptr_t FindUWorld_SetGameMode();

	static uintptr_t FindUWorld_SetGameState();

	static uintptr_t FindUWorld_SetNavigationSystem();

	static uintptr_t FindUWorld_WelcomePlayer();

	static uintptr_t FindFGuid_ToString();

	static uintptr_t FindUGameInstance_StartGameInstance();

	static uintptr_t FindUGameInstance_Shutdown();

	static uintptr_t FindUGameInstance_GetFirstLocalPlayerController();

	static uintptr_t FindUGameInstance_ClientTravelToSession();

	static uintptr_t FindUGameInstance_CleanupGameViewport();

	static uintptr_t FindAActor_ActorHasTag();

	static uintptr_t FindAActor_CallPreReplication();

	static uintptr_t FindAActor_CheckStillInWorld();

	static uintptr_t FindAActor_ClearCrossLevelReferences();

	static uintptr_t FindAActor_CopyRemoteRoleFrom();

	static uintptr_t FindAActor_Destroy();

	static uintptr_t FindAActor_DisableInput();

	static uintptr_t FindAActor_EnableInput();

	static uintptr_t FindAActor_ExchangeNetRoles();

	static uintptr_t FindAActor_FindOrAddNetworkObjectInfo();

	static uintptr_t FindAActor_InternalGetNetMode();

	static uintptr_t FindAGameSession_ApproveLogin();

	static uintptr_t FindAGameSession_AtCapacity();

	static uintptr_t FindAGameSession_GetSessionJoinability();

	static uintptr_t FindAGameSession_KickPlayer();
	
	static uintptr_t FindAGameSession_KickPlayerVFT();

	static uintptr_t FindAGameSession_ReturnToMainMenuHost();

	static uintptr_t FindAGameSession_UnregisterPlayer();

	static uintptr_t FindAGameSession_UpdateSessionJoinability();

	static uintptr_t FindAGameSession_InitOptions();

	static uintptr_t FindAGameSession_RegisterPlayer();

	static uintptr_t FindAGameSession_ProcessAutoLogin();

	static uintptr_t FindAGameSession_OnAutoLoginComplete();

	static uintptr_t FindAGameSession_UnregisterPlayer_1();

	static uintptr_t FindAGameSession_UnregisterPlayers();

	static uintptr_t FindAGameSession_GetPlayerControllerFromNetId();

	static uintptr_t FindAGameSession__NextPlayerID();

	static uintptr_t FindAGameModeBase_InitGame();

	static uintptr_t FindAGameModeBase_InitGameState();

	static uintptr_t FindAGameModeBase_CanServerTravel();

	static uintptr_t FindUKismetStringLibrary_Conv_StringToName();

	static uintptr_t FindAGameMode_AddInactivePlayer();

	static uintptr_t FindAGameMode_Broadcast();

	static uintptr_t FindAGameMode_BroadcastLocalized();

	static uintptr_t FindAGameMode_EndMatch();

	static uintptr_t FindAGameMode_FindInactivePlayer();

	static uintptr_t FindAGameMode_GetNetworkNumber();

	static uintptr_t FindAGameMode_HandleMatchHasEnded();

	static uintptr_t FindAGameMode_HandleMatchHasStarted();

	static uintptr_t FindAGameMode_HandleMatchIsWaitingToStart();

	static uintptr_t FindAGameMode_HandleSeamlessTravelPlayer();

	static uintptr_t FindAGameMode_HandleStartingNewPlayer();

	static uintptr_t FindAGameMode_InitSeamlessTravelPlayer();

	static uintptr_t FindAGameMode_IsHandlingReplays();

	static uintptr_t FindAGameMode_OnMatchStateSet();

	static uintptr_t FindAGameMode_PlayerCanRestart();

	static uintptr_t FindAGameMode_PostSeamlessTravel();

	static uintptr_t FindAGameMode_ReadyToStartMatch();

	static uintptr_t FindAGameMode_RestartGame();

	static uintptr_t FindAGameMode_Say();

	static uintptr_t FindAGameMode_SendPlayer();

	static uintptr_t FindAGameMode_SetMatchState();

	static uintptr_t FindAGameMode_SetSeamlessTravelViewTarget();

	static uintptr_t FindAGameMode_StartMatch();

	static uintptr_t FindAGameMode_StartPlay();

	static uintptr_t FindAPlayerController_HasClientLoadedCurrentWorld();

	static uintptr_t FindUGameMapsSettings_GetGameModeForName();

	static uintptr_t FindAGameMode_OverridePlayerState();

	static uintptr_t FindAGameMode_PostLogin();

	static uintptr_t FindAGameMode_Logout();

	static uintptr_t FindAGameMode_CanServerTravel();

	static uintptr_t FindULevelStreaming_PostLoad();

	static uintptr_t FindULevelStreaming_DetermineTargetState();

	static uintptr_t FindULevelStreaming_OnLevelAdded();

	static uintptr_t FindULevelStreaming_IsDesiredLevelLoaded();

	static uintptr_t FindUWorld_UpdateStreamingLevelShouldBeConsidered();

	static uintptr_t FindULevelStreaming_SetLevelLODIndex();

	static uintptr_t FindULevelStreaming_SetPriority();

	static uintptr_t FindULevelStreaming_SetWorldAsset();

	static uintptr_t FindULevelStreaming_GetWorldAssetPackageName();

	static uintptr_t FindULevelStreaming_GetWorldAssetPackageFName();

	static uintptr_t FindULevelStreaming_RenameForPIE();

	static uintptr_t FindULevelStreaming_ShouldBeVisible();

	static uintptr_t FindULevelStreaming_IsStreamingStatePending();

	static uintptr_t FindULevelStreaming_CreateInstance();

	static uintptr_t FindULevelStreaming_RequestLevel();

	static uintptr_t FindULevelStreaming_SetLoadedLevel();

	static uintptr_t FindULevelStreaming_DiscardPendingUnloadLevel();

	static uintptr_t FindULevel_SortActorList();

	static uintptr_t FindULevel_InitializeNetworkActors();

	static uintptr_t FindULevel_ClearActorsSeamlessTraveledFlag();

	static uintptr_t FindULevel_SetWorldSettings();

	static uintptr_t FindULevel_IsNetActor();

	static uintptr_t FindULevel_PostInitProperties();

	static uintptr_t FindULevel_BeginDestroy();

	static uintptr_t FindULevel_FinishDestroy();

	static uintptr_t FindULevel_PostLoad();

	static uintptr_t FindULevel_CreateCluster();

	static uintptr_t FindULevel_ClearLevelComponents();

	static uintptr_t FindULevel_UpdateLevelComponents();

	static uintptr_t FindULevel_IncrementalUpdateComponents();

	static uintptr_t FindULevel_IncrementalUnregisterComponents();

	static uintptr_t FindULevel_RouteActorInitialize();

	static uintptr_t FindAActor_IsPendingKillPending();

	static uintptr_t FindULevel_ApplyWorldOffset();

	static uintptr_t FindULevel_RegisterActorForAutoReceiveInput();

	static uintptr_t FindULevel_PushPendingAutoReceiveInput();

	static uintptr_t FindAActor_HasNetOwner();

	static uintptr_t FindAActor_HasLocalNetOwner();

	static uintptr_t FindUEngine_LoadMap();

	static uintptr_t FindUEngine_LoadMapVFT();

	static uintptr_t FindFURL_IsInternal();

	static uintptr_t FindFURL_IsLocalInternal();

	static uintptr_t FindFURL_HasOption();

	static uintptr_t FindFURL_GetOption();

	static uintptr_t FindFURL_AddOption();

	static uintptr_t FindFURL_RemoveOption();

	static uintptr_t FindFURL_ToString();

	static uintptr_t FindFURL_GetHostPortString();

	static uintptr_t FindFWorldContext_SetCurrentWorld();

	static uintptr_t FindFWorldContext__ThisCurrentWorld();

	static uintptr_t FindUEngine_CreateNamedNetDriver();

	static uintptr_t FindUEngine_GetWorldContextFromWorldChecked();

	static uintptr_t FindUEngine_FindNamedNetDriver();

	static uintptr_t FindUWorld_FindCollectionByType();

	static uintptr_t FindUEngine_DestroyNamedNetDriver();

	static uintptr_t FindFParse_Param();

	static uintptr_t FindAFortGameModeAthena_SetCurrentPlaylistId();

	static uintptr_t FindAFortGameMode_OnInitWorldRecordComplete();

	static uintptr_t FindUWorld__NextSwitchCountdown();

	static uintptr_t FindAFortGameMode_SetCurrentPlaylistName();

	static uintptr_t FindAFortGameMode_OnEndOfDay();

	static uintptr_t FindAFortGameModeZone_CheckForAllPlayersIn();

	static uintptr_t FindAFortGameModeZone_RemoveKnownPlayer();

	static uintptr_t FindAFortGameModeZone_UpdateSessionForMatchmaking();

	static uintptr_t FindAFortGameModeZone_NotifyPendingConnectionLost();

	static uintptr_t FindAFortGameModeZone_OnZoneEnd();

	static uintptr_t FindAFortGameModeZone_DoLogoutAnalytics();

	static uintptr_t FindAFortGameModeZone_HandleMatchHasStarted();

	static uintptr_t FindAFortGameModeZone_HandleMatchHasEnded();

	static uintptr_t FindAFortGameModeZone_DoInitialRestartAnalytics();

	static uintptr_t FindAFortGameModeAthena_SetTimeOfDayManagerGameplayOverride();

	static uintptr_t FindAFortGameModeAthena_OnPlaylistDataLoaded();

	static uintptr_t FindAFortGameModeAthena_ApplyPlaylistDataTables();

	static uintptr_t FindAFortGameModeAthena_OnDedicatedServerRestarting();

	static uintptr_t FindAFortGameModeAthena_SpawnInitialSafeZone();

	static uintptr_t FindAFortGameModeAthena_AttemptChangeTeam();

	static uintptr_t FindAFortGameModeAthena_PickTeam();

	static uintptr_t FindAFortGameModeAthena_HandleMatchHasStarted();

	static uintptr_t FindAFortGameModeAthena_FinishRestartPlayer();

	static uintptr_t FindAFortGameModeAthena_StartMatchAbandonProcess();

	static uintptr_t FindAFortGameModeAthena_OnMatchAbandonTimeout();

	static uintptr_t FindAFortGameModeAthena_CheckWinConditionLastManStanding();

	static uintptr_t FindAFortGameModeAthena_GrantVictoryToAllRemainingTeams();

	static uintptr_t FindAFortGameModeAthena_GrantVictoryForNumTeamsAndEndGame();

	static uintptr_t FindAFortGameModeAthena_GrantLossToAllRemainingTeams();

	static uintptr_t FindAFortGameModeAthena_GrantLossToAllTeams();

	static uintptr_t FindAFortGameModeAthena_GrantLosses();

	static uintptr_t FindAFortGameModeAthena_ChoosePlayerStart_Creative();

	static uintptr_t FindAFortGameModeAthena_StartWarmupPhase();

	static uintptr_t FindAFortGameModeAthena_UpdateWarmupPhase();

	static uintptr_t FindAFortGameModeAthena_StartAircraftPhase();

	static uintptr_t FindAFortGameModeAthena_StartSafeZonesPhase();

	static uintptr_t FindAFortGameModeAthena_IsSafeZoneAllowed();

	static uintptr_t FindAFortGameModeAthena_StartEndGamePhase();

	static uintptr_t FindAFortGameModeAthena_StartEndGamePhaseTeam();

	static uintptr_t FindAFortGameModeAthena_CreateAIDirector();

	static uintptr_t FindAFortGameModeAthena_PostReloadConfig();

	static uintptr_t FindAFortGameModeAthena_ReadyToStartMatch();

	static uintptr_t FindAFortGameStateAthena_OnRep_CurrentPlaylistInfo();

	static uintptr_t FindAGameMode_InitGame();

	static uintptr_t FindAFortGameMode_InitGame();

	static uintptr_t FindAFortGameModeZone_InitGame();

	static uintptr_t FindAFortGameModeAthena_InitGame();

	static uintptr_t FindAFortGameModeZone_InitGameState();

	static uintptr_t FindAFortGameModeAthena_InitGameState();

	static uintptr_t FindUFortPlaylistManager_InitializePlaylists();

	static uintptr_t FindUNetDriver_TickFlush();

	static uintptr_t FindUReplicationDriver_ServerReplicateActors();

	static uintptr_t FindAFortWorldManager_HandleWorldQueryComplete();

	static uintptr_t FindAFortGameModeAthena_SpawnDefaultPawnFor();

	static uintptr_t FindAGameModeBase_SpawnDefaultPawnFor();

	static uintptr_t FindAGameModeBase_SpawnDefaultPawnAtTransform();

	static uintptr_t FindAGameModeBase_RestartPlayer();

	static uintptr_t FindAFortGameMode_RestartPlayer();

	static uintptr_t FindAFortGameModeAthena_RestartPlayer();

	static uintptr_t FindAGameModeBase_FinishRestartPlayer();

	static uintptr_t FindAGameModeBase_FinishRestartPlayerVFT();

	static uintptr_t FindAFortGameMode_FinishRestartPlayer();

	static uintptr_t FindAPlayerController_ServerAcknowledgePossession();

	static uintptr_t FindUAbilitySystemComponent_ClientActivateAbilityFailed();

	static uintptr_t FindAGameModeBase_HandleStartingNewPlayer();

	static uintptr_t FindULevelStreamingDynamic_LoadLevelInstance();

	static uintptr_t FindULevelStreamingDynamic_LoadLevelInstanceBySoftObjectPtr();

	static uintptr_t FindAFortGameStateAthena_OnPlaylistDataLoadCompleted();

	static uintptr_t FindAFortGameStateAthena_UpdatePlaylistDependentData();

	static uintptr_t FindUGameplayStatics_BeginDeferredActorSpawnFromClass();

	static uintptr_t FindAActor_FinishSpawning();

	static uintptr_t FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid();

	static uintptr_t FindAFortGameMode_SpawnDefaultPawnFor();

	static uintptr_t FindFRotator_Quaternion();

	static uintptr_t FindAActor_SetNetDormancy();

	static uintptr_t FindABuildingSMActor_ReplaceBuildingActor();

	static uintptr_t FindABuildingSMActor_ReplaceBuildingActorVFT();

	static uintptr_t FindUAbilitySystemComponent_GiveAbilityAndActivateOnce();

	static uintptr_t FindAFortPickup_FinishedTargetSpline();

	static uintptr_t FindAFortPickup_ForceFinishedTargetSpline();

	static uintptr_t FindAFortPlayerPawn_ForceFinishIncomingPickups();

	static uintptr_t FindAFortPlayerPawn_ForceFinishIncomingPickupsVFT();

	static uintptr_t FindAFortPickup_SetPickupTarget();

	static uintptr_t FindAFortPickup_UpdateSpecialActorStat();

	static uintptr_t FindAFortPickup_GivePickupTo();

	static uintptr_t FindAFortPickup_GivePickupToVFT();

	static uintptr_t FindAFortQuickBars_GetFocusedQuickBar();

	static uintptr_t FindABuildingContainer_SpawnLootVFT();

	static uintptr_t FindAFortPickup_SetupForMovementCompToss();

	static uintptr_t FindAFortPickup_SetPickupItems();

	static uintptr_t FindAFortPickup_SetPickupItemsVFT();

	static uintptr_t FindAFortInventory_Destroyed();

	static uintptr_t FindAFortInventory_DumpInventoryToLog();

	static uintptr_t FindAFortInventory_GetInventoryCapacity();

	static uintptr_t FindAFortInventory_GetOverflowFromAddingItem();

	static uintptr_t FindAFortInventory_GetOverflowFromAddingItemInNewStack();

	static uintptr_t FindAFortInventory_GetInventoryUsed();

	static uintptr_t FindAFortInventory_GetOverflowFromAddingItemInExistingStack();

	static uintptr_t FindAFortInventory_InitializeExistingItem();

	static uintptr_t FindAFortInventory_GetMaxStackSize();

	static uintptr_t FindUKismetSystemLibrary_GetEngineVersion();

	static uintptr_t FindUEngine_GetWorldContextFromWorld();

	static uintptr_t FindUEngine_CreateNewWorldContext();

	static uintptr_t FindUEngine_CreateNetDriver_Local();

	static uintptr_t FindUPlayer_GetPlayerController();

	static uintptr_t FindUPlayer_SwitchController();

	static uintptr_t FindUNetConnection_IsNetReady();

	static uintptr_t FindUNetConnection_IsNetReadyVFT();

	static uintptr_t FindUNetDriver__ReplicationFrame();

	static uintptr_t FindUNetDriver_GetNetMode();

	static uintptr_t FindAPlayerController_GetViewTarget();

	static uintptr_t FindUNetDriver__NetworkObjects();

	static uintptr_t FindUNetDriver_IsAdaptiveNetUpdateFrequencyEnabled();

	static uintptr_t FindFNetworkObjectList_Remove();

	static uintptr_t FindUNetConnection__ActorChannels();

	static uintptr_t FindUActorChannel__LastUpdateTime();

	static uintptr_t FindUNetConnection__TimeSensitive();

	static uintptr_t FindFNetViewer_Constructor();

	static uintptr_t FindAPlayerController_SendClientAdjustment();

	static uintptr_t FindAPlayerController_SendClientAdjustmentVFT();

	static uintptr_t FindAActor_GetNetPriority();

	static uintptr_t FindAActor_GetNetPriorityVFT();

	static uintptr_t FindUNetDriver__NetTag();

	static uintptr_t FindUNetDriver__DebugRelevantActors();

	static uintptr_t FindUNetDriver__LastPrioritizedActors();

	static uintptr_t FindUNetConnection__TickCount();

	static uintptr_t FindUNetDriver__DestroyedStartupOrDormantActors();

	static uintptr_t FindAActor_IsRelevancyOwnerFor();

	static uintptr_t FindAActor_IsRelevancyOwnerForVFT();

	static uintptr_t FindUActorChannel__RelevantTime();

	static uintptr_t FindUActorChannel_Close();

	static uintptr_t FindUActorChannel_StartBecomingDormant();

	static uintptr_t FindUNetDriver_IsLevelInitializedForActor();

	static uintptr_t FindAActor_IsNetRelevantFor();

	static uintptr_t FindAActor_IsNetRelevantForVFT();

	static uintptr_t FindFDebug_AssertFailed();

	static uintptr_t FindUNetConnection__State();

	static uintptr_t FindAActor_GetWorld();

	static uintptr_t FindUWorld__TimeSeconds();

	static uintptr_t FindUNetConnection__DestroyedStartupOrDormantActors();

	static uintptr_t FindUNetConnection__ClientWorldPackageName();

	static uintptr_t FindUNetConnection__ClientVisibleLevelNames();

	static uintptr_t FindUNetConnection_CreateChannel();

	static uintptr_t FindUActorChannel_SetChannelActorForDestroy();

	static uintptr_t FindFOutBunch_Constructor();

	static uintptr_t FindUNetDriver__LastRelevantActors();

	static uintptr_t FindUNetDriver__LastSentActors();

	static uintptr_t FindUNetDriver__LastNonRelevantActors();

	static uintptr_t FindUNetDriver__GuidCache();

	static uintptr_t FindFNetGUIDCache_SupportsObject();

	static uintptr_t FindUObject_GetArchetypeFromRequiredInfo();

	static uintptr_t FindUActorChannel_SetChannelActor();

	static uintptr_t FindUChannel__NumInRec();

	static uintptr_t FindUChannel__NumOutRec();

	static uintptr_t FindUActorChannel_ReplicateActor();

	static uintptr_t FindUNetDriver_TickDispatch();

	static uintptr_t FindUNetDriver__SendCycles();

	static uintptr_t FindUNetDriver__RecvCycles();

	static uintptr_t FindUNetDriver__LastTickDispatchRealtime();

	static uintptr_t FindUNetConnection_CleanUp();

	static uintptr_t FindUNetConnection_CleanUpVFT();

	static uintptr_t FindUNetDriver__bIsStandbyCheckingEnabled();

	static uintptr_t FindUNetDriver__bHasStandbyCheatTriggered();

	static uintptr_t FindAActor__CreationTime();

	static uintptr_t FindUNetDriver__JoinInProgressStandbyWaitTime();

	static uintptr_t FindUNetConnection__LastRecvAckTime();

	static uintptr_t FindUNetDriver__StandbyRxCheatTime();

	static uintptr_t FindUNetDriver__StandbyTxCheatTime();

	static uintptr_t FindUNetDriver__BadPingThreshold();

	static uintptr_t FindUNetDriver__PercentMissingForRxStandby();

	static uintptr_t FindUNetDriver__PercentMissingForTxStandby();

	static uintptr_t FindUNetDriver__PercentForBadPing();

	static uintptr_t FindAFortGameSession_ValidatePlayer();

	static uintptr_t FindUPackageMap_WriteObject();

	static uintptr_t FindUChannel_SendBunch();

	static uintptr_t FindUChannel_SendBunchVFT();

	static uintptr_t FindFNetworkObjectList_ClearRecentlyDormantConnection();

	static uintptr_t FindFNetworkObjectList_Add();

	static uintptr_t FindUKismetSystemLibrary_ExecuteConsoleCommand();

	static uintptr_t FindAFortGameMode_SetCurrentPlaylistId();

	static uintptr_t FindCollectGarbage();

	static uintptr_t FindCollectGarbageInternal();

	static uintptr_t FindAFortGameModeZone_ChoosePlayerStart();

	static uintptr_t FindAFortPlayerControllerAthena_ClientRestart();

	static uintptr_t FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetup();

	static uintptr_t FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT();

	static uintptr_t FindUMcpProfileGroup_SendRequestNow();

	static uintptr_t FindAFortPlayerPawn_ServerChoosePart();

	static uintptr_t FindAFortPlayerState_InitializeHero();

	static uintptr_t FindAFortPlayerState_RefreshCharacterCustomization();

	static uintptr_t FindFName_GetComparisonNameEntry();

	static uintptr_t FindFString_Append();

	static uintptr_t FindFString_AppendInt();

	static uintptr_t FindFString_ToBool();

	static uintptr_t FindAFortGameModeAthena_FinishWorldInitialization();

	static uintptr_t FindAFortGameMode_FinishWorldInitializationVFT();

	static uintptr_t FindAFortGameStateAthena_SetCurrentPlaylistId();

	static uintptr_t FindAFortPlayerController_OnReadyToStartMatch();

	static uintptr_t FindAFortPlayerControllerZone_OnReadyToStartMatch();

	static uintptr_t FindAFortPlayerController_OnReadyToStartMatchVFT();

	static uintptr_t FindAFortPlayerControllerAthena_SpawnQuickBars();

	static uintptr_t FindAFortPlayerControllerAthena_SetupQuickBars();

	static uintptr_t FindAFortGameStateZone_ApplyHomebaseEffectsOnPlayerSetup();

	static uintptr_t FindAActor_GetNetOwnerVFT();

	static uintptr_t FindUAbilitySystemComponent_FindAbilitySpecFromHandle();

	static uintptr_t FindUChannel_CloseVFT();

	static uintptr_t FindUChannel_StartBecomingDormantVFT();

	static uintptr_t FindUNetConnection_GetUChildConnectionVFT();

	static uintptr_t FindUNetDriver_IsServerVFT();

	static uintptr_t FindUNetDriver_IsLevelInitializedForActorVFT();

	static uintptr_t FindUObject_GetWorldVFT();

	static uintptr_t FindAFortPlayerController_SpawnQuickBarsVFT();

	static uintptr_t FindAFortPlayerController_SetupQuickBarsVFT();

	static uintptr_t FindAFortGameModeZone_CreateAIDirectorVFT();

	static uintptr_t FindAFortGameModeZone_CreateAIGoalManagerVFT();

	static uintptr_t FindUObject_NeedsLoadForClientVFT();

	static uintptr_t FindUFortWorldItemDefinition_ServerExecuteVFT();

	static uintptr_t FindABuildingSMActor_PostUpdate();

	static uintptr_t FindABuildingActor_PostUpdateVFT();
public:
	static void SetupOffsets();

	template <typename CVarT>
	static CVarT* FindCVar(const wchar_t* CVarStr)
	{
		auto sRef = Memcury::Scanner::FindStringRef(CVarStr);

		if (!sRef.IsValid())
			return nullptr;

		uint64_t BeforeVars = 0;

		for (int i = 0; i < 2000; i++)
		{
			auto Ptr = (uint8_t*)(sRef.Get() - i);

			if (*Ptr == 0x48 && *(Ptr + 1) == 0x83 && *(Ptr + 2) == 0xEC)
			{
				BeforeVars = uint64_t(Ptr);
				break;
			}
			else if (*Ptr == 0x48 && *(Ptr + 1) == 0x81 && *(Ptr + 2) == 0xEC)
			{
				BeforeVars = uint64_t(Ptr);
				break;
			}
		}

		for (int i = 0; i < 2000; i++)
		{
			auto Ptr = (uint8_t*)(BeforeVars + i);

			if (*Ptr == 0x4C && *(Ptr + 1) == 0x8D && *(Ptr + 2) == 0x05)
				return Memcury::Scanner(Ptr).RelativeOffset(3).GetAs<CVarT*>();
		}

		return nullptr;
	}

	template <typename CVarT>
	static CVarT* FindCVarDirect(const wchar_t* CVarStr)
	{
		auto sRef = Memcury::Scanner::FindStringRef(CVarStr);

		if (!sRef.IsValid())
		{
			return nullptr;
		}

		for (int i = 0; i < 1000; i++)
		{
			auto Ptr = (uint8_t*)(sRef.Get() + i);

			if (*Ptr == 0x48 && *(Ptr + 1) == 0x89 && *(Ptr + 2) == 0x05)
			{
				auto VarPtr = Memcury::Scanner(Ptr).RelativeOffset(3).GetAs<CVarT**>();
				if (VarPtr && *VarPtr)
				{
					if (**VarPtr >= 0 && **VarPtr <= 1)
					{
						return *VarPtr;
					}
				}
			}

			else if (*Ptr == 0xFF && (*(Ptr + 1) == 0x50 || *(Ptr + 1) == 0x10))
			{
				auto NextPtr = Ptr + 3;

				if (*NextPtr == 0x48 && *(NextPtr + 1) == 0x89 && *(NextPtr + 2) == 0x05)
				{
					auto VarPtr = Memcury::Scanner(NextPtr).RelativeOffset(3).GetAs<CVarT**>();

					if (VarPtr && *VarPtr)
					{
						if (**VarPtr >= 0 && **VarPtr <= 1)
						{
							return *VarPtr;
						}
					}
				}
			}
		}

		return nullptr;
	}
};