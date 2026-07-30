#pragma once
#include "pch.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <intrin.h>

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


typedef int32_t int32;

struct FMath
{
	static FORCEINLINE int32 TruncToInt(float F) { return (int32)F; }
	static FORCEINLINE float TruncToFloat(float F) { return (float)TruncToInt(F); }
	static FORCEINLINE int32 FloorToInt(float F) { return TruncToInt(floorf(F)); }
	static FORCEINLINE float FloorToFloat(float F) { return floorf(F); }
	static FORCEINLINE int32 CeilToInt(float F) { return TruncToInt(ceilf(F)); }
	static FORCEINLINE float CeilToFloat(float F) { return ceilf(F); }
	static FORCEINLINE int32 RoundToInt(float F) { return FloorToInt(F + 0.5f); }
	static FORCEINLINE float RoundToFloat(float F) { return FloorToFloat(F + 0.5f); }
	static FORCEINLINE float Fractional(float Value) { return Value - TruncToFloat(Value); }
	static FORCEINLINE float Frac(float Value) { return Value - FloorToFloat(Value); }
	static FORCEINLINE float Modf(const float InValue, float* OutIntPart) { return modff(InValue, OutIntPart); }

	template<class T> static FORCEINLINE T Min(const T A, const T B) { return (A <= B) ? A : B; }
	template<class T> static FORCEINLINE T Max(const T A, const T B) { return (A >= B) ? A : B; }
	template<class T> static FORCEINLINE T Min3(const T A, const T B, const T C) { return Min(Min(A, B), C); }
	template<class T> static FORCEINLINE T Max3(const T A, const T B, const T C) { return Max(Max(A, B), C); }
	template<class T> static FORCEINLINE T Clamp(const T X, const T Min, const T Max) { return X < Min ? Min : X < Max ? X : Max; }
	template<class T, class U> static FORCEINLINE T Lerp(const T& A, const T& B, const U& Alpha) { return (T)(A + Alpha * (B - A)); }

	template<class T> static FORCEINLINE T Abs(const T A) { return (A >= (T)0) ? A : -A; }
	static FORCEINLINE float Abs(const float A) { return fabsf(A); }
	template<class T> static FORCEINLINE T Sign(const T A) { return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0); }
	template<class T> static FORCEINLINE T Square(const T A) { return A * A; }
	template<class T> static FORCEINLINE T Cube(const T A) { return A * A * A; }

	static FORCEINLINE float Sqrt(float Value) { return sqrtf(Value); }
	static FORCEINLINE float Pow(float A, float B) { return powf(A, B); }
	static FORCEINLINE float Exp(float Value) { return expf(Value); }
	static FORCEINLINE float Exp2(float Value) { return powf(2.f, Value); }
	static FORCEINLINE float Loge(float Value) { return logf(Value); }
	static FORCEINLINE float LogX(float Base, float Value) { return Loge(Value) / Loge(Base); }
	static FORCEINLINE float Log2(float Value) { return Loge(Value) * 1.4426950f; }

	static FORCEINLINE float Sin(float Value) { return sinf(Value); }
	static FORCEINLINE float Cos(float Value) { return cosf(Value); }
	static FORCEINLINE float Tan(float Value) { return tanf(Value); }
	static FORCEINLINE float Asin(float Value) { return asinf(Value < -1.f ? -1.f : (Value < 1.f ? Value : 1.f)); }
	static FORCEINLINE float Acos(float Value) { return acosf(Value < -1.f ? -1.f : (Value < 1.f ? Value : 1.f)); }
	static FORCEINLINE float Atan(float Value) { return atanf(Value); }

	static float Atan2(float Y, float X)
	{
		const float absX = Abs(X);
		const float absY = Abs(Y);
		const bool yAbsBigger = (absY > absX);
		float t0 = yAbsBigger ? absY : absX;
		float t1 = yAbsBigger ? absX : absY;

		if (t0 == 0.f)
			return 0.f;

		float t3 = t1 / t0;
		float t4 = t3 * t3;

		static const float c[7] = {
			+7.2128853633444123e-03f,
			-3.5059680836411644e-02f,
			+8.1675882859940430e-02f,
			-1.3374657325451267e-01f,
			+1.9856563505717162e-01f,
			-3.3324998579202170e-01f,
			+1.0f
		};

		float t2 = c[0];
		t2 = t2 * t4 + c[1];
		t2 = t2 * t4 + c[2];
		t2 = t2 * t4 + c[3];
		t2 = t2 * t4 + c[4];
		t2 = t2 * t4 + c[5];
		t2 = t2 * t4 + c[6];
		t3 = t2 * t3;

		t3 = yAbsBigger ? (0.5f * PI) - t3 : t3;
		t3 = (X < 0.0f) ? PI - t3 : t3;
		t3 = (Y < 0.0f) ? -t3 : t3;

		return t3;
	}

	static FORCEINLINE float FastAsin(float Value)
	{
		bool nonnegative = (Value >= 0.0f);
		float x = Abs(Value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = Sqrt(omx);
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + 1.5707963050f;
		result *= root;
		return (nonnegative ? HALF_PI - result : result - HALF_PI);
	}

	static void SinCos(float* ScalarSin, float* ScalarCos, float Value)
	{
		float quotient = (INV_PI * 0.5f) * Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int32)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int32)(quotient - 0.5f));
		}
		float y = Value - (2.0f * PI) * quotient;

		float sign;
		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}

	static FORCEINLINE float InvSqrt(float F)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(F);
		X0 = _mm_rsqrt_ss(Y0);
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}

	static FORCEINLINE float InvSqrtEst(float F)
	{
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, FOver2;
		float temp;

		Y0 = _mm_set_ss(F);
		X0 = _mm_rsqrt_ss(Y0);
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		_mm_store_ss(&temp, X1);
		return temp;
	}

	static FORCEINLINE float Fmod(float X, float Y)
	{
		const float AbsY = Abs(Y);
		if (AbsY <= 1.e-8f)
		{
			return 0.f;
		}
		const float Div = (X / Y);
		const float Quotient = Abs(Div) < FLOAT_NON_FRACTIONAL ? TruncToFloat(Div) : Div;
		float IntPortion = Y * Quotient;
		if (Abs(IntPortion) > Abs(X))
		{
			IntPortion = X;
		}
		const float Result = X - IntPortion;
		return Clamp(Result, -AbsY, AbsY);
	}

	static FORCEINLINE bool IsNearlyEqual(float A, float B, float ErrorTolerance = SMALL_NUMBER) { return Abs(A - B) <= ErrorTolerance; }
	static FORCEINLINE bool IsNearlyZero(float Value, float ErrorTolerance = SMALL_NUMBER) { return Abs(Value) <= ErrorTolerance; }
	static FORCEINLINE bool IsPowerOfTwo(uint32_t Value) { return ((Value & (Value - 1)) == 0); }
	static FORCEINLINE bool IsFinite(float Value) { return isfinite(Value) != 0; }
	static FORCEINLINE bool IsNaN(float Value) { return isnan(Value) != 0; }

	static FORCEINLINE float FloatSelect(float Comparand, float ValueGEZero, float ValueLTZero) { return Comparand >= 0.f ? ValueGEZero : ValueLTZero; }

	static FORCEINLINE float GridSnap(float Location, float Grid)
	{
		if (Grid == 0.f) return Location;
		return FloorToFloat((Location + (Grid / 2.f)) / Grid) * Grid;
	}

	template <class T> static FORCEINLINE T DivideAndRoundUp(T Dividend, T Divisor) { return (Dividend + Divisor - 1) / Divisor; }
	template <class T> static FORCEINLINE T DivideAndRoundDown(T Dividend, T Divisor) { return Dividend / Divisor; }

	template<class T> static FORCEINLINE auto DegreesToRadians(T const& DegVal) -> decltype(DegVal * (PI / 180.f)) { return DegVal * (PI / 180.f); }
	template<class T> static FORCEINLINE auto RadiansToDegrees(T const& RadVal) -> decltype(RadVal * (180.f / PI)) { return RadVal * (180.f / PI); }

	static float UnwindDegrees(float A)
	{
		while (A > 180.f) { A -= 360.f; }
		while (A < -180.f) { A += 360.f; }
		return A;
	}

	static float UnwindRadians(float A)
	{
		while (A > PI) { A -= ((float)PI * 2.0f); }
		while (A < -PI) { A += ((float)PI * 2.0f); }
		return A;
	}

	static FORCEINLINE float FindDeltaAngleDegrees(float A1, float A2)
	{
		float Delta = A2 - A1;
		if (Delta > 180.0f) { Delta = Delta - 360.0f; }
		else if (Delta < -180.0f) { Delta = Delta + 360.0f; }
		return Delta;
	}

	static FORCEINLINE float FindDeltaAngleRadians(float A1, float A2)
	{
		float Delta = A2 - A1;
		if (Delta > PI) { Delta = Delta - (PI * 2.0f); }
		else if (Delta < -PI) { Delta = Delta + (PI * 2.0f); }
		return Delta;
	}

	static float ClampAngle(float AngleDegrees, float MinAngleDegrees, float MaxAngleDegrees)
	{
		const float MaxDelta = FindDeltaAngleDegrees(MinAngleDegrees, MaxAngleDegrees) * 0.5f;
		const float RangeCenter = MinAngleDegrees + MaxDelta;
		const float DeltaFromCenter = FindDeltaAngleDegrees(RangeCenter, AngleDegrees);

		if (DeltaFromCenter > MaxDelta)
		{
			return RangeCenter + MaxDelta;
		}
		else if (DeltaFromCenter < -MaxDelta)
		{
			return RangeCenter - MaxDelta;
		}

		return AngleDegrees;
	}

	static FORCEINLINE float GetRangePct(float MinValue, float MaxValue, float Value)
	{
		const float Divisor = MaxValue - MinValue;
		if (IsNearlyZero(Divisor))
		{
			return (Value >= MaxValue) ? 1.f : 0.f;
		}
		return (Value - MinValue) / Divisor;
	}

	template<typename T, typename U>
	static FORCEINLINE T GetRangeValue(const U& Range, float Pct)
	{
		return Lerp<T>(Range.X, Range.Y, Pct);
	}

	static FORCEINLINE float GetMappedRangeValueClamped(const float(&InputRange)[2], const float(&OutputRange)[2], float Value)
	{
		const float ClampedPct = Clamp(GetRangePct(InputRange[0], InputRange[1], Value), 0.f, 1.f);
		return Lerp(OutputRange[0], OutputRange[1], ClampedPct);
	}

	static float FInterpTo(float Current, float Target, float DeltaTime, float InterpSpeed)
	{
		if (InterpSpeed <= 0.f)
		{
			return Target;
		}

		const float Dist = Target - Current;

		if (Square(Dist) < SMALL_NUMBER)
		{
			return Target;
		}

		const float DeltaMove = Dist * Clamp<float>(DeltaTime * InterpSpeed, 0.f, 1.f);
		return Current + DeltaMove;
	}

	static float FInterpConstantTo(float Current, float Target, float DeltaTime, float InterpSpeed)
	{
		const float Dist = Target - Current;

		if (Square(Dist) < SMALL_NUMBER)
		{
			return Target;
		}

		const float Step = InterpSpeed * DeltaTime;
		return Current + Clamp<float>(Dist, -Step, Step);
	}

	static FVector VInterpTo(const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed);
	static FVector VInterpConstantTo(const FVector& Current, const FVector& Target, float DeltaTime, float InterpSpeed);
	static FRotator RInterpTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);
	static FRotator RInterpConstantTo(const FRotator& Current, const FRotator& Target, float DeltaTime, float InterpSpeed);
	static FVector VRand();

	static FORCEINLINE bool RandBool() { return (RandRange(0, 1) == 1) ? true : false; }

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
