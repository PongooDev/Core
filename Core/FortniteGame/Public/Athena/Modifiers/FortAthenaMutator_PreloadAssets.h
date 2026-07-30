#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_PreloadAssets : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PreloadAssets);

	DefineUProperty(TArray<UMaterialInstance*>, MaterialInstances);
	DefineUProperty(TArray<USkeletalMesh*>, SkeletalMeshes);
	DefineUProperty(TArray<UStaticMesh*>, StaticMeshes);
	DefineUProperty(TArray<UDataAsset*>, DataAssets);
	DefineUProperty(TArray<TSubclassOf<UAnimInstance>>, AnimBlueprints);
	DefineUProperty(TArray<TSubclassOf<AActor>>, ActorBlueprints);
};
