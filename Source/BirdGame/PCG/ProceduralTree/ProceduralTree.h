// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Branch.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "TreeBase.h"
#include "BirdGame/PCG/ProceduralLandscape.h"
#include "ProceduralTree.generated.h"

class ALeaves;
class ABranch;

UCLASS()
class BIRDGAME_API AProceduralTree : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralTree();
	void SpawnTreeBase(const FVector& Location);
	void SpawnTrees();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnBranch(const float& Scale, const FVector& Location, ATreeBase* MainTree);
	void SpawnLeaves(UStaticMeshComponent* BranchMesh, ABranch* BranchToAttach);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATreeBase> TreeBaseClass; //getting the blueprint class of tree base

	UPROPERTY()
	ATreeBase* TreeBase; //stores the instance of tree base class here


	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABranch> BranchClass; //getting the blueprint class of branch

	UPROPERTY()
	ABranch* Branch; //stores the instance of branch class here

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALeaves> LeavesClass; //getting the blueprint class of leaves

	UPROPERTY()
	ALeaves* Leaves; //stores the instance of leaves class here
	

	UPROPERTY(EditAnywhere, Category="Spawning")
	TArray<UStaticMesh*> PossibleMeshBranches;

	UPROPERTY(EditAnywhere)
	AProceduralLandscape* ProceduralLandscape;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
