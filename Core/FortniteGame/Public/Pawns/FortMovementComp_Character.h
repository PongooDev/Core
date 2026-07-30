#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

class AFortPawn;
class UCurveFloat;

class EFortAILODLevel {
public:
	DefineUnrealEnum(EFortAILODLevel);

	DefineEnumProperty(Invalid);
	DefineEnumProperty(Lowest);
	DefineEnumProperty(Lower);
	DefineEnumProperty(BelowNormal);
	DefineEnumProperty(Normal);
	DefineEnumProperty(AboveNormal);
	DefineEnumProperty(MAX);
};

struct FAirControlParams {
public:
	DefineUnrealStruct(FAirControlParams);

	DefineStructProperty(FScalableFloat, MaxAcceleration);
	DefineStructProperty(FScalableFloat, LateralFriction);
	DefineStructProperty(FScalableFloat, MaxLateralSpeed);
	DefineStructProperty(FScalableFloat, TerminalVelocity);
	DefineStructProperty(FScalableFloat, UpwardTerminalVelocity);
	DefineStructProperty(FScalableFloat, GravityScalar);
	DefineStructProperty(FScalableFloat, CustomGravityCeiling);
	DefineStructProperty(FScalableFloat, CustomGravityCeilingWidth);
public:
	uint8 Padding[0x100];
};

class UFortMovementComp_Character : public UCharacterMovementComponent {
public:
	DefineUnrealClass(UFortMovementComp_Character);

	DefineUProperty(float, LandHardSoundFallSpeedThreshold);
	DefineUProperty(float, LandSoundFallSpeedThreshold);
	DefineUProperty(TSubclassOf<AFortPawn>, PushBumpedPawnClass);
	DefineUProperty(float, NetworkSmoothingVisibilityThreshold);
	DefineUProperty(int32, PlayerLodRequiredForFloorCheckWhenRendered);
	DefineUProperty(EFortAILODLevel, AILodRequiredForFloorCheckWhenRendered);
	DefineUProperty(UCurveFloat*, VelocityBasedStrafeCurve);
	DefineUProperty(UCurveFloat*, VelocityBasedTurnCurve);
	DefineUProperty(UCurveFloat*, VelocityBasedBackupCurve);
	DefineUProperty(bool, bWasUsingVelocityBasedTurnCurve);
	DefineUProperty(float, RotationYawRateToRestore);
	DefineUProperty(FScalableFloat, FallingSlopeSafeSlideAngleScalable);
	DefineUProperty(FScalableFloat, FallingSlopeSafeSlideMaxSpeedScalable);
	DefineUProperty(FScalableFloat, FallingSlopeDamageScalarMinScalable);
	DefineUProperty(float, FallingSlopeSafeSlideAngleCached);
	DefineUProperty(float, FallingSlopeSafeSlideNormalZ);
	DefineUProperty(FAirControlParams, SkydivingControlParamsPassive);
	DefineUProperty(FAirControlParams, SkydivingControlParamsActive);
	DefineUProperty(FAirControlParams, ParachuteControlParamsSkydive);
	DefineUProperty(FAirControlParams, ParachuteControlParamsJump);
	DefineUProperty(FAirControlParams, ParachuteControlParamsGliderRedeploy);
	DefineUProperty(FAirControlParams, BalloonControlParams);
	DefineUProperty(FAirControlParams, VortexSkydivingControlParamsPassive);
	DefineUProperty(FAirControlParams, VortexSkydivingControlParamsActive);
	DefineUProperty(FAirControlParams, VortexSkydivingControlParamsActiveUpward);
	DefineUProperty(FAirControlParams, VortexParachuteControlParams);
	DefineUProperty(float, SkydivingMaxSmoothUpdateDistanceScale);
	DefineUProperty(float, SlideTimeUntilReset);
	DefineUProperty(float, SlideIfVelocityLessThanZ);
	DefineUProperty(int32, GravityCeilingRelation);
	DefineUProperty(float, CurrentZiplineVelocityDirection);
	DefineUProperty(float, GracePeriodToConnectToZipline);
	DefineUProperty(float, FallingStartedZ);
	DefineUProperty(FVector, ReplicatedAcceleration);
};
