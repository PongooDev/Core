#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"

void AFortPlayerControllerAthena::SpawnQuickBars()
{
	if (Finder::FindAFortPlayerControllerAthena_SpawnQuickBars()) {
		void (*SpawnQuickBarsInternal)(AFortPlayerControllerAthena* This) = decltype(SpawnQuickBarsInternal)(ImageBase + Finder::FindAFortPlayerControllerAthena_SpawnQuickBars());
		SpawnQuickBarsInternal(this);
	}
	else {
		if (Version::Fortnite_Version <= 1.72) {
			if (!this->QuickBars)
			{
				UWorld* World = this->GetWorld();
				if (!World) {
					return;
				}
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
				}
			}
		}
		else {
			if (!this->QuickBars)
			{
				UWorld* World = this->GetWorld();
				if (!World) {
					return;
				}
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBarsAthena::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					ClientQuickBars = NewQuickBars->Cast<AFortQuickBars>();
				}
			}
		}
	}
}

void AFortPlayerControllerAthena::SetupQuickBars()
{
	if (Finder::FindAFortPlayerControllerAthena_SetupQuickBars()) {
		void (*SetupQuickBarsInternal)(AFortPlayerControllerAthena* This) = decltype(SetupQuickBarsInternal)(ImageBase + Finder::FindAFortPlayerControllerAthena_SetupQuickBars());
		SetupQuickBarsInternal(this);
	}
}