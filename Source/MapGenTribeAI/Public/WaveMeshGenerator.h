// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMeshGenerator.h"
#include "WaveMeshGenerator.generated.h"

/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API AWaveMeshGenerator : public ABaseMeshGenerator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float Amplitude = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float Frequency = 0.1f;

	void GenerateTerrain();
	
protected:
	virtual float GetVertexHeight(int32 X, int32 Y) override;
	
};
