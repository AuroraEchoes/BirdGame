// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdMovementComponent.h"

void UBirdMovementComponent::TickComponent(float DeltaTime,
    enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (const FVector DesiredDirection = (ConsumeInputVector().GetClampedToMaxSize(1.0f)) * DeltaTime * 200.0f;
        !DesiredDirection.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredDirection, UpdatedComponent->GetComponentRotation(), true, Hit);
        // TODO: Sliding, maybe?
    }
}