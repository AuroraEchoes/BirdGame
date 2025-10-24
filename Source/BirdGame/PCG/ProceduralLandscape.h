// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralFoliage/ProceduralGrass.h"
#include "ProceduralLandscape.generated.h"

class AGoal;
class AProceduralLake;
class AProceduralTree;
class ATreeBase;
class ATree;
class ANavigationNode;
class UProceduralMeshComponent;

UCLASS()
class BIRDGAME_API AProceduralLandscape : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralLandscape();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	/// <summary>
	/// Create a flat, simple plane procedurally.
	/// </summary>
	void CreateSimplePlane();

	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector2D> UVCoords;

	UPROPERTY()
	TArray<int32> FlatTerrainTriangles; //stores triangles that are flat?
	

	virtual bool ShouldTickIfViewportsOnly() const override;

	UPROPERTY(EditAnywhere)
	bool bShouldRegenerate;

	UPROPERTY(EditAnywhere, meta=(ClampMin="1"))
	int32 Width = 10;
	UPROPERTY(EditAnywhere, meta=(ClampMin="1"))
	int32 Height = 10;
	UPROPERTY(EditAnywhere, meta=(ClampMin="1.0"))
	float VertexSpacing = 1000.0f;

	UPROPERTY(EditAnywhere, meta=(ClampMin="1.0"))
	float PerlinScale = 1000.0f;
	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0"))
	float PerlinRoughness = 0.00012f;
	UPROPERTY(VisibleAnywhere)
	float PerlinOffset;

	UPROPERTY(EditAnywhere)
	float FlatTerrainHeight = 0.1f;

	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int32 MinTreeFrequency = 1; //determines the amount of min spawn points per triangle
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int32 MaxTreeFrequency = 3; //determines the amount of max spawn points per triangle

	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int32 MinGrassFrequency = 1; //determines the amount of min spawn points per triangle
	
	UPROPERTY(EditAnywhere, meta=(ClampMin=1))
	int32 MaxGrassFrequency = 5; //determines the amount of max spawn points per triangle

	FVector GetRandPointInTriangle(const FVector& A, const FVector& B, const FVector& C);

	// UPROPERTY(EditDefaultsOnly)
	// TSubclassOf<ATree> TreeClass; //getting the blueprint class of tree
	//
	// UPROPERTY()
	// ATree* Tree; //stores the instance of tree class here

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGoal> GoalClass; //getting the blueprint class of the goal
	
	UPROPERTY()
	AGoal* Goal; //stores the instance of the goal class here

	UPROPERTY(EditAnywhere)
	AProceduralTree* ProceduralTree; //get the procedural tree spawner actor from outliner

	UPROPERTY(EditAnywhere)
	AProceduralGrass* ProceduralGrass; //get the procedural grass spawner actor from outliner
	
	UPROPERTY(EditAnywhere)
	AProceduralLake* ProceduralLake; //get the procedural lake spawner actor from outliner
	

public:	
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Clear the landscape by emptying all relevant TArrays and clearing Mesh Sections. 
	/// </summary>
	void ClearLandscape();

	
	/// Creates spawn points
	void GenerateSpawnPoints(TArray<FVector>& TerrainSpawnPoints, int32 MaxFrequency, int32 MinFrequency);

	/// Creates spawn points within each triangle for trees
	void GenerateTreeSpawnPoints();

	/// Creates spawn points within each triangle for grass
	void GenerateGrassSpawnPoints();
	
	/// Creates spawn points in a square that is flat
	void GenerateQuadSpawnPoints();

	///<summary>
	/// Clears the Landscape and regenerates all vertices and nodes procedurally.
	///</summary>
	void GenerateLandscape();

	void SpawnGoalPoint();

	UPROPERTY(VisibleAnywhere) 
	TArray<FVector> TreeSpawnPoints; //stores spawn points for trees

	UPROPERTY(VisibleAnywhere) 
	TArray<FVector> GrassSpawnPoints; //stores spawn points for grass

	//rock spawn points?
	UPROPERTY(VisibleAnywhere) 
	TArray<FVector> FlatSpawnPoints; //stores spawn points on flat squares for lakes and grassland for feeding
};
