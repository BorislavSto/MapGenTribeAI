// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "WorldGen.generated.h"

/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API UWorldGen : public UObject
{
	GENERATED_BODY()
	
public:
	UWorldGen();

    UFUNCTION(BlueprintCallable, Category = "World Generation")
    void GenerateWorldPerlinNoise(int32 Width, int32 Height, float Scale);

    UFUNCTION(BlueprintCallable, Category = "World Generation")
    void GenerateWorldDiamondSquare(int32 Size, float Roughness);

    UFUNCTION(BlueprintCallable, Category = "World Generation")
    void GenerateWorldCellularAutomata(int32 Width, int32 Height, float FillProbability);

private:
    UPROPERTY()
    UProceduralMeshComponent* MeshComponent;

    void CreateMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles);
};
