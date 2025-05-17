// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMeshGenerator.h"
#include "StairMeshGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API AStairMeshGenerator : public ABaseMeshGenerator
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	int32 NumSteps = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stairs")
	float StepHeight = 50.0f;

protected:
	virtual float GetVertexHeight(int32 X, int32 Y) override;
};
