#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerGameplay.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Inventory/FortQuickBars.h"

void AFortPlayerControllerGameplay::CheckGhostModeItemReplicated()
{
    static UFunction* Func = nullptr;

    if (Func == nullptr)
        Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerGameplay.CheckGhostModeItemReplicated");

    if (!Func) {
        Log("FortPlayerControllerGameplay::CheckGhostModeItemReplicated: Failed to find function!");
        return;
    }

    return GetDefaultObj()->Call<void>(Func);
}

void AFortPlayerControllerGameplay::CheckGhostModeItemRemoved(UFortWorldItemDefinition* GhostModeItemDef)
{
    static UFunction* Func = nullptr;

    if (Func == nullptr)
        Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerControllerGameplay.CheckGhostModeItemRemoved");

    if (!Func) {
        Log("FortPlayerControllerGameplay::CheckGhostModeItemRemoved: Failed to find function!");
        return;
    }

    return GetDefaultObj()->Call<void>(Func, GhostModeItemDef);
}

void AFortPlayerControllerGameplay::execStartGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack) {
    UFortWorldItemDefinition* ItemProvidingGhostMode = nullptr;
    Stack.StepCompiledIn(&ItemProvidingGhostMode);
    Stack.IncrementCode();

    Context->StartGhostMode(ItemProvidingGhostMode);
}

void AFortPlayerControllerGameplay::execEndGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack)
{
    Stack.IncrementCode();

    Context->EndGhostMode();
}

void AFortPlayerControllerGameplay::StartGhostMode(UFortWorldItemDefinition* ItemProvidingGhostMode)
{
    if (GhostModeRepData.bInGhostMode || !ItemProvidingGhostMode)
        return;

    AFortPlayerPawnAthena* AthenaPawn = (AFortPlayerPawnAthena*)this->Pawn;

    UFortKismetLibrary::K2_RemoveItemFromPlayer(this, ItemProvidingGhostMode, -1, true);

    this->WorldInventory->AddItem(ItemProvidingGhostMode, 1, 0, true);

    GhostModeRepData.bInGhostMode = true;
    GhostModeRepData.GhostModeItemDef = ItemProvidingGhostMode;

    AFortQuickBars* FortQuickBars = QuickBars;
    if (!FortQuickBars) {
        FortQuickBars = ClientQuickBars;
    }
    if (QuickBars)
    {
        if (GhostModeRepData.bInGhostMode)
        {
            GhostModeRepData.PreviousFocusedSlot = QuickBars->PrimaryQuickBar.CurrentFocusedSlot;
            CheckGhostModeItemReplicated();
        }
        else
        {
            CheckGhostModeItemRemoved(GhostModeRepData.GhostModeItemDef);
        }
    }

    OnGhostModeChanged.ProcessMulticastDelegate(&GhostModeRepData.bInGhostMode);

    ForceNetUpdate();
}

void AFortPlayerControllerGameplay::EndGhostMode()
{
    if (UFortWorldItemDefinition* GhostModeItemDef = GhostModeRepData.GhostModeItemDef)
        UFortKismetLibrary::K2_RemoveItemFromPlayer(this, GhostModeItemDef, -1, true);

    GhostModeRepData.bInGhostMode = false;

    AFortQuickBars* FortQuickBars = QuickBars;
    if (!FortQuickBars) {
        FortQuickBars = ClientQuickBars;
    }

    if (QuickBars) {
        CheckGhostModeItemRemoved(GhostModeRepData.GhostModeItemDef);
    }

    OnGhostModeChanged.ProcessMulticastDelegate(&GhostModeRepData.bInGhostMode);

    ForceNetUpdate();
}

void AFortPlayerControllerGameplay::Hook() {
    ExecHook("Function /Script/FortniteGame.FortPlayerControllerGameplay.StartGhostMode", execStartGhostMode, execStartGhostModeOG);

    ExecHook("Function /Script/FortniteGame.FortPlayerControllerGameplay.EndGhostMode", execEndGhostMode, execEndGhostModeOG);

    Log("AFortPlayerControllerGameplay Hooked");
}