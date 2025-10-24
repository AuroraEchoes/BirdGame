// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLandscape.h"

#include "EngineUtils.h"
#include "Goal.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralLake.h"
#include "ProceduralFoliage/Grass.h"
#include "ProceduralTree/ProceduralTree.h"
#include "ProceduralTree/TreeBase.h"

// Sets default values
AProceduralLandscape::AProceduralLandscape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	SetRootComponent(ProceduralMesh);
}

// Called when the game starts or when spawned
void AProceduralLandscape::BeginPlay()
{
	Super::BeginPlay();
	SpawnGoalPoint();
	ProceduralTree->SpawnTrees();
	ProceduralGrass->SpawnGrasses();
	ProceduralLake->SpawnLakes();
}


void AProceduralLandscape::CreateSimplePlane()
{
	ClearLandscape();
	Vertices.Append(
		{
		FVector::Zero(),
		FVector(1000.0f, 0.0f, FMath::RandRange(-500.0f, 500.0f)),
		FVector(0.0f, 1000.0f, FMath::RandRange(-500.0f, 500.0f)),
		FVector(1000.0f, 1000.0f, FMath::RandRange(-500.0f, 500.0f)),
		});
	for (auto Vertex : Vertices)
	{
		DrawDebugSphere(GetWorld(), Vertex, 50.0f, 4, FColor::Blue, true);
	}
	Triangles.Append({0,2,1,1,2,3});
	UVCoords.Append({
		FVector2D::Zero(),
		FVector2D::UnitX(),
		FVector2D::UnitY(),
		FVector2D::One()
	});
	if (ProceduralMesh)
	{
		ProceduralMesh->CreateMeshSection(0,
			Vertices, Triangles, TArray<FVector>(),
			UVCoords, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	}
}

bool AProceduralLandscape::ShouldTickIfViewportsOnly() const
{
	return true;
}

// Called every frame
void AProceduralLandscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRegenerate)
	{
		GenerateLandscape();
		bShouldRegenerate = false;
	}

}

void AProceduralLandscape::ClearLandscape()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();
	TreeSpawnPoints.Empty();
	GrassSpawnPoints.Empty();
	FlatSpawnPoints.Empty();
	PotentialGoalPoints.Empty();

	Goal->Destroy();

	for (ATreeBase* SpawnedTree : ProceduralTree->SpawnedTrees) //do a loop for grass, branch, and leaves
	{
		SpawnedTree->Destroy();
	}
	ProceduralTree->SpawnedTrees.Empty();
	
	if (ProceduralMesh)
	{
		ProceduralMesh->ClearMeshSection(0);
	}
	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());
}

FVector AProceduralLandscape::GetRandPointInTriangle(const FVector& A, const FVector& B, const FVector& C)
{
	float r1 = FMath::FRand();
	float r2 = FMath::FRand();

	// use square root to ensure uniform distribution
	float u = 1 - FMath::Sqrt(r1);
	float v = r2 * FMath::Sqrt(r1);
	float w = 1 - u - v;

	return u*A + v*B + w*C; // uses barycentric coordinates to ensure the point is within the triangle mesh
}

void AProceduralLandscape::GenerateSpawnPoints(TArray<FVector>& TerrainSpawnPoints, int32 MaxFrequency, int32 MinFrequency)
{
	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		int32 SpawnPointFrequency = FMath::RandRange(MinFrequency, MaxFrequency);
		for (int32 n = 0; n < SpawnPointFrequency; n++)
		{
			FVector P = GetRandPointInTriangle(
				Vertices[Triangles[i]], Vertices[Triangles[i+1]], Vertices[Triangles[i+2]]);
			TerrainSpawnPoints.Add(P);
		}
	}
}

void AProceduralLandscape::GenerateTreeSpawnPoints()
{
	GenerateSpawnPoints(TreeSpawnPoints, MaxTreeFrequency, MinTreeFrequency);

	for (auto Vertex : TreeSpawnPoints)
	{
		DrawDebugSphere(GetWorld(), Vertex, 25.0f, 4, FColor::Red, true);
	}
}

void AProceduralLandscape::GenerateGrassSpawnPoints()
{
	GenerateSpawnPoints(GrassSpawnPoints, MaxGrassFrequency, MinGrassFrequency);

	for (auto Vertex : GrassSpawnPoints)
	{
		DrawDebugSphere(GetWorld(), Vertex, 25.0f, 4, FColor::Green, true);
	}
}

void AProceduralLandscape::GenerateQuadSpawnPoints()
{
	float ZTolerance = 10.0f; //the points can have a 10 pixel z difference

	for (int32 i = 0; i < Width - 1; i++)
	{
		for (int32 j = 0; j < Height - 1; j++)
		{
			int32 V0 = i + Width * j;
			int32 V1 = i + Width * (j + 1);
			int32 V2 = i + 1 + Width * j;
			int32 V3 = i + 1 + Width * (j + 1);

			float AvgZ = (Vertices[V0].Z + Vertices[V1].Z + Vertices[V2].Z + Vertices[V3].Z) / 4.0f;

			bool bIsFlat =
				FMath::Abs(Vertices[V0].Z - AvgZ) < ZTolerance &&
				FMath::Abs(Vertices[V1].Z - AvgZ) < ZTolerance &&
				FMath::Abs(Vertices[V2].Z - AvgZ) < ZTolerance &&
				FMath::Abs(Vertices[V3].Z - AvgZ) < ZTolerance;

			if (bIsFlat)
			{
				float SpawnFrequency = 0.5f; //50% chance of lake spawning on a flat square.
				FVector Center = (Vertices[V0] + Vertices[V1] + Vertices[V2] + Vertices[V3]) / 4.0f;
				
				if (FMath::FRandRange(0.0f, 1.0f) <= SpawnFrequency)
				{
					//FVector Center = (Vertices[V0] + Vertices[V1] + Vertices[V2] + Vertices[V3]) / 4.0f;
					FlatSpawnPoints.Add(Center);

					DrawDebugSphere(GetWorld(), Center, 50.f, 6, FColor::Yellow, true); //center of spawnpoint

					DrawDebugSphere(GetWorld(), Center, 500, 12, FColor::Cyan, true, 10.f); //radius to destroy foliage
					for (int32 k = GrassSpawnPoints.Num() - 1; k >= 0; k--)
					{
						float Dist = FVector::Dist(GrassSpawnPoints[k], Center);
						if (Dist < 500)
						{
							GrassSpawnPoints.RemoveAt(k);
						}
					}
				}

				else
				{
					PotentialGoalPoints.Add(Center);
					DrawDebugSphere(GetWorld(), Center, 50.f, 6, FColor::White, true); //signals this is a flat quad, but is not chosen as spawnpoint for lakes.
				}
			}
			
		}
	}
}

void AProceduralLandscape::GenerateLandscape()
{
	ClearLandscape();
	PerlinOffset = FMath::RandRange(-1'000'000.0f,1'000'000.0f);
	for (auto i=0; i < Width; i++)
	{
		for (auto j=0; j < Height; j++)
		{
			float X = i*VertexSpacing;
			float Y = j*VertexSpacing;
			
			float Z = PerlinScale*FMath::PerlinNoise2D(PerlinRoughness*FVector2D(X + PerlinOffset,Y + PerlinOffset));
			
			if (Z <= FlatTerrainHeight ) //make area flat at a set world z value.
			{
				Z = FlatTerrainHeight;
			}

			Vertices.Add(FVector(X, Y, Z));
			UVCoords.Add(FVector2D(i,j));

			// If this vertex isn't located at an edge in the generating direction, then construct a square made up of 2 triangles.
			if (i!=Width-1 && j!=Height-1)
			{
				int32 V0 = i + Width*j;
				int32 V1 = i + Width*(j+1);
				int32 V2 = i+1 + Width*j;
				int32 V3 = i+1 + Width*(j+1);
				
				//first triangle
				Triangles.Append({V0, V2, V1});

				//second triangle
				Triangles.Append({V1, V2, V3});
			}
		}
	}
	for (auto Vertex : Vertices)
	{
		DrawDebugSphere(GetWorld(), Vertex, 50.0f, 4, FColor::Blue, true);
	}
	
	GenerateTreeSpawnPoints();
	GenerateGrassSpawnPoints();
	GenerateQuadSpawnPoints();
	
	// For tangents and normal computation, we use the CalculateTangentsForMesh() function of the UKismetProceduralMeshLibrary.
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices,Triangles,UVCoords,Normals,Tangents);

	// Create the mesh section using the TArrays that have been populated.
	// We leave VertexColors empty as we color the vertices using a material shader.
	if (ProceduralMesh)
	{
		ProceduralMesh->CreateMeshSection(0,
			Vertices, Triangles, Normals,
			UVCoords, TArray<FColor>(), Tangents, true);
	}
}

void AProceduralLandscape::SpawnGoalPoint()
{
	FVector GoalLocation = FVector::ZeroVector;
	if (PotentialGoalPoints.Num() > 0) //choose a random spawn point on a flat surface to spawn the goal
	{
		int32 RandomIndex = FMath::RandRange(0, PotentialGoalPoints.Num() - 1);
		GoalLocation = PotentialGoalPoints[RandomIndex];
	}
	else
	{
		//get the last spawn point from FlatSpawnPoints and pop it off
		GoalLocation = FlatSpawnPoints.Last();
		FlatSpawnPoints.Pop();
	}

	//Spawn in the goal
	if (GoalClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FRotator Rotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GoalLocation);
		SpawnTransform.SetRotation(FQuat(Rotation));
		
		Goal = GetWorld()->SpawnActor<AGoal>(GoalClass, SpawnTransform, SpawnParams);

		if (Goal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal spawned: %s"), *Goal->GetName());
		}
	}
}

