// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralGrass.h"

#include "Grass.h"
#include "BirdGame/PCG/ProceduralLandscape.h"

// Sets default values
AProceduralGrass::AProceduralGrass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralGrass::BeginPlay()
{
	Super::BeginPlay();
	FVector Location = FVector(0.f, 0.f, 0.f);
	// SpawnGrass(Location);
	// SpawnGrassCluster(Location);
	SpawnGrasses();
}

void AProceduralGrass::SpawnGrass(const FVector& Location)
{
	if (GrassClass)
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
		
		Grass = GetWorld()->SpawnActor<AGrass>(GrassClass, SpawnTransform, SpawnParams);

		if (Grass)
		{
			UStaticMeshComponent* MeshComp = Grass->FindComponentByClass<UStaticMeshComponent>();
			if (MeshComp && PossibleGrassMaterials.Num() > 0)
			{
				int32 MaterialIndex = FMath::RandRange(0, PossibleGrassMaterials.Num() - 1);
				MeshComp->SetMaterial(0, PossibleGrassMaterials[MaterialIndex]);
			}
			UE_LOG(LogTemp, Warning, TEXT("Grass spawned: %s"), *Grass->GetName());
			Grass->SetFolderPath(TEXT("/SpawnedGrass"));
			// SpawnedGrass.Add(Grass);
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("GrassClass not set. Need to assign it in the editor."));
	}
}

void AProceduralGrass::SpawnGrassCluster(const FVector& SpawnOrigin)
{
	for (int32 i = 0; i < FoliageNum; i++) 
	{
		FVector2D RandPoint = FMath::RandPointInCircle(SpawnRadius); // Get a random location within a circle
		FVector Location = SpawnOrigin + FVector(RandPoint.X, RandPoint.Y, 0.f); //move it to the spawn origin point
		SpawnGrass(Location);
	}
}

void AProceduralGrass::SpawnGrasses()
{
	for (auto Vertex : ProceduralLandscape->GrassSpawnPoints)
	{
		SpawnGrassCluster(Vertex);
	}
}

// Called every frame
void AProceduralGrass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

