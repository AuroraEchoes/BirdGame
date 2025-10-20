// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBirdPawn.h"

#include "BirdMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/MovementComponent.h"

// Sets default values
APlayerBirdPawn::APlayerBirdPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    MovementComponent = CreateDefaultSubobject<UBirdMovementComponent>(TEXT("Bird Movement Component"));
    MovementComponent->UpdatedComponent = RootComponent;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = true;
    bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void APlayerBirdPawn::BeginPlay()
{
    Super::BeginPlay();

    if (MovementComponent)
    {
        MovementComponent->SetBaseMovementSpeed(BaseMovementSpeed);
    }
    if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        const TObjectPtr<APlayerCameraManager>& CameraManager = PlayerController->PlayerCameraManager;
        CameraManager->ViewRollMin = -MaxRollDegrees;
        CameraManager->ViewRollMax = MaxRollDegrees;
        if (UEnhancedInputLocalPlayerSubsystem* EnhancedInput =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            EnhancedInput->AddMappingContext(InputMappingContext, 0);
        }
    }
}

UPawnMovementComponent* APlayerBirdPawn::GetMovementComponent() const
{
    return MovementComponent;
}

// Called every frame
void APlayerBirdPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddMovementInput(GetActorForwardVector());
    const float LateralLiftRatio = GetActorRotation().Roll / MaxRollDegrees;
    AddControllerYawInput(LateralLiftRatio);
}

// Called to bind functionality to input
void APlayerBirdPawn::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        InputComponent->BindAction(FlapAction, ETriggerEvent::Triggered, this, &APlayerBirdPawn::Flap);
        InputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerBirdPawn::Look);
    }
}

void APlayerBirdPawn::Flap(const FInputActionValue& Value)
{
    AddMovementInput(FVector::UpVector);
    // Move in the forward direction and upwards
}

void APlayerBirdPawn::Look(const FInputActionValue& Value)
{
    const FVector2D LookVector = Value.Get<FVector2D>() * LookSensitivity;
    AddControllerRollInput(FMath::Clamp(LookVector.X, -0.25f, 0.25f));
    AddControllerPitchInput(LookVector.Y);
}