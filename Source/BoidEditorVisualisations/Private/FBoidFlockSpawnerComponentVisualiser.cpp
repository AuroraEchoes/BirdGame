#include "FBoidFlockSpawnerComponentVisualiser.h"

#include "BoidFlockSpawnerComponent.h"

void FBoidFlockSpawnerComponentVisualiser::DrawVisualization(
    const UActorComponent* Component, const FSceneView* View,
    FPrimitiveDrawInterface* PDI)
{
    if (const UBoidFlockSpawnerComponent* BoidFlock = Cast<UBoidFlockSpawnerComponent>(Component))
    {
        const double SpawnRadius = BoidFlock->GetSpawnRadius();
        const FVector& Center = BoidFlock->GetOwner()->GetActorLocation();
        const int32 NumSubdivisions = FMath::Max(SpawnRadius / 50, 25);
        DrawWireSphere(PDI, Center, FLinearColor::Green, SpawnRadius, NumSubdivisions, 0);
    }
}