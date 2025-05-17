// Fill out your copyright notice in the Description page of Project Settings.


#include "StairMeshGenerator.h"

float AStairMeshGenerator::GetVertexHeight(int32 X, int32 Y)
{
	int32 StepIndex = FMath::Min(X / (Width / NumSteps), NumSteps - 1);
	return StepIndex * StepHeight;
}
