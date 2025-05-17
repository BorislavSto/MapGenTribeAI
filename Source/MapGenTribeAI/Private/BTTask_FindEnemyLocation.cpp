// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindEnemyLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindEnemyLocation::UBTTask_FindEnemyLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Find Enemy Location";
}

EBTNodeResult::Type UBTTask_FindEnemyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
		return EBTNodeResult::Failed;

	// Get the target actor (enemy) directly from the blackboard
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("AttackTarget"));
	if (TargetActor)
	{
		// Set the TargetLocation key to the enemy’s current location
		BlackboardComponent->SetValueAsVector("TargetLocation", TargetActor->GetActorLocation());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed; // Fail if no target actor is found
}
