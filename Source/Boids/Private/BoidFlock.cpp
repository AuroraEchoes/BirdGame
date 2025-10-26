// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidFlock.h"

#include "BoidMovementComponent.h"
#include "UnrealEngine.h"

// Sets default values
ABoidFlock::ABoidFlock()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SetReplicates(true);
    SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Transform"));
    SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ABoidFlock::BeginPlay() { Super::BeginPlay(); }

void ABoidFlock::TickControllingBoids_Implementation(const float& DeltaTime)
{
    if (HasAuthority())
    {
        for (ABoidBase* Boid : ControllingBoids)
        {
            TickControllingBoid(DeltaTime, Boid);
        }
    }
}


// Called every frame
void ABoidFlock::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority())
    {
        TickControllingBoids(DeltaTime);
        PacketIndex++;
    }
}

void ABoidFlock::TickControllingBoid_Implementation(const float& DeltaTime,
    ABoidBase* Boid)
{
    if (HasAuthority())
    {
        FVector DesiredDirection{};
        for (UBoidBehaviour* Behaviour : BoidBehaviours)
        {
            const FBoidBehaviourDirectionInfo DirectionInfo = Behaviour->GetDesiredDirection(this, Boid);
            FVector BehaviourDesiredDirection = DirectionInfo.DesiredDirection;
            BehaviourDesiredDirection.Normalize();
            BehaviourDesiredDirection *= DirectionInfo.RelativeWeighting;
            if (DebugVisualiseAllBehaviours || Behaviour->ShouldDebugVisualise())
            {
                DebugVisualiseBehaviour(Boid, Behaviour, BehaviourDesiredDirection);
            }
            const double Weighting = Behaviour->GetWeighting() / TotalBehaviourWeighting;
            DesiredDirection += BehaviourDesiredDirection * Weighting;
        }
        DesiredDirection.Normalize();
        const FRotator DesiredRotation = DesiredDirection.Rotation();
        const FRotator CurrentRotation = Boid->GetActorRotation();
        const FRotator NewRotation = FMath::RInterpConstantTo(CurrentRotation, DesiredRotation, DeltaTime, MaxBoidTurnRateDegrees);
        const FVector NewLocation = (Boid->GetActorForwardVector() * MaxBoidSpeed * DeltaTime) + Boid->GetActorLocation();
        Boid->SetActorLocationAndRotation(NewLocation, NewRotation);
        // Boid->SetActorLocationRotation(NewLocation, NewRotation, GetWorld()->GetTimeSeconds());
        // Boid->SendBoidLocation(NewLocation, NewRotation, PacketIndex, GetWorld()->GetTimeSeconds());
    }
}

// TODO: Some sort of memoisation
TArray<FVector> ABoidFlock::GetCollisionSweepPoints(int NumPoints)
{
    TArray<FVector> Points;
    for (int I = 0; I < NumPoints; I++)
    {
        // Phi is the golden angle (but we can’t calculate it constexpr; damn you Unreal)!
        constexpr double Phi = 3.883222077;
        const double Y = 1.0 - (static_cast<double>(I) / (static_cast<double>(NumPoints) - 1.0)) * 2.0;
        const double Radius = FMath::Sqrt(1.0 - FMath::Pow(Y, 2));
        const double Theta = Phi * static_cast<double>(I);
        const double X = FMath::Cos(Theta) * Radius;
        const double Z = FMath::Sin(Theta) * Radius;
        const FVector PointLocation = FVector(X, Y, Z);
        Points.Add(PointLocation);
    }
    return Points;
}

void ABoidFlock::AddSpawnedBoid(ABoidBase* Boid)
{
    ControllingBoids.Add(Boid);
    // Boid->SetMaxTurnRateDegrees(MaxBoidTurnRateDegrees);
    // Boid->SetMovementSpeed(MaxBoidSpeed);
}

TArray<ABoidBase*> ABoidFlock::GetNearbyBoidLocations(const ABoidBase* Boid, const double Distance)
{
    TArray<ABoidBase*> NearbyBoids{};
    if (Boid)
    {
        const FVector Location = Boid->GetActorLocation();
        const double DistanceSquared = Distance * Distance;
        for (ABoidBase* NearbyBoid : ControllingBoids)
        {
            if (NearbyBoid)
            {
                FVector NearbyBoidLocation = NearbyBoid->GetActorLocation();
                if (FVector::DistSquared(Location, NearbyBoidLocation) <= DistanceSquared)
                {
                    NearbyBoids.Add(NearbyBoid);
                }
            }
        }
    }
    return NearbyBoids;
}

APawn* ABoidFlock::GetPlayerLocation()
{
    return GetWorld()->GetFirstLocalPlayerFromController()->GetPlayerController(GetWorld())->GetPawn();
}

TArray<APawn*> ABoidFlock::GetAllPlayers() const
{
    TArray<APawn*> Pawns{};
    for (TPlayerControllerIterator<APlayerController>::ServerAll It(GetWorld()); It; ++It)
    {
        APlayerController* Controller = *It;
        if (APawn* Pawn = Controller->GetPawn())
        {
            Pawns.Add(Pawn);
        }
    }
    return Pawns;
}

FVector ABoidFlock::GetClosestUnobstructedDirection(const ABoidBase* Boid, double Distance, int NumPoints)
{
    auto SweepAlongVector = [&](const FVector& Direction) -> double
    {
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(Boid);
        FHitResult Hit;
        const FVector EndLocation = Boid->GetActorLocation() + Direction * Distance;
        const bool DidHit = GetWorld()->LineTraceSingleByChannel(Hit, Boid->GetActorLocation(), EndLocation, ECC_GameTraceChannel1, QueryParams);
        return DidHit ? Hit.Distance : INFINITY;
    };
    // If going ahead is fine, let’s just do that
    if (SweepAlongVector(Boid->GetActorForwardVector()) >= Distance)
    {
        return Boid->GetActorForwardVector();
    }
    // Otherwise let’s check points
    const FVector& BoidForward = Boid->GetActorForwardVector();
    double MaxPointDistance = 0;
    FVector ClosestCurrentVector;
    for (FVector Point : GetCollisionSweepPoints(NumPoints))
    {
        if
        (
            const double PointDistance = SweepAlongVector(Point);
            PointDistance > MaxPointDistance
            || FVector::Dist(BoidForward, Point) < FVector::Dist(BoidForward, ClosestCurrentVector)
        )
        {
            MaxPointDistance = PointDistance;
            ClosestCurrentVector = Point;
        }
    }
    // DrawDebugLine(GetWorld(), Boid->GetActorLocation(), Boid->GetActorLocation() + Boid->GetActorForwardVector() * 500.0f, FColor::Red, false, -1, 0, 5);
    // DrawDebugLine(GetWorld(), Boid->GetActorLocation(), Boid->GetActorLocation() + ClosestCurrentVector * 500.0f, FColor::Green, false, -1, 0, 5);
    return ClosestCurrentVector;
}

AGameStateBase* ABoidFlock::GetGameState()
{
    return GetWorld()->GetGameState();
}

UWorld* ABoidFlock::GetWorldContext() const
{
    return GetWorld();
}

void ABoidFlock::AddBoidBehaviour(const TSubclassOf<UBoidBehaviour>& BehaviourClass)
{
    UBoidBehaviour* Behaviour = NewObject<UBoidBehaviour>(this, BehaviourClass);
    TotalBehaviourWeighting += Behaviour->GetWeighting();
    BoidBehaviours.Add(Behaviour);
}

void ABoidFlock::DebugVisualiseBehaviour(const ABoidBase* Boid, const UBoidBehaviour* Behaviour, const FVector& DesiredDirection) const
{
    const double NeighbourRadius = Behaviour->GetNeighbourRadius();
    const int Segments = static_cast<int>(FMath::Max(25.0, NeighbourRadius / 50.0));
    DrawDebugSphere(GetWorld(), Boid->GetActorLocation(), NeighbourRadius, Segments, Behaviour->GetDebugColor(), false);
    const FVector ArrowEnd = Boid->GetActorLocation() + DesiredDirection * (NeighbourRadius + 300.0f);
    DrawDebugDirectionalArrow(GetWorld(), Boid->GetActorLocation(), ArrowEnd, 300.0f, Behaviour->GetDebugColor(), false, -1, 0, 3);
}