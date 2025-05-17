// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "FastNoiseLite.h"
#include "LandscapeGenerator.generated.h"

UCLASS()
class ALandscapeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandscapeGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	int32 MapWidth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	int32 MapHeight = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	float NoiseScale = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	float MountainThreshold = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	float PlainThreshold = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landscape")
	float RiverThreshold = 0.1f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* MeshComponent;

	void GenerateLandscape() const;
	float GetNoiseValue(int32 X, int32 Y) const;
	FLinearColor GetBiomeColor(float NoiseValue) const;

};
