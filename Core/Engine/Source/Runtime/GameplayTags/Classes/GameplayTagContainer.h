#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

enum class EGameplayContainerMatchType : uint8
{
	//	Means the filter is populated by any tag matches in this container.
	Any,

	//	Means the filter is only populated if all of the tags in this container match.
	All
};

struct FGameplayTag {
public:
	DefineUnrealStruct(FGameplayTag);
public:
	/** Constructors */
	FGameplayTag()
	{
	}

	/** Operators */
	inline bool operator==(FGameplayTag const& Other) const
	{
		return TagName == Other.TagName;
	}

	inline bool operator!=(FGameplayTag const& Other) const
	{
		return TagName != Other.TagName;
	}

	/**
	 * Determine if TagToCheck is valid and exactly matches this tag
	 * "A.1".MatchesTagExact("A") will return False
	 * If TagToCheck is not Valid it will always return False
	 *
	 * @return True if TagToCheck is Valid and is exactly this tag
	 */
	inline bool MatchesTagExact(const FGameplayTag& TagToCheck) const
	{
		if (!TagToCheck.IsValid())
		{
			return false;
		}
		// Only check explicit tag list
		return TagName == TagToCheck.TagName;
	}

	/** Returns whether the tag is valid or not; Invalid tags are set to NAME_None and do not exist in the game-specific global dictionary */
	inline bool IsValid() const
	{
		return (TagName.ToString().ToString() != "None");
	}

	/** Displays gameplay tag as a string for blueprint graph usage */
	inline FString ToString() const
	{
		return TagName.ToString();
	}

	/** Get the tag represented as a name */
	inline FName GetTagName() const
	{
		return TagName;
	}
protected:
	FName TagName;
};

struct FGameplayTagContainer {
public:
	DefineUnrealStruct(FGameplayTagContainer);
public:
	/** Constructors */
	FGameplayTagContainer()
	{
	}

	FGameplayTagContainer(FGameplayTagContainer const& Other)
	{
		*this = Other;
	}

	~FGameplayTagContainer()
	{
	}

	/** Returns the number of explicitly added tags */
	inline int32 Num() const
	{
		return GameplayTags.Num();
	}

	/** Returns whether the container has any valid tags */
	inline bool IsValid() const
	{
		return GameplayTags.Num() > 0;
	}

	/** Returns true if container is empty */
	inline bool IsEmpty() const
	{
		return GameplayTags.Num() == 0;
	}

	bool IsValidIndex(int32 Index) const
	{
		return GameplayTags.IsValidIndex(Index);
	}

	FGameplayTag GetByIndex(int32 Index) const
	{
		if (IsValidIndex(Index))
		{
			return GameplayTags[Index];
		}
		return FGameplayTag();
	}

	FGameplayTag First() const
	{
		return GameplayTags.Num() > 0 ? GameplayTags[0] : FGameplayTag();
	}

	FGameplayTag Last() const
	{
		return GameplayTags.Num() > 0 ? GameplayTags[GameplayTags.Num() - 1] : FGameplayTag();
	}
public:
	TArray<FGameplayTag> GameplayTags;

	TArray<FGameplayTag> ParentTags;
};