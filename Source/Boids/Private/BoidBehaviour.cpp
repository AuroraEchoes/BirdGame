// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidBehaviour.h"

#include "BoidFlock.h"

FBoidBehaviourDirectionInfo UBoidBehaviour::GetDesiredDirection_Implementation(ABoidFlock* Flock, ABoidBase* Boid)
{
    return FBoidBehaviourDirectionInfo{FVector::Zero(), 0.0};
}

double UBoidBehaviour::GetWeighting() const
{
    return Weighting;
}

double UBoidBehaviour::GetNeighbourRadius() const
{
    return NeighbourRadius;
}

FColor UBoidBehaviour::GetDebugColor() const
{
    return DebugColor;
}

bool UBoidBehaviour::ShouldDebugVisualise() const
{
    return DebugVisualise;
}

TArray<ABoidBase*> UBoidBehaviour::GetNeighbours(ABoidFlock* Flock,
                                             const ABoidBase* Boid) const
{
    return Flock->GetNearbyBoidLocations(Boid, NeighbourRadius);
}