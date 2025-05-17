// Fill out your copyright notice in the Description page of Project Settings.


#include "PCGBiomeGen.h"

// Sets default values
APCGBiomeGen::APCGBiomeGen(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APCGBiomeGen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APCGBiomeGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
