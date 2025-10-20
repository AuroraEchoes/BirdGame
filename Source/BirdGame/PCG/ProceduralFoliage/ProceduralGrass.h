// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralGrass.generated.h"

class AProceduralLandscape;
class AGrass;

UCLASS()
class BIRDGAME_API AProceduralGrass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralGrass();

	UPROPERTY()
	TArray<AGrass*> SpawnedGrass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnGrass(const FVector& Location);

	void SpawnGrassCluster(const FVector& SpawnOrigin);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGrass> GrassClass;

	UPROPERTY()
	AGrass* Grass;

	UPROPERTY(EditAnywhere)
	float SpawnRadius = 100.0f;

	UPROPERTY(EditAnywhere)
	int32 FoliageNum = 6;
	
	UPROPERTY(EditAnywhere)
	TArray<UMaterialInterface*> PossibleGrassMaterials;

	UPROPERTY(EditAnywhere)
	AProceduralLandscape* ProceduralLandscape;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnGrasses();

};
