// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidBase.h"
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
    void AddSpawnedBoid(ABoidBase* Boid);
    UFUNCTION(BlueprintCallable)
    TArray<ABoidBase*> GetNearbyBoidLocations(const ABoidBase* Boid, double Distance);
    UFUNCTION(BlueprintCallable)
    APawn* GetPlayerLocation();
    UFUNCTION(BlueprintCallable)
    TArray<APawn*> GetAllPlayers() const;
    UFUNCTION(BlueprintCallable)
    FVector GetClosestUnobstructedDirection(const ABoidBase* Boid, double Distance, int NumPoints);
    UFUNCTION(BlueprintCallable)
    AGameStateBase* GetGameState();
    UFUNCTION(BlueprintCallable)
    UWorld* GetWorldContext() const;
    void AddBoidBehaviour(const TSubclassOf<UBoidBehaviour>& BehaviourClass);
    void DebugVisualiseBehaviour(const ABoidBase* Boid, const UBoidBehaviour* Behaviour, const FVector& DesiredDirection) const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    UFUNCTION(Server, Reliable)
    void TickControllingBoids(const float& DeltaTime);
    UFUNCTION(Server, Reliable)
    void TickControllingBoid(const float& DeltaTime, ABoidBase* Boid);
    TArray<FVector> GetCollisionSweepPoints(int NumPoints);

    UPROPERTY(EditAnywhere)
    double MaxBoidSpeed = 100.0;
    UPROPERTY(EditAnywhere)
    double MaxBoidTurnRateDegrees = 30.0;
    UPROPERTY(EditAnywhere)
    USceneComponent* SceneComponent;
    UPROPERTY(VisibleAnywhere)
    TArray<ABoidBase*> ControllingBoids;
    UPROPERTY(VisibleAnywhere)
    TArray<UBoidBehaviour*> BoidBehaviours;
    UPROPERTY(EditAnywhere)
    bool DebugVisualiseAllBehaviours;
    double TotalBehaviourWeighting{};
    int PacketIndex{};
};
