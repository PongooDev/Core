#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

class AFortPawn;
class UCurveFloat;

class UFortMovementComp_Character : public UCharacterMovementComponent {
public:
	DefineUnrealClass(UFortMovementComp_Character);

	DefineUProperty(float, LandHardSoundFallSpeedThreshold);
	DefineUProperty(float, LandSoundFallSpeedThreshold);
	DefineUProperty(TSubclassOf<AFortPawn>, PushBumpedPawnClass);
	DefineUProperty(float, NetworkSmoothingVisibilityThreshold);
	DefineUProperty(int32, PlayerLodRequiredForFloorCheckWhenRendered);
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
	DefineUProperty(float, SkydivingMaxSmoothUpdateDistanceScale);
	DefineUProperty(float, SlideTimeUntilReset);
	DefineUProperty(float, SlideIfVelocityLessThanZ);
	DefineUProperty(int32, GravityCeilingRelation);
	DefineUProperty(float, CurrentZiplineVelocityDirection);
	DefineUProperty(float, GracePeriodToConnectToZipline);
	DefineUProperty(float, FallingStartedZ);
	DefineUProperty(FVector, ReplicatedAcceleration);
};
