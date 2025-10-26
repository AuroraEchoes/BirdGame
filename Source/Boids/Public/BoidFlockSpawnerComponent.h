// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidBase.h"
#include "Components/ActorComponent.h"
#include "BoidFlockSpawnerComponent.generated.h"


class UBoidBehaviour;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BOIDS_API UBoidFlockSpawnerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UBoidFlockSpawnerComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction*
                               ThisTickFunction) override;
    double GetSpawnRadius() const;

protected:
    UPROPERTY(EditAnywhere)
    int BoidsToSpawn = 10;
    UPROPERTY(EditAnywhere)
    double SpawnRadius = 300.0;
    UPROPERTY(EditAnywhere)
    TSubclassOf<ABoidBase> BoidInstanceClass;
    UPROPERTY(EditAnywhere)
    TArray<TSubclassOf<UBoidBehaviour>> InitialBoidBehaviours;
    
    // Called when the game starts
    virtual void BeginPlay() override;
    void SpawnBoids();

private:
    FVector GetBoidSpawnLocation(const int& BoidIndex) const;
};