#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

FString APlayerState::GetPlayerName() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetPlayerName");

	if (!Func) {
		APlayerState* Mutable = const_cast<APlayerState*>(this);
		return Mutable->_HasPlayerNamePrivate() ? Mutable->PlayerNamePrivate : Mutable->PlayerName;
	}

	return const_cast<APlayerState*>(this)->Call<FString>(Func);
}