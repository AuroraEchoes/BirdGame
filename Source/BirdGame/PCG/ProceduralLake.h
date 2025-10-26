// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralLake.generated.h"

class AProceduralGrass;
class AProceduralLandscape;
class ALake;

UCLASS()
class BIRDGAME_API AProceduralLake : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralLake();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnLake(const FVector& Location);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALake> LakeClass;

	UPROPERTY()
	ALake* Lake;

	UPROPERTY(EditAnywhere)
	AProceduralLandscape* ProceduralLandscape;

	UPROPERTY(EditAnywhere)
	AProceduralGrass* ProceduralGrass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnLakes();

};
