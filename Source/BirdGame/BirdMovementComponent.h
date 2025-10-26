// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Deque.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BirdMovementComponent.generated.h"

USTRUCT()
struct FBirdSavedMove
{
    GENERATED_BODY()

    FRotator RotationInput;
    FVector Location;
    FRotator Rotation;
    float TimeStamp;
    float DeltaTime;
};


/**
 * 
 */
UCLASS()
class BIRDGAME_API UBirdMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()
public:
    void SetBaseMovementSpeed(const float MovementSpeed);
    void AddRotation(FRotator DeltaRotation);

protected:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(Server, Reliable)
    void ServerMove(FRotator RotationInput, float DeltaTime, float TimeStamp);
    UFUNCTION(Client, Reliable)
    void ClientAdjustPosition(FVector ServerLocation, FRotator ServerRotation, float TimeStamp);

    TArray<FBirdSavedMove> UnacknowledgedMoves;

    float BaseMovementSpeed{200.0f};
    float Velocity{200.0f};
    float TimeStamp{};
    FRotator FrameAccumulatedRotation;
    FRotator CurrentRotation;

private:
    FRotator GetFrameRotation();
};
