// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "BaseMeshGenerator.generated.h"

UCLASS()
class UNREALELECTIVEV3_API ABaseMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseMeshGenerator();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	int32 Width = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	int32 Height = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float TileSize = 100.0f;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* MeshComponent;

	virtual void GenerateMesh();
	virtual float GetVertexHeight(int32 X, int32 Y) { return 0.0f; }
};
