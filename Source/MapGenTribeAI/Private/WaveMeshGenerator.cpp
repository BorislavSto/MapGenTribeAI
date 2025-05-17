// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveMeshGenerator.h"

void AWaveMeshGenerator::GenerateTerrain()
{
	Super::GenerateMesh();
}

float AWaveMeshGenerator::GetVertexHeight(int32 X, int32 Y)
{
	float Distance = FVector2D(X, Y).Size();
	return Amplitude * FMath::Sin(Distance * Frequency);
}
