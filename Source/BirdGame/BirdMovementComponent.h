// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BirdMovementComponent.generated.h"

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
    void ServerApplyRotation(FRotator NewRotation);
    void ApplyRotation(const FRotator& NewRotation);
    void ApplyRotationImplementation(const FRotator& NewRotation);

    float BaseMovementSpeed{200.0f};
    FRotator AccumulatedRotation;

private:
    FRotator HandleRotation();
};
