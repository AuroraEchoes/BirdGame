#include "BoidMovementComponent.h"

void UBoidMovementComponent::SetForwardVector(const FVector& ForwardVector)
{
    this->ForwardVector = ForwardVector;
}

void UBoidMovementComponent::SetMaxTurnRateDegrees(
    const float MaxTurnRateDegrees)
{
    this->MaxTurnRateDegrees = MaxTurnRateDegrees;
}

void UBoidMovementComponent::SetMovementSpeed(const float MovementSpeed)
{
    this->MovementSpeed = MovementSpeed;
}

void UBoidMovementComponent::TickComponent(float DeltaTime,
    enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (GetOwnerRole() == ROLE_Authority)
    {
        const FRotator DesiredRotation = ForwardVector.Rotation();
        const FRotator CurrentRotation = PawnOwner->GetActorRotation();
        const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, DesiredRotation, DeltaTime, MaxTurnRateDegrees);
        UpdatedComponent->SetWorldRotation(NewRotation);
        const FVector DeltaMovement = PawnOwner->GetActorForwardVector() * MovementSpeed * DeltaTime;
        FHitResult Hit;
        SafeMoveUpdatedComponent(DeltaMovement, UpdatedComponent->GetComponentRotation(), true, Hit);
        ClientSetPosition(UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentRotation());
    }
    else
    {
        UpdatedComponent->SetWorldLocation(FMath::VInterpTo(UpdatedComponent->GetComponentLocation(), TargetLocation, DeltaTime, MovementSpeed));
        UpdatedComponent->SetWorldRotation(FMath::RInterpTo(UpdatedComponent->GetComponentRotation(), TargetRotation, DeltaTime, MaxTurnRateDegrees));
    }
}

void UBoidMovementComponent::ClientSetPosition_Implementation(const FVector Location, const FRotator Rotation)
{
    TargetLocation = Location;
    TargetRotation = Rotation;
    // UpdatedComponent->SetWorldLocation(Location);
    // UpdatedComponent->SetWorldRotation(Rotation);
}