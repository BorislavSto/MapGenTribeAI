// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribeData.h"
#include "WaveMeshGenerator.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class UNREALELECTIVEV3_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	UFUNCTION(BlueprintCallable, Category = "Game Management")
	void InitializeGame();

	UFUNCTION(BlueprintCallable, Category = "Game Setup")
	void InitializeTribes();

	UFUNCTION(BlueprintCallable, Category = "Game Setup")
	void GenerateLandscape();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tribes")
	TArray<FTribePreset> TribePresets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Terrain")
	AWaveMeshGenerator* WaveMeshGenerator;

	UPROPERTY(EditAnywhere)
	bool bPlayThisGM;
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<ATribe*> Tribes;

	void SetupTribalRelations();

};
