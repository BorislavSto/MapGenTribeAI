// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TribeData.h"
#include "Tribe.generated.h"

UCLASS()
class UNREALELECTIVEV3_API ATribe : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATribe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Tribe")
	void InitializeFromPreset(const FTribePreset& Preset, ATribe* Tribe);
	
	UFUNCTION(BlueprintCallable, Category = "Tribe Relations")
	void SetRelationWithTribe(ATribe* OtherTribe, ERelationType Relation);
	
	UFUNCTION(BlueprintPure, Category = "Tribe Relations")
	ERelationType GetRelationWithTribe(ATribe* OtherTribe) const;

    UPROPERTY(EditAnywhere, Category = "Tribe")
	float AreaRadius = 500.0f; // Adjust based on wanted tribe spread

    UPROPERTY(EditAnywhere, Category = "Tribe")
	float MinSpaceBetweenHuts = 400.0f;
	
    UPROPERTY(EditAnywhere, Category = "Tribe")
	float MinSpaceBetweenMembers = 300.0f;

private:
    UPROPERTY(VisibleAnywhere, Category = "Tribe")
    int32 MemberCount;

    UPROPERTY(VisibleAnywhere, Category = "Tribe")
    UStaticMesh* HutMesh;

    UPROPERTY(VisibleAnywhere, Category = "Tribe")
    TSubclassOf<AAdvancedNPC> NPCBlueprint;
	
    UPROPERTY(VisibleAnywhere, Category = "Tribe")
    FVector SpawnLoc;

public:
    TArray<AAdvancedNPC*> TribeMembers;

	FVector TribeLoc;
	
	bool FindSuitablePosition(const FVector& Center, float Radius, float MinDistance, const TArray<FVector>& OccupiedPositions, FVector& OutPosition) const;
//* MemberMesh;

    UPROPERTY()
    TMap<ATribe*, ERelationType> Relations;

};
