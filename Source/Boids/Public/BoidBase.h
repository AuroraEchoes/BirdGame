#pragma once
#include "BoidMovementComponent.h"
#include "Containers/Deque.h"
#include "BoidBase.generated.h"

USTRUCT()
struct FBufferedBoidState
{
    GENERATED_BODY()
    UPROPERTY()
    FVector Location{};
    UPROPERTY()
    FRotator Rotation{};
    UPROPERTY()
    float TimeStamp{};
};

USTRUCT()
struct FLocationRotation
{
    GENERATED_BODY()
    UPROPERTY()
    FVector Location{};
    FRotator Rotation{};
};

UCLASS()
class ABoidBase : public APawn
{
    GENERATED_BODY()
public:
    ABoidBase();
    UFUNCTION(NetMulticast, Reliable)
    void SendBoidLocation(FVector Location, FRotator Rotation, int PacketIndex, float TimeStamp);
    
    UFUNCTION(NetMulticast, Reliable)
    void SetActorLocationRotation(FVector Location, FRotator Rotation, float TimeStamp);
    virtual void BeginPlay() override;
  
protected:
    TArray<FBufferedBoidState> BufferedStates;
    UPROPERTY(EditAnywhere)
    float BufferDelaySeconds = 0.2f;
    UPROPERTY(EditAnywhere)
    float InterpSpeed = 10.0f;
    void Tick(float DeltaSeconds) override;
    float LatestTimestamp{};
};
