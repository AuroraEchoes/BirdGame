// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoidBehaviour.generated.h"

class ABoidFlock;

USTRUCT(BlueprintType)
struct FBoidBehaviourDirectionInfo
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite)
    FVector DesiredDirection{};
    UPROPERTY(BlueprintReadWrite)
    double RelativeWeighting{};
};

UCLASS(Blueprintable, BlueprintType)
class BOIDS_API UBoidBehaviour : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    FBoidBehaviourDirectionInfo GetDesiredDirection(ABoidFlock* Flock, APawn* Boid);
    double GetWeighting() const;
    double GetNeighbourRadius() const;
    FColor GetDebugColor() const;
    bool ShouldDebugVisualise() const;

protected:
    UPROPERTY(EditAnywhere)
    double Weighting = 1.0;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    double NeighbourRadius = 1000.0f;
    UPROPERTY(EditAnywhere)
    FColor DebugColor = FColor::Cyan;
    UPROPERTY(EditAnywhere)
    bool DebugVisualise;

    UFUNCTION(BlueprintCallable)
    TArray<APawn*> GetNeighbours(ABoidFlock* Flock, const APawn* Boid) const;
};
