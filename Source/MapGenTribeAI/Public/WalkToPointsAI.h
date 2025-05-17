// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WalkToPointsAI.generated.h"

UCLASS()
class UNREALELECTIVEV3_API AWalkToPointsAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWalkToPointsAI();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetPointLocation(int const index) const;

	int Num() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> WalkPoints;
};
