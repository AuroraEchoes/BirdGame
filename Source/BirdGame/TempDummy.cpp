// Fill out your copyright notice in the Description page of Project Settings.


#include "TempDummy.h"

// Sets default values
ATempDummy::ATempDummy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATempDummy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATempDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

