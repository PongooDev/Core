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

    return Call<void>(Func);
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

    return Call<void>(Func, GhostModeItemDef);
}

void AFortPlayerControllerGameplay::Hook() {
    Log("AFortPlayerControllerGameplay Hooked");
}
