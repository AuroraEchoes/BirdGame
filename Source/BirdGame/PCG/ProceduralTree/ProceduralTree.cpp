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
	float Scale = FMath::FRandRange(0.5f, 2.0f);
	if (TreeBaseClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

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

	if ( FMath::FRandRange(0.0f, 1.0f) <= NakedTreeSpawnRate)
	{
		BIsNakedTree = true;
		TreeBase->Tags.Add(FName("Naked Tree")); //for later integration with boids for birds to land on.
	}
	else
	{
		BIsNakedTree = false;
	}

	int32 numOfBranches; //random num from 3 to 8

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

	for (int32 i = 0; i < numOfBranches; i++)
	{
		SpawnBranch(Scale, Location, TreeBase); //attach branch to tree base
	}
	
	TreeBase->SetActorScale3D(FVector(Scale));
	
	SpawnedTrees.Add(TreeBase);
	
}

void AProceduralTree::SpawnTrees()
{
	for (auto Vertex : ProceduralLandscape->TreeSpawnPoints)
	{
		SpawnTreeBase(Vertex);
	}
}

// Called when the game starts or when spawned
void AProceduralTree::BeginPlay()
{
	Super::BeginPlay();
	// debugging
	// FVector Location = FVector(-250.f, 0.f, 0.f);
	// SpawnTreeBase(Location);
	// SpawnTrees();
}


void AProceduralTree::SpawnBranch(const float& Scale, const FVector& Location, ATreeBase* MainTree)
{
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
		//SpawnTransform.SetScale3D(FVector(Scale, Scale, Scale));
	
		
		Branch = GetWorld()->SpawnActor<ABranch>(BranchClass, SpawnTransform, SpawnParams);

		// Leaves = GetWorld()->SpawnActor<ALeaves>(LeavesClass, SpawnTransform, SpawnParams);

		Branch->AddActorWorldOffset(FVector(0.f, 0.f, FMath::FRandRange(50.f, 170.f))); //moves the branch up and down the base of the tree.
		Branch->AttachToActor(MainTree, FAttachmentTransformRules::KeepWorldTransform);
		
		if (Branch)
		{
			UStaticMeshComponent* MeshComp = Branch->FindComponentByClass<UStaticMeshComponent>(); //get the mesh of branch
			if (MeshComp && PossibleMeshBranches.Num() > 0) //check if possible mesh branches are assigned
			{
				int32 Index = FMath::RandRange(0, PossibleMeshBranches.Num() - 1); //chooses a random branch
				MeshComp->SetStaticMesh(PossibleMeshBranches[Index]); //set the random branch as the static mesh of branch
			}
			UE_LOG(LogTemp, Warning, TEXT("Branch spawned: %s"), *Branch->GetName());

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

