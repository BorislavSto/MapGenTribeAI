// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolAroundSetLocation.generated.h"

class AAdvancedNPC;
/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API UBTTask_PatrolAroundSetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_PatrolAroundSetLocation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	FVector GenerateRandomLocation(FVector PatrolCenter, float Radius);
	bool IsLocationValid(FVector Location) const;

	AAdvancedNPC* ControlledNPC;
	
	float PatrolRadius = 500.0f;  // Default radius for patrol area
	float StuckThreshold = 50.0f;  // Threshold to detect if the NPC is stuck

	FTimerHandle CheckStuckTimerHandle;

};
