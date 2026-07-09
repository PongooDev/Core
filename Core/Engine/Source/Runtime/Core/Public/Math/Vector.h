#pragma once
#include "pch.h"

#include "UnrealMathUtility.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FVector
{
public:
	DefineUnrealStruct(FVector);
public:
	float X;
	float Y;
	float Z;
public:
	FORCEINLINE FVector();

	explicit FORCEINLINE FVector(float InF);

	FORCEINLINE FVector(float InX, float InY, float InZ);
public:
	FORCEINLINE float operator|(const FVector& V) const;

	FORCEINLINE FVector operator+(const FVector& V) const;

	FORCEINLINE FVector operator-(const FVector& V) const;

	FORCEINLINE FVector operator+(float Bias) const;

	FORCEINLINE FVector operator*(float Scale) const;

	FORCEINLINE FVector operator*(const FVector& V) const;

	FORCEINLINE FVector operator+=(const FVector& V);
public:
	float SizeSquared() const;

	std::string FormatToString() const {
		return std::format("X={}, Y={}, Z={}", X, Y, Z);
	}

	bool IsZero() const {
		return X == 0 && Y == 0 && Z == 0;
	}
};

FORCEINLINE FVector::FVector()
{
	X = 0;
	Y = 0;
	Z = 0;
}

FORCEINLINE FVector::FVector(float InF)
	: X(InF), Y(InF), Z(InF)
{

}

FORCEINLINE FVector::FVector(float InX, float InY, float InZ)
	: X(InX), Y(InY), Z(InZ)
{

}

FORCEINLINE float FVector::operator|(const FVector& V) const
{
	return X * V.X + Y * V.Y + Z * V.Z;
}

FORCEINLINE FVector FVector::operator+(const FVector& V) const
{
	return FVector(X + V.X, Y + V.Y, Z + V.Z);
}

FORCEINLINE FVector FVector::operator-(const FVector& V) const
{
	return FVector(X - V.X, Y - V.Y, Z - V.Z);
}

FORCEINLINE FVector FVector::operator+(float Bias) const
{
	return FVector(X + Bias, Y + Bias, Z + Bias);
}

FORCEINLINE FVector FVector::operator*(float Scale) const
{
	return FVector(X * Scale, Y * Scale, Z * Scale);
}

FORCEINLINE FVector FVector::operator*(const FVector& V) const
{
	return FVector(X * V.X, Y * V.Y, Z * V.Z);
}

FORCEINLINE FVector FVector::operator+=(const FVector& V)
{
	X += V.X; Y += V.Y; Z += V.Z;
	return *this;
}

FORCEINLINE float FVector::SizeSquared() const
{
	return X * X + Y * Y + Z * Z;
}