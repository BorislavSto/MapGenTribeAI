// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkToPointsAI.h"

// Sets default values
AWalkToPointsAI::AWalkToPointsAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// 

}

// Called when the game starts or when spawned
void AWalkToPointsAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWalkToPointsAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AWalkToPointsAI::GetPointLocation(int const index) const
{
	 return WalkPoints[index];
}

int AWalkToPointsAI::Num() const
{
	return WalkPoints.Num();
}

