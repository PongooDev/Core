#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/PrimaryAssetId.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

class UCurveFloat;
class UCurveVector;
class UCustomCharacterPart;
class UFortAbilitySet;
class UFortAnalyticsData;
class UFortAttributeCategory;
class UFortBuildingAudioBank;
class UFortDeathCauseData;
class UFortGamepadSettings;
class UFortGameplayTagData;
class UFortHeroData;
class UFortHeroType;
class UFortHomebaseManager;
class UFortInputData;
class UFortItemCategory;
class UFortItemDefinition;
class UFortLoadingConfig;
class UFortMusicManagerBank;
class UFortOutpostData;
class UFortRarityData;
class UFortScoreStylingData;
class UFortSurvivorData;
class UFortTimeOfDayData;
class UFortTokenToTextTable;
class UFortTokenType;
class UFortTooltipMap;
class UFortUIFeedbackBank;
class UMaterialInterface;
class UMyTownData;
class UParticleSystem;
class UPhysicalMaterial;
class USoundBase;
class USoundMix;
class UStaticMesh;

using TFPrimaryAssetIdToint32Map = TMap<FPrimaryAssetId, int32>;

class UEnvQuery;

struct FConditionalFoundationQuota {
public:
	DefineUnrealStruct(FConditionalFoundationQuota);

};

struct FItemDefinitionAndCount {
public:
	DefineUnrealStruct(FItemDefinitionAndCount);

	DefineStructProperty(int32, Count);
	DefineStructProperty(TSoftObjectPtr<UFortItemDefinition>, ItemDefinition);
public:
	uint8 Padding[0x30];
};

struct FFortPlayerSpawnPadPlacementData {
public:
	DefineUnrealStruct(FFortPlayerSpawnPadPlacementData);

	DefineStructProperty(UEnvQuery*, PlacementQuery);
	DefineStructProperty(TSoftClassPtr<UClass>, ActorToPlace);
	DefineStructProperty(bool, bSnapToGrid);
	DefineStructProperty(bool, bAdjustPlacementForFloors);
	DefineStructProperty(FGameplayTagContainer, TagsToAddToChosenPlacementActor);
public:
	uint8 Padding[0x58];
};

class ESlateVisibility {
public:
	DefineUnrealEnum(ESlateVisibility);

	DefineEnumProperty(Visible);
	DefineEnumProperty(Collapsed);
	DefineEnumProperty(Hidden);
	DefineEnumProperty(HitTestInvisible);
	DefineEnumProperty(SelfHitTestInvisible);
};

struct FSettingsHUDVisibilityAndText {
public:
	DefineUnrealStruct(FSettingsHUDVisibilityAndText);

	DefineStructProperty(FGameplayTag, HUDVisibilityGameplayTag);
	DefineStructProperty(ESlateVisibility, DefaultHUDVisibility);
	DefineStructProperty(FText, DisplayText);
	DefineStructProperty(FText, ToolTipText);
public:
	uint8 Padding[0x40];
};

struct FOnlineAccountTexts {
public:
	DefineUnrealStruct(FOnlineAccountTexts);

	DefineStructProperty(FText, AllGiftCodesUsed);
	DefineStructProperty(FText, AssociateConsoleAuth);
	DefineStructProperty(FText, AutoLoginFailed);
	DefineStructProperty(FText, AutoLoginFailedMobile);
	DefineStructProperty(FText, BannedFromGame);
	DefineStructProperty(FText, CheckEntitledToPlay);
	DefineStructProperty(FText, CheckingRejoin);
	DefineStructProperty(FText, CheckServiceAvailability);
	DefineStructProperty(FText, ConsolePrivileges);
	DefineStructProperty(FText, CreateAccountCompleted);
	DefineStructProperty(FText, CreateAccountFailure);
	DefineStructProperty(FText, CreateHeadless);
	DefineStructProperty(FText, DoQosPingTests);
	DefineStructProperty(FText, DowntimeMinutesWarningText);
	DefineStructProperty(FText, DowntimeSecondsWarningText);
	DefineStructProperty(FText, DuplicateAuthAssociaton);
	DefineStructProperty(FText, EulaCheck);
	DefineStructProperty(FText, ExchangeConsoleGiftsForAccess);
	DefineStructProperty(FText, ExchangeConsolePurchaseForAccess);
	DefineStructProperty(FText, FailedAccountCreate);
	DefineStructProperty(FText, FailedEulaCheck_EulaAcceptanceFailed);
	DefineStructProperty(FText, FailedEulaCheck_MustAcceptEula);
	DefineStructProperty(FText, FailedLoginCredentialsMsg);
	DefineStructProperty(FText, FailedLoginParentalLock);
	DefineStructProperty(FText, FailedLoginNoRealId);
	DefineStructProperty(FText, FailedLoginLockoutMsg);
	DefineStructProperty(FText, FailedLoginRequiresMFA);
	DefineStructProperty(FText, FailedLoginRequiresAuthAppMFA);
	DefineStructProperty(FText, FailedInvalidMFA);
	DefineStructProperty(FText, FailedLoginMsg);
	DefineStructProperty(FText, FailedLoginMsg_InvalidRefreshToken);
	DefineStructProperty(FText, FailedLoginTencent_UnableToSignIn);
	DefineStructProperty(FText, FailedLoginTencent_NotSignedInToWeGame);
	DefineStructProperty(FText, FailedLoginTencent_FailedToInitializeWeGame);
	DefineStructProperty(FText, FailedLoginTencent_WeGameSystemOffline);
	DefineStructProperty(FText, FailedStartLogin);
	DefineStructProperty(FText, FounderChatExitedText);
	DefineStructProperty(FText, FounderChatJoinedText);
	DefineStructProperty(FText, GameDisplayName);
	DefineStructProperty(FText, GeneralLoginFailure);
	DefineStructProperty(FText, GlobalChatExitedText);
	DefineStructProperty(FText, GlobalChatJoinedText);
	DefineStructProperty(FText, HeadlessAccountFailed);
	DefineStructProperty(FText, InMatchShutdownTimeWarningText);
	DefineStructProperty(FText, InvalidUser);
	DefineStructProperty(FText, LoggedOutofMCP);
	DefineStructProperty(FText, DisconnectedFromMCP);
	DefineStructProperty(FText, LoggedOutReturnedToTitle);
	DefineStructProperty(FText, LoggedOutSwitchedProfile);
	DefineStructProperty(FText, LoggingIn);
	DefineStructProperty(FText, LoggingInConsoleAuth);
	DefineStructProperty(FText, LoggingOut);
	DefineStructProperty(FText, LoginConsole);
	DefineStructProperty(FText, LoginFailure);
	DefineStructProperty(FText, Logout_Unlink);
	DefineStructProperty(FText, LogoutCompleted);
	DefineStructProperty(FText, LostConnection);
	DefineStructProperty(FText, MCPTimeout);
	DefineStructProperty(FText, LightswitchCheckNetworkFailureMsg);
	DefineStructProperty(FText, NetworkConnectionUnavailable);
	DefineStructProperty(FText, NoPlayEntitlement);
	DefineStructProperty(FText, NoServerAccess);
	DefineStructProperty(FText, PlayAccessRevoked);
	DefineStructProperty(FText, PremiumAccountName_Default);
	DefineStructProperty(FText, PremiumAccountName_PS4);
	DefineStructProperty(FText, PremiumAccountName_Switch);
	DefineStructProperty(FText, PremiumAccountName_XboxOne);
	DefineStructProperty(FText, RedeemOfflinePurchases);
	DefineStructProperty(FText, ServiceDowntime);
	DefineStructProperty(FText, SignInCompleting);
	DefineStructProperty(FText, SignIntoConsoleServices);
	DefineStructProperty(FText, TokenExpired);
	DefineStructProperty(FText, UnableToConnect);
	DefineStructProperty(FText, UnableToJoinWaitingRoomLoginQueue);
	DefineStructProperty(FText, UnexpectedConsoleAuthFailure);
	DefineStructProperty(FText, UnlinkConsoleFailed);
	DefineStructProperty(FText, UserLoginFailed);
	DefineStructProperty(FText, WaitingRoom);
	DefineStructProperty(FText, WaitingRoomError);
	DefineStructProperty(FText, WaitingRoomFailure);
	DefineStructProperty(FText, WaitingRoomWaiting);
	DefineStructProperty(FText, LoggingInExternalAuth);
	DefineStructProperty(FText, CreateDeviceAuth);
	DefineStructProperty(FText, ExtAuthCanceled);
	DefineStructProperty(FText, ExtAuthFailure);
	DefineStructProperty(FText, ExtAuthAssociationFailure);
	DefineStructProperty(FText, ExtAuthTimeout);
	DefineStructProperty(FText, ExtAuthMissingAuthAssociation);
	DefineStructProperty(FText, UnableToQueryReceipts);
public:
	uint8 Padding[0x990];
};

struct FPartnerPcbInfo {
public:
	DefineUnrealStruct(FPartnerPcbInfo);

	DefineStructProperty(FGameplayTag, PartnerTag);
	DefineStructProperty(FGameplayTagContainer, ValidPartnerBundles);
public:
	uint8 Padding[0x28];
};

struct FFortQuestPackInfo {
public:
	DefineUnrealStruct(FFortQuestPackInfo);

	DefineStructProperty(FString, Name);
	DefineStructProperty(FString, DefaultQuestPack);
	DefineStructProperty(int32, MaxActiveDailyQuests);
	DefineStructProperty(int32, MaxRerollsPerDay);
	DefineStructProperty(int32, DaysToKeepClaimedQuests);
	DefineStructProperty(int32, DaysToKeepCompletedQuests);
	DefineStructProperty(int32, MaxUnclaimedQuests);
	DefineStructProperty(bool, IsStreamingQuestPack);
public:
	uint8 Padding[0x35];
};

class EStatCategory {
public:
	DefineUnrealEnum(EStatCategory);

	DefineEnumProperty(Combat);
	DefineEnumProperty(Building);
	DefineEnumProperty(Utility);
	DefineEnumProperty(Max_None);
};

struct FFortReplicatedStatMapping {
public:
	DefineUnrealStruct(FFortReplicatedStatMapping);

	DefineStructProperty(EStatCategory, StatCategory);
	DefineStructProperty(FText, DisplayName);
public:
	uint8 Padding[0x20];
};

class UFortGameData : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortGameData);

	DefineUProperty(FText, DefaultHeroName);
	DefineUProperty(float, PickupSplineRandomMax);
	DefineUProperty(float, PickupSplineDropToGroundLength);
	DefineUProperty(float, PickupMaxCollectionTime);
	DefineUProperty(float, PickupMaxDelayPerItem);
	DefineUProperty(TArray<FName>, AutoEquipTags);
	DefineUProperty(FGameplayTagContainer, AutoEquipTagContainer);
	DefineUProperty(FLinearColor, BuildingFullHealthFlashColor);
	DefineUProperty(FLinearColor, InvalidEditPatternColor);
	DefineUProperty(float, ScoreDisplayFactor);
	DefineUProperty(TArray<FFortReplicatedStatMapping>, StatMappings);
	DefineUProperty(int32, ScoreDivisor);
	DefineUProperty(int32, XPMult);
	DefineUProperty(float, ScoreToXPLinearRate);
	DefineUProperty(int32, LinearEnd);
	DefineUProperty(int32, XpPerAccountLevel);
	DefineUProperty(float, CriticalMatch_XpBonusPercent);
	DefineUProperty(float, PersonalBoost_XpBonusPercent);
	DefineUProperty(float, GroupBoost_XpBonusPercent);
	DefineUProperty(float, GroupBoost_BuffMultiplier);
	DefineUProperty(float, Rest_XpBonusPercent);
	DefineUProperty(FVector, MobileInteractionIconScale);
	DefineUProperty(FCurveTableRowHandle, CritRatingTable);
	DefineUProperty(FCurveTableRowHandle, ArmorToDamageResistanceTable);
	DefineUProperty(float, LowXpConningValue);
	DefineUProperty(float, VeryLowXpConningValue);
	DefineUProperty(float, NoXpConningValue);
	DefineUProperty(FText, FullXpConningMsg);
	DefineUProperty(FText, LowXpConningMsg);
	DefineUProperty(FText, VeryLowXpConningMsg);
	DefineUProperty(FText, NoXpConningMsg);
	DefineUProperty(int32, MaxCraftQueueSize);
	DefineUProperty(float, DefaultLootInstancingRange);
	DefineUProperty(TArray<FFortQuestPackInfo>, QuestPackInfos);
	DefineUProperty(FDataTableRowHandle, OnboardingCompleteZoneObjective);
	DefineUProperty(FGameplayTagContainer, DamageDoneTargetQuest_AllowedTags);
	DefineUProperty(FGameplayTagContainer, DamageDoneQuest_AllowedTags);
	DefineUProperty(FGameplayTagContainer, DamageDoneTargetQuest_AllowedTags_Old);
	DefineUProperty(FGameplayTagContainer, DamageDoneQuest_AllowedTags_Old);
	DefineUProperty(FConditionalFoundationQuota, ConditionalFoundationQuota);
	DefineUProperty(FCurveTableRowHandle, PowerPointToRatingConversion);
	DefineUProperty(FDataTableRowHandle, EventMovieQuestObjective);
	DefineUProperty(TArray<FSettingsHUDVisibilityAndText>, HUDVisibilityGameplayTags);
	DefineUProperty(FDataTableRowHandle, GameDifficultyRowPIE);
	DefineUProperty(int32, AIDirectorIndex);
	DefineUProperty(int32, AlternateBalanceAIDirectorIndex);
	DefineUProperty(TArray<FItemDefinitionAndCount>, DefaultInventoryList);
	DefineUProperty(TArray<FItemDefinitionAndCount>, FastLoadDefaultInventoryList);
	DefineUProperty(float, SquadMemberStatBonusMultiplier);
	DefineUProperty(TSoftObjectPtr<USoundBase>, PlayerIdleWarningSound);
	DefineUProperty(FGameplayTag, ExpeditionUnlockedTag);
	DefineUProperty(FGameplayTagContainer, ExpeditionVehicleTags);
	DefineUProperty(int32, DailyMissionAlertQuota);
	DefineUProperty(FOnlineAccountTexts, OnlineAccountTexts);
	DefineUProperty(TArray<FName>, ServerOnlyAssetTypes);
	DefineUProperty(TSoftObjectPtr<UFortLoadingConfig>, LoadingConfig);
	DefineUProperty(TSoftClassPtr<UClass>, TestEncounter);
	DefineUProperty(TSoftClassPtr<UClass>, GameModeAthenaClass);
	DefineUProperty(TSoftClassPtr<UClass>, PlaylistManagerClass);
	DefineUProperty(TSoftClassPtr<UClass>, CombatManager);
	DefineUProperty(TSoftClassPtr<UClass>, DefaultCameraManager);
	DefineUProperty(TSoftClassPtr<UClass>, UIMapManager);
	DefineUProperty(TSoftClassPtr<UClass>, UIMapManagerAthena);
	DefineUProperty(TSoftClassPtr<UClass>, MusicManager);
	DefineUProperty(TSoftObjectPtr<UFortMusicManagerBank>, MusicManagerBank);
	DefineUProperty(TSoftObjectPtr<UFortAnalyticsData>, AnalyticsData);
	DefineUProperty(TSoftObjectPtr<UFortGameplayTagData>, GameplayTagData);
	DefineUProperty(TSoftObjectPtr<UFortTooltipMap>, ObjClassToTooltipClassMap);
	DefineUProperty(TSoftClassPtr<UClass>, DefaultDamageReceiverClass);
	DefineUProperty(TSoftClassPtr<UClass>, DefaultDamageReceiverClassAthena);
	DefineUProperty(TSoftObjectPtr<UFortTokenToTextTable>, TooltipTokenToTextTable);
	DefineUProperty(TSoftObjectPtr<UCurveVector>, PickupSplineCurve);
	DefineUProperty(TSoftObjectPtr<USoundBase>, PickupLandedSound);
	DefineUProperty(TSoftObjectPtr<UFortTokenType>, AccountInventoryBonusToken);
	DefineUProperty(TSoftObjectPtr<UFortTokenType>, WorldInventoryBonusToken);
	DefineUProperty(TSoftObjectPtr<UFortTokenType>, FounderChatUnlockToken);
	DefineUProperty(TSoftObjectPtr<UFortRarityData>, RarityData);
	DefineUProperty(TSoftObjectPtr<UFortScoreStylingData>, ScoreStylingData);
	DefineUProperty(TSoftObjectPtr<UFortItemCategory>, ItemCategories);
	DefineUProperty(TSoftObjectPtr<UDataTable>, ScoreMultiplierDataTable);
	DefineUProperty(UDataTable*, CachedScoreMultiplierDataTable);
	DefineUProperty(TSoftClassPtr<UClass>, AppActivationSoundMixManager);
	DefineUProperty(TSoftObjectPtr<USoundMix>, LoadingScreenSoundMix);
	DefineUProperty(TSoftObjectPtr<USoundMix>, BaseSoundMix);
	DefineUProperty(TSoftClassPtr<UClass>, AbilityTargetingActorBlueprintClass);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, MobileInteractionMaterial);
	DefineUProperty(TSoftObjectPtr<UStaticMesh>, MobileInteractionMesh);
	DefineUProperty(FGameplayTagContainer, PlacementActorUsedTags);
	DefineUProperty(FFortPlayerSpawnPadPlacementData, DefaultPlayerSpawnPadPlacementData);
	DefineUProperty(TSoftObjectPtr<UCustomCharacterPart>, DefaultBackpackCharacterPart);
	DefineUProperty(TSoftClassPtr<UClass>, AIGoalManagerClass);
	DefineUProperty(TSoftClassPtr<UClass>, SharedMissionListsClass);
	DefineUProperty(FCurveTableRowHandle, WeakSpotEnabledTimeoutHandle);
	DefineUProperty(float, BuildingStructuralCollapseDelay);
	DefineUProperty(float, BuildingStructuralCollapseDelayVariance);
	DefineUProperty(float, BuildingStructuralCollapseCellDistAdditiveDelay);
	DefineUProperty(float, BuildingStructuralCollapseCellDistAdditiveDelayVariance);
	DefineUProperty(float, EditModeCancelDistance);
	DefineUProperty(FCurveTableRowHandle, BuildingExcessDamageRewardTimeoutHandle);
	DefineUProperty(float, FORTAttributeToPowerMultiplier);
	DefineUProperty(TSoftObjectPtr<UFortBuildingAudioBank>, BuildingAudioBank);
	DefineUProperty(TSoftObjectPtr<UFortItemDefinition>, CriticalMissionRewardItemDefinition);
	DefineUProperty(TSoftObjectPtr<UFortItemDefinition>, ScoreBonusMultiplerItemDefinition);
	DefineUProperty(TSoftObjectPtr<UFortItemDefinition>, GenericCampaignEventCurrency);
	DefineUProperty(FText, OutpostRefundDialogTitle);
	DefineUProperty(FText, OutpostRefundDialogText);
	DefineUProperty(FText, OutpostRefundButtonText);
	DefineUProperty(TSoftObjectPtr<UFortInputData>, FortInputData);
	DefineUProperty(TSoftObjectPtr<UFortTimeOfDayData>, FortTimeOfDayData);
	DefineUProperty(TSoftObjectPtr<UFortHeroData>, FortHeroData);
	DefineUProperty(TSoftObjectPtr<UFortGamepadSettings>, GamepadSettingsDefault);
	DefineUProperty(TSoftObjectPtr<UFortGamepadSettings>, GamepadSettingsPS4);
	DefineUProperty(TSoftObjectPtr<UFortGamepadSettings>, GamepadSettingsXboxOne);
	DefineUProperty(TSoftObjectPtr<UFortGamepadSettings>, GamepadSettingsSwitch);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationMigrationMapping);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationIntrinsicMapping);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationNamedExclusions);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationGroups);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationSlotDefinitions);
	DefineUProperty(TSoftObjectPtr<UDataTable>, AlterationSlotsLoadouts);
	DefineUProperty(TArray<TSoftObjectPtr<UDataTable>>, LootTierDataTables);
	DefineUProperty(TArray<TSoftObjectPtr<UDataTable>>, LootPackageDataTables);
	DefineUProperty(TSoftObjectPtr<UDataTable>, WaveSetCollectionData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, WaveSetData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, ModifierSetData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, WaveSetCollectionLootData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, PlayerStatClampingData);
	DefineUProperty(FGameplayTagContainer, NonGameplayRelevantTags);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, CraftingTimeTable);
	DefineUProperty(TSoftObjectPtr<USoundBase>, DefaultCraftingSuccessSound);
	DefineUProperty(TSoftClassPtr<UClass>, TeamStatsGameplayEffect);
	DefineUProperty(TSoftObjectPtr<UDataTable>, ExpeditionCriteriaRequirementsData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, DifficultyData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, LootQuotaData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, LootLevelData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, TagQueryTable);
	DefineUProperty(TSoftObjectPtr<UDataTable>, ItemToQuestDataTable);
	DefineUProperty(TSoftObjectPtr<UDataTable>, SurfaceRatioBySurfaceCategoryData);
	DefineUProperty(UDataTable*, CachedSurfaceRatioBySurfaceCategoryData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, SurfaceRatioByAffiliationData);
	DefineUProperty(UDataTable*, CachedSurfaceRatioByAffiliationData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, QuestObjectiveStatData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, XPAccountLevelsTable);
	DefineUProperty(TSoftObjectPtr<UDataTable>, PostMaxAccountLevelRewardsTable);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, XPAccountItemLevelsTable);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, XPAccountItemRewardsTable);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, XPConningTable);
	DefineUProperty(TArray<FPartnerPcbInfo>, PartnerCafeBundleMapping);
	DefineUProperty(TSoftObjectPtr<UDataTable>, QuestAchievementData);
	DefineUProperty(TSoftObjectPtr<UFortItemDefinition>, SkillPointItemDef);
	DefineUProperty(TSoftObjectPtr<UFortItemDefinition>, ResearchPointItemDef);
	DefineUProperty(TSoftObjectPtr<UDataTable>, StatNamesToTrackTable);
	DefineUProperty(TSoftClassPtr<UClass>, TheaterMapViewerClass);
	DefineUProperty(TSoftObjectPtr<UDataTable>, MissionAlertCategories);
	DefineUProperty(TSoftObjectPtr<UMyTownData>, MyFortData);
	DefineUProperty(TSoftObjectPtr<UFortHomebaseManager>, HomebaseManager);
	DefineUProperty(TSoftObjectPtr<UFortSurvivorData>, SurvivorData);
	DefineUProperty(TSoftObjectPtr<UFortOutpostData>, OutpostData);
	DefineUProperty(TSoftObjectPtr<UFortAttributeCategory>, AttributeCategories);
	DefineUProperty(TSoftClassPtr<UClass>, DamageNumbersActorClass);
	DefineUProperty(TSoftClassPtr<UClass>, WeakSpotBlueprintClass);
	DefineUProperty(TSoftObjectPtr<UPhysicalMaterial>, WeakSpotWoodPhysicalMaterial);
	DefineUProperty(TSoftObjectPtr<UPhysicalMaterial>, WeakSpotStonePhysicalMaterial);
	DefineUProperty(TSoftObjectPtr<UPhysicalMaterial>, WeakSpotMetalPhysicalMaterial);
	DefineUProperty(TSoftClassPtr<UClass>, PickupEffectBlueprintClass);
	DefineUProperty(TArray<TSoftClassPtr<UClass>>, AIDirectors);
	DefineUProperty(TSoftClassPtr<UClass>, FeedbackManagerClass);
	DefineUProperty(TSoftClassPtr<UClass>, FeedbackManagerClassAthena);
	DefineUProperty(TSoftClassPtr<UClass>, BluGloManagerClass);
	DefineUProperty(TSoftObjectPtr<UFortUIFeedbackBank>, UIFeedbackBank);
	DefineUProperty(TSoftObjectPtr<UFortUIFeedbackBank>, AthenaUIFeedbackBank);
	DefineUProperty(TSoftClassPtr<UClass>, StatEventManagerClass);
	DefineUProperty(TSoftObjectPtr<UFortHeroType>, DefaultHero);
	DefineUProperty(TSoftObjectPtr<UFortHeroType>, DefaultAthenaHero);
	DefineUProperty(TSoftClassPtr<UClass>, ThreatVisualsManager);
	DefineUProperty(TSoftClassPtr<UClass>, WindManager);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, GenericPlayerAbilitySet);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, GenericTrapAbilitySet);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, GenericVehicleAbilitySet);
	DefineUProperty(TSoftObjectPtr<UFortAbilitySet>, AthenaPlayerAbilitySet);
	DefineUProperty(TSoftObjectPtr<UFortDeathCauseData>, FortDeathCauseData);
	DefineUProperty(TSoftClassPtr<UClass>, EnvironmentDamageTemplateGE);
	DefineUProperty(TSoftClassPtr<UClass>, FallingDamageTemplateGE);
	DefineUProperty(TSoftClassPtr<UClass>, VehicleImpactDamageTemplateGE);
	DefineUProperty(TSoftClassPtr<UClass>, HealingTemplateGE);
	DefineUProperty(TSoftClassPtr<UClass>, FatalDamageTemplateGE);
	DefineUProperty(TSoftClassPtr<UClass>, SquadMemberStatBonusGE);
	DefineUProperty(TSoftClassPtr<UClass>, TeamTrapBonusGE);
	DefineUProperty(TSoftClassPtr<UClass>, BASEClass);
	DefineUProperty(TSoftClassPtr<UClass>, EmoteGameplayAbility);
	DefineUProperty(TSoftClassPtr<UClass>, SprayGameplayAbility);
	DefineUProperty(TSoftClassPtr<UClass>, QuestNotificationHandlerClass);
	DefineUProperty(TSoftClassPtr<UClass>, BasicNotificationClass);
	DefineUProperty(TSoftClassPtr<UClass>, FriendNotificationClass);
	DefineUProperty(TSoftClassPtr<UClass>, TwitchNotificationClass);
	DefineUProperty(TSoftClassPtr<UClass>, MissionVoteNotificationClass);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, HealthScreenDamagePostProcessMat);
	DefineUProperty(TSoftClassPtr<UClass>, ZoneModifiersAnnouncementClass);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, SessionXPCurveTable);
	DefineUProperty(TSoftObjectPtr<UCurveTable>, TieredWavesDefenseReqTable);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, FallbackDeathParticles);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, WallFallbackDeathParticles);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, FallbackConstructedEffect);
	DefineUProperty(TSoftObjectPtr<UParticleSystem>, RepairEffect);
	DefineUProperty(TSoftObjectPtr<USoundBase>, ConstructionCompleteSound);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, DamageMaterial);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, BounceCurve);
	DefineUProperty(TSoftObjectPtr<UMaterialInterface>, BlueprintParentMaterial);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, EditCurve);
	DefineUProperty(TSoftObjectPtr<UCurveVector>, BlueprintDamageCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, FullHealthAnimCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, RepairAnimCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, DynamicLODEffectCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, InteractFullBounceCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, InteractFullBounceNormalCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, InteractEmptyBounceCurve);
	DefineUProperty(TSoftObjectPtr<UCurveFloat>, InteractEmptyBounceNormalCurve);
	DefineUProperty(TSoftClassPtr<UClass>, ExpeditionMasterWidgetClass);
	DefineUProperty(TSoftObjectPtr<UDataTable>, BannerIconData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, BannerIconCategoryData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, BannerColorData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, BannerColorCategoryData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, CreativeInventoryTable);
	DefineUProperty(TSoftClassPtr<UClass>, DefaultRebootVanIndicatorClass);
	DefineUProperty(TFPrimaryAssetIdToint32Map, CachedCreativeInventory);
};
