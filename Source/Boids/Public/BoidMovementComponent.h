#pragma once
#include "GameFramework/PawnMovementComponent.h"
#include "BoidMovementComponent.generated.h"

UCLASS()
class UBoidMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()
    
public:
    void SetForwardVector(const FVector& ForwardVector);
    void SetMaxTurnRateDegrees(const float MaxTurnRateDegrees);
    void SetMovementSpeed(const float MovementSpeed);
    
protected:
    FVector TargetLocation;
    FRotator TargetRotation;
    FVector ForwardVector;
    float MovementSpeed;
    float MaxTurnRateDegrees;

    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    UFUNCTION(NetMulticast, Reliable)
    void ClientSetPosition(const FVector Location, const FRotator Rotation);
};
