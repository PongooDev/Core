#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class FArchive;
class FOutputDevice;
class UObject;

/**
 * Enumerates known GUID formats.
 */
enum class EGuidFormats
{
	/**
	 * 32 digits.
	 *
	 * For example: "00000000000000000000000000000000"
	 */
	Digits,

	/**
	 * 32 digits separated by hyphens.
	 *
	 * For example: 00000000-0000-0000-0000-000000000000
	 */
	DigitsWithHyphens,

	/**
	 * 32 digits separated by hyphens and enclosed in braces.
	 *
	 * For example: {00000000-0000-0000-0000-000000000000}
	 */
	DigitsWithHyphensInBraces,

	/**
	 * 32 digits separated by hyphens and enclosed in parentheses.
	 *
	 * For example: (00000000-0000-0000-0000-000000000000)
	 */
	DigitsWithHyphensInParentheses,

	/**
	 * Comma-separated hexadecimal values enclosed in braces.
	 *
	 * For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
	 */
	HexValuesInBraces,

	/**
	 * This format is currently used by the FUniqueObjectGuid class.
	 *
	 * For example: 00000000-00000000-00000000-00000000
	*/
	UniqueObjectGuid,

	/**
	 * Base64 characters with dashes and underscores instead of pluses and slashes (respectively)
	 *
	 * For example: AQsMCQ0PAAUKCgQEBAgADQ
	*/
	Short,

	/**
	 * Base-36 encoded, compatible with case-insensitive OS file systems (such as Windows).
	 *
	 * For example: 1DPF6ARFCM4XH5RMWPU8TGR0J
	 */
	Base36Encoded,
};

/**
 * Implements a globally unique identifier.
 */
struct FGuid
{
public:
	DefineUnrealStruct(FGuid);
public:

	/** Default constructor. */
	FGuid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{
	}

	/**
	 * Creates and initializes a new GUID from the specified components.
	 *
	 * @param InA The first component.
	 * @param InB The second component.
	 * @param InC The third component.
	 * @param InD The fourth component.
	 */
	FGuid(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
		: A(InA), B(InB), C(InC), D(InD)
	{
	}

public:

	/**
	 * Compares two GUIDs for equality.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the GUIDs are equal, false otherwise.
	 */
	friend bool operator==(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	/**
	 * Compares two GUIDs for inequality.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the GUIDs are not equal, false otherwise.
	 */
	friend bool operator!=(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	/**
	 * Compares two GUIDs.
	 *
	 * @param X The first GUID to compare.
	 * @param Y The second GUID to compare.
	 * @return true if the first GUID is less than the second one.
	 */
	friend bool operator<(const FGuid& X, const FGuid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
					((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}

	/**
	 * Provides access to the GUIDs components.
	 *
	 * @param Index The index of the component to return (0...3).
	 * @return The component.
	 */
	uint32& operator[](int32 Index)
	{
		if (!(Index >= 0)) Log("FGuid::operator[]: Index < 0");
		if (!(Index < 4)) Log("FGuid::operator[]: Index >= 4");

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	/**
	 * Provides read-only access to the GUIDs components.
	 *
	 * @param Index The index of the component to return (0...3).
	 * @return The component.
	 */
	const uint32& operator[](int32 Index) const
	{
		if (!(Index >= 0)) Log("FGuid::operator[] const: Index < 0");
		if (!(Index < 4)) Log("FGuid::operator[] const: Index >= 4");

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

public:
	/**
	 * Invalidates the GUID.
	 *
	 * @see IsValid
	 */
	void Invalidate()
	{
		A = B = C = D = 0;
	}

	/**
	 * Checks whether this GUID is valid or not.
	 *
	 * A GUID that has all its components set to zero is considered invalid.
	 *
	 * @return true if valid, false otherwise.
	 * @see Invalidate
	 */
	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	/**
	 * Converts this GUID to its string representation.
	 *
	 * @return The string representation.
	 */
	FString ToString() const
	{
		return ToString(EGuidFormats::Digits);
	}

	/**
	 * Converts this GUID to its string representation using the specified format.
	 *
	 * @param Format The string format to use.
	 * @return The string representation.
	 */
	FString ToString(EGuidFormats Format) const;

public:

	/** Holds the first component. */
	uint32 A;

	/** Holds the second component. */
	uint32 B;

	/** Holds the third component. */
	uint32 C;

	/** Holds the fourth component. */
	uint32 D;
public:
	std::string FormatGuid() const
	{
		return std::format("{{{:08X}-{:08X}-{:08X}-{:08X}}}", A, B, C, D);
	}

	static FGuid ParseGUID(std::string GuidString) {
		FGuid Guid;

		if (GuidString.empty()) {
			return Guid;
		}

		if (GuidString.front() == '{' && GuidString.back() == '}') {
			GuidString = GuidString.substr(1, GuidString.size() - 2);
		}

		std::replace(GuidString.begin(), GuidString.end(), '-', ' ');

		std::istringstream iss(GuidString);

		uint32_t a, b, c, d;

		if (!(iss >> std::hex >> a >> b >> c >> d)) {
			return FGuid();
		}

		Guid.A = a;
		Guid.B = b;
		Guid.C = c;
		Guid.D = d;

		return Guid;
	}
};