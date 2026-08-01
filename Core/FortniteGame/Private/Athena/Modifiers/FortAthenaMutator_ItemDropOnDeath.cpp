#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_ItemDropOnDeath.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"


void AFortAthenaMutator_ItemDropOnDeath::Hook() {
  if (Finder::FindAFortAthenaMutator_ItemDropOnDeath_SpawnItems_K2_SpawnPickupInWorld())
    PatchCallFar(ImageBase + Finder::FindAFortAthenaMutator_ItemDropOnDeath_SpawnItems_K2_SpawnPickupInWorld(), UFortKismetLibrary::K2_SpawnPickupInWorld);

  if (Finder::FindAFortAthenaMutator_ItemDropOnDeath_SpawnItems_K2_SpawnPickupInWorldWithLootTier())
    PatchCallFar(ImageBase + Finder::FindAFortAthenaMutator_ItemDropOnDeath_SpawnItems_K2_SpawnPickupInWorldWithLootTier(), UFortKismetLibrary::K2_SpawnPickupInWorldWithClass);

  Log("Hooked AFortAthenaMutator_ItemDropOnDeath");
}