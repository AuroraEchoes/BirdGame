// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdMovementComponent.h"

void UBirdMovementComponent::SetBaseMovementSpeed(const float MovementSpeed)
{
    BaseMovementSpeed = MovementSpeed;
}

void UBirdMovementComponent::AddRotation(FRotator DeltaRotation)
{
    AccumulatedRotation += DeltaRotation;
}

void UBirdMovementComponent::TickComponent(float DeltaTime,
                                           enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    const FRotator NewRotation = HandleRotation();
    ApplyRotation(NewRotation);
    if (const FVector DesiredDirection = (ConsumeInputVector().GetClampedToMaxSize(1.0f)) * DeltaTime * BaseMovementSpeed;
        !DesiredDirection.IsNearlyZero())
    {
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredDirection, UpdatedComponent->GetComponentRotation(), true, Hit);
        // TODO: Sliding, maybe?
    }
}

void UBirdMovementComponent::ApplyRotation(const FRotator& NewRotation)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        GetOwner()->SetActorRotation(NewRotation);
    }
    else if (GetOwnerRole() == ROLE_AutonomousProxy)
    {
        ServerApplyRotation(NewRotation);
    }
}

void UBirdMovementComponent::ServerApplyRotation_Implementation(const FRotator NewRotation)
{
    GetOwner()->SetActorRotation(NewRotation);
}


FRotator UBirdMovementComponent::HandleRotation()
{
    const FRotator NewRotation = GetOwner()->GetActorRotation() + AccumulatedRotation;
    AccumulatedRotation = FRotator::ZeroRotator;
    return NewRotation;
}