// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdMovementComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

void UBirdMovementComponent::SetBaseMovementSpeed(const float MovementSpeed)
{
    BaseMovementSpeed = MovementSpeed;
}

void UBirdMovementComponent::AddRotation(FRotator DeltaRotation)
{
    FrameAccumulatedRotation += DeltaRotation;
}

void UBirdMovementComponent::TickComponent(
    float DeltaTime,
    enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    TimeStamp += DeltaTime;

    const FRotator DeltaRotation = GetFrameRotation();
    CurrentRotation += DeltaRotation;

    FVector ForwardMove = UpdatedComponent->GetForwardVector() * BaseMovementSpeed * DeltaTime;
    FHitResult Hit;
    SafeMoveUpdatedComponent(ForwardMove, UpdatedComponent->GetComponentRotation(), true, Hit);

    if (Hit.IsValidBlockingHit())
    {
        SlideAlongSurface(ForwardMove, 1.f - Hit.Time, Hit.Normal, Hit);
    }

    if (PawnOwner->IsLocallyControlled())
    {
        FBirdSavedMove NewMove(
            DeltaRotation,
            UpdatedComponent->GetComponentLocation(),
            CurrentRotation,
            TimeStamp,
            DeltaTime
        );
        UnacknowledgedMoves.Add(NewMove);
        UpdatedComponent->SetWorldRotation(CurrentRotation);
        ServerMove(DeltaRotation, DeltaTime, TimeStamp);
    }
}

void UBirdMovementComponent::ServerMove_Implementation(FRotator RotationInput, float DeltaTime, float TimeStamp)
{
    CurrentRotation += RotationInput;
    GetOwner()->SetActorRotation(CurrentRotation);

    FVector ForwardMove = UpdatedComponent->GetForwardVector() * BaseMovementSpeed * DeltaTime;
    FHitResult Hit;
    SafeMoveUpdatedComponent(ForwardMove, UpdatedComponent->GetComponentRotation(), true, Hit);

    if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
    {
        ClientAdjustPosition(OwnerPawn->GetActorLocation(), OwnerPawn->GetActorRotation(), TimeStamp);
    }
}

void UBirdMovementComponent::ClientAdjustPosition_Implementation(
    FVector ServerLocation, FRotator ServerRotation, float TimeStamp)
{
    int32 MoveIndex = INDEX_NONE;
    for (int32 i = 0; i < UnacknowledgedMoves.Num(); i++)
    {
        if (FMath::IsNearlyEqual(UnacknowledgedMoves[i].TimeStamp, TimeStamp, SMALL_NUMBER))
        {
            MoveIndex = i;
            break;
        }
    }

    if (MoveIndex != INDEX_NONE)
    {
        const FBirdSavedMove& AckMove = UnacknowledgedMoves[MoveIndex];

        const float ErrorDistSq = FVector::DistSquared(ServerLocation, AckMove.Location);

        if (ErrorDistSq > FMath::Square(5.f))
        {
            UpdatedComponent->SetWorldLocation(ServerLocation);
            UpdatedComponent->SetWorldRotation(ServerRotation);
            CurrentRotation = ServerRotation;

            UnacknowledgedMoves.RemoveAt(0, MoveIndex + 1);

            for (const FBirdSavedMove& Move : UnacknowledgedMoves)
            {
                CurrentRotation += Move.RotationInput;
                UpdatedComponent->SetWorldRotation(ServerRotation);

                FVector ForwardMove = UpdatedComponent->GetForwardVector() * BaseMovementSpeed * GetWorld()->GetDeltaSeconds();
                FHitResult Hit;
                SafeMoveUpdatedComponent(ForwardMove, UpdatedComponent->GetComponentRotation(), true, Hit);

                if (Hit.IsValidBlockingHit())
                {
                    SlideAlongSurface(ForwardMove, 1.f - Hit.Time, Hit.Normal, Hit);
                }
            }
        }
        else
        {
            UnacknowledgedMoves.RemoveAt(0, MoveIndex + 1);
        }
    }
}

FRotator UBirdMovementComponent::GetFrameRotation()
{
    const FRotator DeltaRotation = FrameAccumulatedRotation;
    FrameAccumulatedRotation = FRotator::ZeroRotator;
    return DeltaRotation;
}