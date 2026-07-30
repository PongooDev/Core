#include "pch.h"
#include "../Public/Client.h"

#include "../Public/Configuration.h"
#include "../Public/Finder.h"
#include "../Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameInstance.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "FortniteGame/Public/FortCheatManager.h"

void Client::Init(FCoreConfig& Config) {
    Finder::SetupOffsets();

    UEngine* Engine = UEngine::GetEngine();
    if (!Engine) {
        Log("Failed to get GEngine, is the game fully loaded?");
        return;
    }

    if (Config.bSetClientLogVerbosity) {
        Utils::SetLogVerbosity();
    }

    if (!Config.bIsProd) {
        UObject* Console = UGameplayStatics::SpawnObject(Engine->ConsoleClass.Get(), Engine->GameViewport);
        if (Console) {
            Engine->GameViewport->ViewportConsole = (UConsole*)Console;
            Log("Successfully spawned console: " + Console->GetName().ToString());
        }
        else {
            Log("Failed to spawn console!");
        }

        CreateThread(0, 0, ClientThread, 0, 0, 0);
    }

    SetConsoleTitleA(std::format("CoreClient ({:.2f})", Version::Fortnite_Version).c_str());
}

DWORD Client::ClientThread(LPVOID) {
    while (true)
    {
        UWorld* World = UWorld::GetWorld();

        if (World && World->OwningGameInstance)
        {
            if (World->OwningGameInstance->LocalPlayers.Num() > 0)
            {
                APlayerController* PlayerController = World->OwningGameInstance->LocalPlayers[0]->PlayerController;
                if (PlayerController && !PlayerController->CheatManager)
                {
                    const UClass* CheatClass = PlayerController->CheatClass.Get() ? PlayerController->CheatClass.Get() : UFortCheatManager::StaticClass();
                    Log("Spawning CheatManager: " + CheatClass->GetName().ToString() + " for PlayerController: " + PlayerController->GetName().ToString());
                    UObject* CheatManager = UGameplayStatics::SpawnObject(CheatClass, PlayerController);
                    if (CheatManager) {
                        PlayerController->CheatManager = (UCheatManager*)CheatManager;
                        Log("Successfully spawned CheatManager: " + CheatManager->GetName().ToString());
                    }
                    else
                    {
                        Log("Failed to spawn CheatManager!");
                    }
                }
            }
        }

        Sleep(30);
    }
}