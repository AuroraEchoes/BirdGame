// Fill out your copyright notice in the Description page of Project Settings.


#include "Lake.h"

#include "ProceduralFoliage/Grass.h"

// Sets default values
ALake::ALake()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root); //set root component for actor

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void ALake::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

