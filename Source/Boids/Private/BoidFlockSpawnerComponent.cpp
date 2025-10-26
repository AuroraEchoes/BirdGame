// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidFlockSpawnerComponent.h"

#include "BoidFlock.h"

// Sets default values for this component's properties
UBoidFlockSpawnerComponent::UBoidFlockSpawnerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(false);

    // ...
}


// Called when the game starts
void UBoidFlockSpawnerComponent::BeginPlay()
{
    Super::BeginPlay();
    if (GetOwnerRole() == ROLE_Authority)
        SpawnBoids();
}

void UBoidFlockSpawnerComponent::SpawnBoids()
{
    if (ABoidFlock* Flock = Cast<ABoidFlock>(GetOwner()))
    {
        for (const TSubclassOf<UBoidBehaviour>& BehaviourClass : InitialBoidBehaviours)
        {
            Flock->AddBoidBehaviour(BehaviourClass);
        }
        
        for (int I = 0; I < BoidsToSpawn; I++)
        {
            const FVector SpawnLocation = GetBoidSpawnLocation(I);
            UChildActorComponent* ChildComponent = NewObject<
                UChildActorComponent>(this);
            ChildComponent->RegisterComponent();
            ChildComponent->SetChildActorClass(BoidInstanceClass);
            ChildComponent->CreateChildActor();
            ChildComponent->SetWorldLocation(SpawnLocation);
            Flock->AddSpawnedBoid(Cast<ABoidBase>(ChildComponent->GetChildActor()));
            FString SpawnLocStr = SpawnLocation.ToString();
            UE_LOG(LogTemp, Display, TEXT("Spawning boid at %s"), *SpawnLocStr);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Tried to spawn boids from a BoidFlockSpawnerComponent without a BoidFlock parent"))
    }
}

FVector UBoidFlockSpawnerComponent::GetBoidSpawnLocation(const int& BoidIndex) const
{
    // Phi is the golden angle (but we can’t calculate it constexpr; damn you Unreal)!
    constexpr double Phi = 3.883222077;
    const double Y = 1.0 - (static_cast<double>(BoidIndex) / (static_cast<double>(BoidsToSpawn) - 1.0)) * 2.0;
    const double Radius = FMath::Sqrt(1.0 - FMath::Pow(Y, 2));
    const double Theta = Phi * static_cast<double>(BoidIndex);
    const double X = FMath::Cos(Theta) * Radius;
    const double Z = FMath::Sin(Theta) * Radius;
    const FVector RelativeUnitSpawnLocation = FVector(X, Y, Z);
    return GetOwner()->GetActorLocation() + RelativeUnitSpawnLocation * SpawnRadius;
}

// Called every frame
void UBoidFlockSpawnerComponent::TickComponent(float DeltaTime,
                                               ELevelTick TickType,
                                               FActorComponentTickFunction*
                                               ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

double UBoidFlockSpawnerComponent::GetSpawnRadius() const
{
    return SpawnRadius;
}

