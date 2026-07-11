#pragma once
#include "pch.h"

struct  FVector;
struct  FVector4;
struct  FPlane;
struct  FBox;
struct  FRotator;
struct  FMatrix;
struct  FQuat;
struct  FTwoVectors;
struct  FTransform;
class  FSphere;
struct FVector2D;
struct FLinearColor;
class TRange;

/*-----------------------------------------------------------------------------
	Floating point constants.
-----------------------------------------------------------------------------*/

#undef  PI
#define PI 					(3.1415926535897932f)	/* Extra digits if needed: 3.1415926535897932384626433832795f */
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)
#define EULERS_NUMBER       (2.71828182845904523536f)
#define UE_GOLDEN_RATIO		(1.6180339887498948482045868343656381f)	/* Also known as divine proportion, golden mean, or golden section - related to the Fibonacci Sequence = (1 + sqrt(5)) / 2 */
#define FLOAT_NON_FRACTIONAL (8388608.f) /* All single-precision floating point numbers greater than or equal to this have no fractional value. */

// Copied from float.h
#define MAX_FLT 3.402823466e+38F

// Aux constants.
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

// Common square roots
#define UE_SQRT_2		(1.4142135623730950488016887242097f)
#define UE_SQRT_3		(1.7320508075688772935274463415059f)
#define UE_INV_SQRT_2	(0.70710678118654752440084436210485f)
#define UE_INV_SQRT_3	(0.57735026918962576450914878050196f)
#define UE_HALF_SQRT_2	(0.70710678118654752440084436210485f)
#define UE_HALF_SQRT_3	(0.86602540378443864676372317075294f)


// Magic numbers for numerical precision.
#define DELTA			(0.00001f)

/**
 * Lengths of normalized vectors (These are half their maximum values
 * to assure that dot products with normalized vectors don't overflow).
 */
#define FLOAT_NORMAL_THRESH				(0.0001f)

 //
 // Magic numbers for numerical precision.
 //
#define THRESH_POINT_ON_PLANE			(0.10f)		/* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE			(0.20f)		/* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME			(0.00002f)	/* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR			(0.015f)	/* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME			(0.00002f)	/* Two normal points are same if within this distance */
#define THRESH_UVS_ARE_SAME			    (0.0009765625f)/* Two UV are same if within this threshold (1.0f/1024f) */
													/* Making this too large results in incorrect CSG classification and disaster */
#define THRESH_VECTORS_ARE_NEAR			(0.0004f)	/* Two vectors are near if within this distance and can be combined if imprecise math is ok */
													/* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define THRESH_SPLIT_POLY_WITH_PLANE	(0.25f)		/* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY		(0.01f)		/* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED		(0.0001f)	/* Size of a unit normal that is considered "zero", squared */
#define THRESH_NORMALS_ARE_PARALLEL		(0.999845f)	/* Two unit vectors are parallel if abs(A dot B) is greater than or equal to this. This is roughly cosine(1.0 degrees). */
#define THRESH_NORMALS_ARE_ORTHOGONAL	(0.017455f)	/* Two unit vectors are orthogonal (perpendicular) if abs(A dot B) is less than or equal this. This is roughly cosine(89.0 degrees). */

#define THRESH_VECTOR_NORMALIZED		(0.01f)		/** Allowed error for a normalized vector (against squared magnitude) */
#define THRESH_QUAT_NORMALIZED			(0.01f)		/** Allowed error for a normalized quaternion (against squared magnitude) */

/*-----------------------------------------------------------------------------
	FMath - minimal native port of Epic's FGenericPlatformMath / FMath scalar
	primitives (GenericPlatformMath.h / UnrealMathUtility.h / UnrealMath.cpp).

	Unlike most of this codebase, these functions are NOT gated behind
	Version::Engine_Version checks. Epic has kept this exact layer -- the
	SRand LCG constants, RoundToInt's rounding rule, RandHelper, Clamp/Lerp --
	byte-identical from UE4.0 through current UE5, specifically because so
	much downstream content (replication jitter, procedural generation seeds,
	particle systems) depends on a given seed producing the same sequence.
	The only related engine change across that span is UE5's large-world-
	coordinates move to double-precision FVector/FRotator, which added new
	vector/double overloads but did not alter these int32/float scalar ones.
	So there is nothing to branch on here -- if a future engine version is
	ever found to actually diverge, gate it the same way the rest of this
	file does, but don't add a branch that doesn't correspond to a real
	documented engine change.
-----------------------------------------------------------------------------*/

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>

// This header is pulled in before HAL/Platform.h defines the engine int types;
// re-declare the one we need (identical typedefs are legal to repeat).
typedef int32_t int32;

struct FMath
{
	static FORCEINLINE int32 TruncToInt(float F) { return (int32)F; }
	static FORCEINLINE float TruncToFloat(float F) { return (float)TruncToInt(F); }
	static FORCEINLINE int32 FloorToInt(float F) { return TruncToInt(floorf(F)); }
	static FORCEINLINE int32 RoundToInt(float F) { return FloorToInt(F + 0.5f); }
	static FORCEINLINE float Fractional(float Value) { return Value - TruncToFloat(Value); }

	template<class T> static FORCEINLINE T Min(const T A, const T B) { return (A <= B) ? A : B; }
	template<class T> static FORCEINLINE T Max(const T A, const T B) { return (A >= B) ? A : B; }
	template<class T> static FORCEINLINE T Clamp(const T X, const T Min, const T Max) { return X < Min ? Min : X < Max ? X : Max; }
	template<class T, class U> static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha) { return (T)(A + Alpha * (B - A)); }

	// FGenericPlatformMath::Rand/FRand - CRT-based, seeded once (the engine seeds these
	// at startup with FPlatformTime::Cycles(); we seed at static init instead).
	static inline const bool bRandSeeded = ([] { srand((unsigned int)time(nullptr)); return true; })();
	static FORCEINLINE void RandInit(int32 Seed) { srand(Seed); }
	static FORCEINLINE int32 Rand() { return rand(); }
	static FORCEINLINE float FRand() { return Rand() / (float)RAND_MAX; }

	// FMath::SRand - seeded LCG, bit-exact port of UnrealMath.cpp
	static inline int32 GSRandSeed = (int32)time(nullptr);
	static FORCEINLINE void SRandInit(int32 Seed) { GSRandSeed = Seed; }
	static FORCEINLINE int32 GetRandSeed() { return GSRandSeed; }
	static float SRand()
	{
		GSRandSeed = (GSRandSeed * 196314165) + 907633515;
		union { float f; int32 i; } Result;
		union { float f; int32 i; } Temp;
		const float SRandTemp = 1.0f;
		Temp.f = SRandTemp;
		Result.i = (Temp.i & 0xff800000) | (GSRandSeed & 0x007fffff);
		return Fractional(Result.f);
	}

	static FORCEINLINE int32 RandHelper(int32 A) { return A > 0 ? Min(TruncToInt(FRand() * A), A - 1) : 0; }
	static FORCEINLINE int32 RandRange(int32 Min, int32 Max) { const int32 Range = (Max - Min) + 1; return Min + RandHelper(Range); }
	static FORCEINLINE float FRandRange(float InMin, float InMax) { return InMin + (InMax - InMin) * FRand(); }
};