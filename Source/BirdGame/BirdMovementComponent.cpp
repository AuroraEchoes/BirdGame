// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdMovementComponent.h"

void UBirdMovementComponent::SetBaseMovementSpeed(const float MovementSpeed)
{
    BaseMovementSpeed = MovementSpeed;
}

void UBirdMovementComponent::TickComponent(float DeltaTime,
                                           enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (const FVector DesiredDirection = (ConsumeInputVector().GetClampedToMaxSize(1.0f)) * DeltaTime * BaseMovementSpeed;
        !DesiredDirection.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredDirection, UpdatedComponent->GetComponentRotation(), true, Hit);
        // TODO: Sliding, maybe?
    }
}