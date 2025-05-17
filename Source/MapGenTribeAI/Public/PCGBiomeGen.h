// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCGBiomeGen.generated.h"

UCLASS()
class UNREALELECTIVEV3_API APCGBiomeGen : public AActor
{
	GENERATED_BODY()
	
public:
	explicit APCGBiomeGen(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Mesh Generation")
	void OnMeshGenerated(UStaticMesh* Mesh);
};
