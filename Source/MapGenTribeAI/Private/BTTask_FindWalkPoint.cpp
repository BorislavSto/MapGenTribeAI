// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindWalkPoint.h"

#include "AdvancedNPC.h"
#include "AdvancedNPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindWalkPoint::UBTTask_FindWalkPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Walk Point";
}

EBTNodeResult::Type UBTTask_FindWalkPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AAdvancedNPC_AIController* const cont = Cast<AAdvancedNPC_AIController>(OwnerComp.GetAIOwner()))
	{
		// try to get the blackboard component from the behavior tree
		if (UBlackboardComponent* const bc = OwnerComp.GetBlackboardComponent())
		{
			int32 const index = bc->GetValueAsInt(GetSelectedBlackboardKey());

			if (AAdvancedNPC* npc = Cast<AAdvancedNPC>(cont->GetPawn()))
			{
				
			}
		}
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
