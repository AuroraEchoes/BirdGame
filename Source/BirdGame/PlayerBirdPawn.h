// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BirdMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerBirdPawn.generated.h"

UCLASS()
class BIRDGAME_API APlayerBirdPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    APlayerBirdPawn();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent) override;

protected:
    
    // Flap to gain height, then glide
    UPROPERTY(EditDefaultsOnly)
    UInputMappingContext* InputMappingContext;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* FlapAction;
    UPROPERTY(EditDefaultsOnly)
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly)
    float LookSensitivity;
    UPROPERTY(EditDefaultsOnly)
    float MaxRollDegrees{30.0f};
    UPROPERTY(EditDefaultsOnly)
    float BaseMovementSpeed{200.0f};

    UPROPERTY()
    UBirdMovementComponent* MovementComponent;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual UPawnMovementComponent* GetMovementComponent() const override;

private:
    void Flap(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
};