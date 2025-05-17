// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCGBiomeGen.h"
#include "ProceduralMeshComponent.h"
#include "TribeData.h"
#include "BiomeTerrainGen.generated.h"


UENUM(BlueprintType)
enum class EBiomeType : uint8
{
	Sand     UMETA(DisplayName = "Sand"),
	Mountain UMETA(DisplayName = "Mountain"),
	Plains   UMETA(DisplayName = "Plains"),
	Volcano  UMETA(DisplayName = "Volcano")
};

USTRUCT(BlueprintType)
struct FBiomeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBiomeType BiomeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BiomeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Exponent  = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHeight  = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NoiseScale = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UVScale = 0.003f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<APCGBiomeGen> BiomeGenBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tribes")
	FTribePreset TribePreset;
};

UCLASS()
class UNREALELECTIVEV3_API ABiomeTerrainGen : public AActor
{
	GENERATED_BODY()
	
public:
	ABiomeTerrainGen();
	
	UPROPERTY()
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere)
	TArray<FBiomeData> Biomes;
	
	UPROPERTY(EditAnywhere)
	int32 Width = 10;

	UPROPERTY(EditAnywhere)
	int32 Height = 10;

	UPROPERTY(EditAnywhere)
	float TileSize = 100.0f;

protected:
	virtual void BeginPlay() override;
	
    void GenerateConnectedMesh(float OffsetX, float OffsetY, FBiomeData Biome);

	void GenerateNextMesh(int32 Direction, FBiomeData Biome);
    
	void GenerateInitialMesh(FBiomeData Biome);
	
	TMap<FVector, FVector> AllVerticesUsed;

	// Track current position
	float CurrentOffsetX = 0.0f;
	float CurrentOffsetY = 0.0f;

	// Track index of current created mesh
	int32 CurrentSectionIndex = 0;

	static FVector CalculateMeshCenter(const TArray<FVector>& Vertices);
	
	void InitializeTribe(FVector Location, FTribePreset& TribePreset);

	UPROPERTY()
	TArray<ATribe*> Tribes;

};
