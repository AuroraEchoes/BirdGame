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
 
protected:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    float BaseMovementSpeed{200.0f};
};
