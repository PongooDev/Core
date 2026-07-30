#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class USoundBase;
class UTexture2D;

class UAthenaMusicPackItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaMusicPackItemDefinition);

	DefineUProperty(TSoftObjectPtr<USoundBase>, FrontEndLobbyMusic);
	DefineUProperty(TSoftObjectPtr<UTexture2D>, CoverArtImage);
};
