// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLake.h"

#include "Lake.h"
#include "ProceduralLandscape.h"
#include "ProceduralFoliage/Grass.h"

// Sets default values
AProceduralLake::AProceduralLake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProceduralLake::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralLake::SpawnLake(const FVector& Location)
{
	//spawns the lake at the given location.
	if (LakeClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location + FVector(0,0,10)); //move it up slightly above the landscape mesh
		
		Lake = GetWorld()->SpawnActor<ALake>(LakeClass, SpawnTransform, SpawnParams);

		UStaticMeshComponent* MeshComp = Lake->FindComponentByClass<UStaticMeshComponent>();
		
		if (Lake)
		{
			//creates a dynamic material of lake to procedurally generate a lake shape using random values.
			UMaterialInstanceDynamic* DynamicMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

			if (DynamicMat)
			{
				DynamicMat->SetScalarParameterValue(FName("Circle1Size"), FMath::FRandRange(0.2f, 0.6f));
				DynamicMat->SetScalarParameterValue(FName("Circle2Size"), FMath::FRandRange(0.2f, 0.6f));
				
				DynamicMat->SetScalarParameterValue(FName("Circle1Location"), FMath::FRandRange(-0.064, -0.432));
				DynamicMat->SetScalarParameterValue(FName("Circle2Location"), FMath::FRandRange(-0.32f, 0.32f));

				DynamicMat->SetScalarParameterValue(FName("Circle1PerlinOffset"), FMath::FRandRange(0.0f, 0.3f));
				DynamicMat->SetScalarParameterValue(FName("Circle2PerlinOffset"), FMath::FRandRange(0.0f, 0.3f));

				DynamicMat->SetScalarParameterValue(FName("Circle1PerlinScale"), FMath::FRandRange(0.5f, 1.0f));
				DynamicMat->SetScalarParameterValue(FName("Circle2PerlinScale"), FMath::FRandRange(0.5f, 1.0f));

				DynamicMat->SetScalarParameterValue(FName("ConnectCirclesDistance"), FMath::FRandRange(0.0f, 1.0f));

			}
			
			UE_LOG(LogTemp, Warning, TEXT("Lake spawned: %s"), *Lake->GetName());
			Lake->SetFolderPath(TEXT("/SpawnedLake"));
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("LakeClass not set. Need to assign it in the editor."));
	}
}

void AProceduralLake::SpawnLakes()
{
	//gets spawn points on the middle of flat quads from ProceduralLandscape to spawn the lake on.
	for (auto Vertex : ProceduralLandscape->FlatSpawnPoints)
	{
		SpawnLake(Vertex);
	}
}

// Called every frame
void AProceduralLake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

