#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

class UDataTable : public UObject {
public:
	DefineUnrealClass(UDataTable);

	UScriptStruct* RowStruct;
	TMap<FName, uint8*> RowMap;

	DefineBitfieldUProperty(bStripFromClientBuilds);
	DefineBitfieldUProperty(bIgnoreExtraFields);
	DefineBitfieldUProperty(bIgnoreMissingFields);

	DefineUProperty(FString, ImportKeyField);
public:
	template <class T>
	void GetAllRows(TArray<T*>& OutRowArray) const
	{
		if (RowStruct == nullptr)
		{
			Log("UDataTable::GetAllRows : DataTable '" + GetPathName().ToString() + "' has no RowStruct specified.");
		}
		else
		{
			OutRowArray.Reserve(OutRowArray.Num() + RowMap.Num());
			for (int i = 0; i < RowMap.Num(); ++i)
			{
				auto& Row = RowMap[i];
				OutRowArray.Add(reinterpret_cast<T*>(Row.Value()));
			}
		}
	}

	/** Function to find the row of a table given its name. */
	template <class T>
	T* FindRow(FName RowName, bool bWarnIfRowMissing = true) const
	{
		if (RowStruct == nullptr)
		{
			Log("UDataTable::FindRow : specified no row for DataTable '" + GetPathName().ToString() + "'.");
			return nullptr;
		}

		if (RowName == UKismetStringLibrary::Conv_StringToName(L"None"))
		{
			Log("UDataTable::FindRow : requested invalid row 'None' from DataTable '" + GetPathName().ToString() + "'.");
			return nullptr;
		}

		uint8* const* RowDataPtr = nullptr;
		for (int i = 0; i < RowMap.Num(); i++)
		{
			auto& Row = RowMap[i];
			if (Row.Key() == RowName)
			{
				RowDataPtr = &Row.Value();
				break;
			}
		}
		if (RowDataPtr == nullptr)
		{
			if (bWarnIfRowMissing)
			{
				Log("UDataTable::FindRow : requested row '" + RowName.ToString().ToString() + "' not in DataTable '" + GetPathName().ToString() + "'.");
			}
			return nullptr;
		}

		uint8* RowData = *RowDataPtr;
		if (RowData == nullptr)
		{
			Log("UDataTable::FindRow: RowData is nullptr!");
			return nullptr;
		}

		return reinterpret_cast<T*>(RowData);
	}

	uint8* FindRowUnchecked(FName RowName, bool MustExist = false) const
	{
		if (RowStruct == nullptr)
		{
			return nullptr;
		}

		if (RowName == UKismetStringLibrary::Conv_StringToName(L"None"))
		{
			return nullptr;
		}

		uint8* const* RowDataPtr = nullptr;
		for (int i = 0; i < RowMap.Num(); i++)
		{
			auto& Row = RowMap[i];
			if (Row.Key() == RowName)
			{
				RowDataPtr = &Row.Value();
				break;
			}
		}

		if (RowDataPtr == nullptr)
		{
			return nullptr;
		}

		uint8* RowData = *RowDataPtr;
		return RowData;
	}
};

struct FDataTableRowHandle {
public:
	DefineUnrealStruct(FDataTableRowHandle);

	/** Pointer to table we want a row from */
	UDataTable* DataTable;

	/** Name of row in the table that we want */
	FName RowName;
};

struct FTableRowBase
{
public:
	DefineUnrealStruct(FTaleRowBase);

	FTableRowBase() {}

public:
	void** VTable;
};