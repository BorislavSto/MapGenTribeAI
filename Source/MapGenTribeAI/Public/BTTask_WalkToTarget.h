// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WalkToTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API UBTTask_WalkToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_WalkToTarget(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	float AttackRange = 150.0f;
	
};
