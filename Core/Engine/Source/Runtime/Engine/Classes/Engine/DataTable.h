#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

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
	FORCEINLINE UScriptStruct* GetRowStruct() const { return RowStruct; }
	FORCEINLINE const TMap<FName, uint8*>& GetRowMap() const { return RowMap; }
	FORCEINLINE TMap<FName, uint8*>& GetRowMap() { return RowMap; }

	template <class T>
	void GetAllRows(TArray<T*>& OutRowArray) const
	{
		if (RowStruct == nullptr)
		{
			Log("UDataTable::GetAllRows : DataTable '" + GetPathName().ToString() + "' has no RowStruct specified.");
			return;
		}

		TArray<uint8*> Rows;
		RowMap.GenerateValueArray(Rows);

		OutRowArray.Reserve(OutRowArray.Num() + Rows.Num());
		for (int32 i = 0; i < Rows.Num(); ++i)
		{
			OutRowArray.Add(reinterpret_cast<T*>(Rows[i]));
		}
	}

	TArray<FName> GetRowNames() const
	{
		TArray<FName> Keys;
		RowMap.GenerateKeyArray(Keys);
		return Keys;
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

		uint8* const* RowDataPtr = RowMap.Find(RowName);
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

		uint8* const* RowDataPtr = RowMap.Find(RowName);
		if (RowDataPtr == nullptr)
		{
			return nullptr;
		}

		return *RowDataPtr;
	}
};

class UDataTable;

struct FDataTableCategoryHandle {
public:
	DefineUnrealStruct(FDataTableCategoryHandle);

	DefineStructProperty(UDataTable*, DataTable);
	DefineStructProperty(FName, ColumnName);
	DefineStructProperty(FName, RowContents);
public:
	uint8 Padding[0x18];
};

struct FDataTableRowHandle {
public:
	DefineUnrealStruct(FDataTableRowHandle);

	/** Pointer to table we want a row from */
	UDataTable* DataTable;

	/** Name of row in the table that we want */
	FName RowName;

	FORCEINLINE bool IsNull() const { return DataTable == nullptr && RowName == FName(); }

	template <class T>
	T* GetRow(bool bWarnIfRowMissing = true) const
	{
		if (DataTable == nullptr)
		{
			return nullptr;
		}

		return DataTable->FindRow<T>(RowName, bWarnIfRowMissing);
	}
};

struct FTableRowBase
{
public:
	DefineUnrealStruct(FTableRowBase);

public:
	void** VTable;
};
