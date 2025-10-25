// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralTree.h"

#include "Branch.h"
#include "Leaves.h"

// Sets default values
AProceduralTree::AProceduralTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AProceduralTree::SpawnTreeBase(const FVector& Location)
{
	float Scale = FMath::FRandRange(0.5f, 2.0f); //gets a random scale for the tree. This determines the size of the mesh and how many branches will be on the tree.

	// spawn the base of the tree.
	if (TreeBaseClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f); //set a random 360 degree rotation of the base of the tree.

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(FQuat(Rotation));
		
		TreeBase = GetWorld()->SpawnActor<ATreeBase>(TreeBaseClass, SpawnTransform, SpawnParams);

		if (TreeBase)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tree spawned: %s"), *TreeBase->GetName());
			TreeBase->SetFolderPath(TEXT("/SpawnedTrees"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TreeClass not set. Need to assign it in the editor."));
	}

	// determine whether this tree will have leaves or not
	if ( FMath::FRandRange(0.0f, 1.0f) <= NakedTreeSpawnRate)
	{
		BIsNakedTree = true;
		TreeBase->Tags.Add(FName("Naked Tree")); //for later integration with boids for birds to land on.
	}
	else
	{
		BIsNakedTree = false;
	}

	// the number of branches will be a random num from 3 to 8 based on scale.
	int32 numOfBranches;

	if (Scale < 1.0f)
	{
		numOfBranches = 3;
	}
	else if (Scale >= 1.0f && Scale <= 1.5f)
	{
		numOfBranches = 5;
	}
	else
	{
		numOfBranches = 8;
	}

	// spawn branches for the tree
	for (int32 i = 0; i < numOfBranches; i++)
	{
		SpawnBranch(Scale, Location, TreeBase); 
	}

	// apply scale the tree
	TreeBase->SetActorScale3D(FVector(Scale));
	
	SpawnedTrees.Add(TreeBase);
	
}

void AProceduralTree::SpawnTrees()
{
	//Gets the tree spawn points from procedural landscape and spawns a tree.
	for (auto Vertex : ProceduralLandscape->TreeSpawnPoints)
	{
		SpawnTreeBase(Vertex);
	}
}

// Called when the game starts or when spawned
void AProceduralTree::BeginPlay()
{
	Super::BeginPlay();
}


void AProceduralTree::SpawnBranch(const float& Scale, const FVector& Location, ATreeBase* MainTree)
{
	//spawn the branch using the treebase/maintree coordinates
	if (BranchClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);
		FTransform SpawnTransform;
		
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(FQuat(Rotation));
	
		
		Branch = GetWorld()->SpawnActor<ABranch>(BranchClass, SpawnTransform, SpawnParams);

		Branch->AddActorWorldOffset(FVector(0.f, 0.f, FMath::FRandRange(50.f, 170.f))); //moves the branch up and down the base of the tree.
		Branch->AttachToActor(MainTree, FAttachmentTransformRules::KeepWorldTransform);
		
		if (Branch)
		{
			//randomly assign a branch mesh
			UStaticMeshComponent* MeshComp = Branch->FindComponentByClass<UStaticMeshComponent>(); //get the mesh of branch
			if (MeshComp && PossibleMeshBranches.Num() > 0) //check if possible mesh branches are assigned
			{
				int32 Index = FMath::RandRange(0, PossibleMeshBranches.Num() - 1); //chooses a random branch
				MeshComp->SetStaticMesh(PossibleMeshBranches[Index]); //set the random branch as the static mesh of branch
			}
			UE_LOG(LogTemp, Warning, TEXT("Branch spawned: %s"), *Branch->GetName());

			//if the tree is not a naked tree, spawn the leaves on the socket of the branch
			if (!BIsNakedTree)
				SpawnLeaves(MeshComp, Branch);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BranchClass not set. Need to assign it in the editor."));
	}
}

void AProceduralTree::SpawnLeaves(UStaticMeshComponent* BranchMesh, ABranch* BranchToAttach)
{
	//spawn the leaves at the socket of the branch.
	if (BranchMesh->DoesSocketExist("Socket"))
	{
		FTransform SocketTransform = BranchMesh->GetSocketTransform("Socket", RTS_World);
		FVector SocketLocation = SocketTransform.GetLocation();
		FRotator SocketRotation = SocketTransform.GetRotation().Rotator();
		
		Leaves = GetWorld()->SpawnActor<ALeaves>(LeavesClass, SocketLocation, SocketRotation);
		Leaves->AttachToActor(BranchToAttach, FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AProceduralTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

