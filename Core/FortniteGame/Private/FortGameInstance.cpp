#include "pch.h"
#include "FortniteGame/Public/FortGameInstance.h"

#include "FortniteGame/Public/Analytics/FortAnalytics.h"

bool UFortGameInstance::ServerTravel(const FString& URL, bool bAbsolute, bool bShouldSkipGameNotify) {
	if (Finder::FindUFortGameInstance_ServerTravel()) {
		bool (*ServerTravelInternal)(UFortGameInstance*, const FString&, bool, bool) = decltype(ServerTravelInternal)(ImageBase + Finder::FindUFortGameInstance_ServerTravel());
		return ServerTravelInternal(this, URL, bAbsolute, bShouldSkipGameNotify);
	} {
		UWorld* World = UWorld::GetWorld();
		if (!World) {
			Log("UFortGameInstance::ServerTravel: World is null!");
			return false;
		}

		World->ServerTravel(URL, bAbsolute, bShouldSkipGameNotify);
	}
}