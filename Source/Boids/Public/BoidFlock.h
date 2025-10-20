// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidBehaviour.h"
#include "GameFramework/Actor.h"
#include "BoidFlock.generated.h"

UCLASS()
class BOIDS_API ABoidFlock : public AActor
{
    GENERATED_BODY()

public:
    ABoidFlock();
    virtual void Tick(float DeltaTime) override;
    void AddSpawnedBoid(APawn* Boid);
    UFUNCTION(BlueprintCallable)
    TArray<APawn*> GetNearbyBoidLocations(const APawn* Boid, double Distance);
    UFUNCTION(BlueprintCallable)
    APawn* GetPlayerLocation();
    UFUNCTION(BlueprintCallable)
    FVector GetClosestUnobstructedDirection(const APawn* Boid, double Distance, int NumPoints);
    void AddBoidBehaviour(const TSubclassOf<UBoidBehaviour>& BehaviourClass);
    void DebugVisualiseBehaviour(const APawn* Boid, const UBoidBehaviour* Behaviour, const FVector& DesiredDirection) const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void TickControllingBoids(const float& DeltaTime);
    void TickControllingBoid(const float& DeltaTime, APawn* Boid);
    TArray<FVector> GetCollisionSweepPoints(int NumPoints);
    
    UPROPERTY(EditAnywhere)
    double MaxBoidSpeed = 100.0;
    UPROPERTY(EditAnywhere)
    double MaxBoidTurnRateDegrees = 30.0;
    UPROPERTY(EditAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(VisibleAnywhere)
    TArray<APawn*> ControllingBoids;
    UPROPERTY(VisibleAnywhere)
    TArray<UBoidBehaviour*> BoidBehaviours;
    UPROPERTY(EditAnywhere)
    bool DebugVisualiseAllBehaviours;
    double TotalBehaviourWeighting{};
};
