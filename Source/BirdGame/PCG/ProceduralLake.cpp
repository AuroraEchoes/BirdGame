// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLake.h"

#include "Lake.h"

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

	SpawnLake(FVector(0,0,0));
	
}

void AProceduralLake::SpawnLake(const FVector& Location)
{
	if (LakeClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		//SpawnTransform.SetRotation(FQuat(Rotation));
		
		Lake = GetWorld()->SpawnActor<ALake>(LakeClass, SpawnTransform, SpawnParams);

		UStaticMeshComponent* MeshComp = Lake->FindComponentByClass<UStaticMeshComponent>();
		
		if (Lake)
		{
			UMaterialInstanceDynamic* DynamicMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

			if (DynamicMat)
			{
				DynamicMat->SetScalarParameterValue(FName("Circle1Size"), FMath::FRandRange(0.2f, 0.6f));
				DynamicMat->SetScalarParameterValue(FName("Circle2Size"), FMath::FRandRange(0.2f, 0.6f));
				DynamicMat->SetScalarParameterValue(FName("Circle1Location"), FMath::FRandRange(-0.064, -0.432));
				DynamicMat->SetScalarParameterValue(FName("Circle2Location"), FMath::FRandRange(-0.32f, 0.32f));
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Lake spawned: %s"), *Lake->GetName());
			Lake->SetFolderPath(TEXT("/SpawnedLake"));
			// SpawnedLake.Add(Lake);
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("LakeClass not set. Need to assign it in the editor."));
	}
}

// Called every frame
void AProceduralLake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

