#include "BoidBase.h"

#include "Net/UnrealNetwork.h"

ABoidBase::ABoidBase()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABoidBase::SetActorLocationRotation_Implementation(FVector Location, FRotator Rotation, float TimeStamp)
{
    if (HasAuthority())
    {
        this->SetActorLocationAndRotation(Location, Rotation);
    }
    else
    {
        if (TimeStamp < LatestTimestamp)
            return;
        LatestTimestamp = TimeStamp;
        const FBufferedBoidState NewState(Location, Rotation, TimeStamp);
        BufferedStates.Add(NewState);
    }
}

void ABoidBase::BeginPlay()
{
    Super::BeginPlay();
    SetReplicateMovement(false);
}

void ABoidBase::SendBoidLocation_Implementation(FVector Location, FRotator Rotation, int PacketIndex, float TimeStamp)
{
    if (!HasAuthority())
    {
        const FBufferedBoidState NewState(Location, Rotation, TimeStamp);
        BufferedStates.Add(NewState);
    }
}

void ABoidBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (HasAuthority()) return;
    if (BufferedStates.Num() < 2) return;

    const float Now = GetWorld()->GetTimeSeconds();
    const float TargetTime = Now - BufferDelaySeconds;

    // Find interpolation window
    int Index = 0;
    while (Index < BufferedStates.Num() - 1 && BufferedStates[Index + 1].TimeStamp < TargetTime)
    {
        Index++;
    }

    if (Index < BufferedStates.Num() - 1)
    {
        const FBufferedBoidState& Older = BufferedStates[Index];
        const FBufferedBoidState& Newer = BufferedStates[Index + 1];

        const float Alpha = FMath::GetRangePct(Older.TimeStamp, Newer.TimeStamp, TargetTime);

        FVector TargetLoc = FMath::Lerp(Older.Location, Newer.Location, Alpha);
        FQuat TargetRot = FQuat::Slerp(Older.Rotation.Quaternion(), Newer.Rotation.Quaternion(), Alpha);

        FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaSeconds, InterpSpeed);
        FQuat NewRot = FQuat::Slerp(GetActorQuat(), TargetRot, DeltaSeconds * InterpSpeed);

        SetActorLocationAndRotation(NewLoc, NewRot.Rotator());
    }
    else
    {
        const FBufferedBoidState& Last = BufferedStates.Last();
        SetActorLocationAndRotation(Last.Location, Last.Rotation);
    }

    while (BufferedStates.Num() > 10)
    {
        BufferedStates.RemoveAt(0);
    }
}