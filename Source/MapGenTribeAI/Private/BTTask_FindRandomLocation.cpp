// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"

#include "AdvancedNPC_AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Random Location In Navmesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAdvancedNPC_AIController* const cont = Cast<AAdvancedNPC_AIController>(OwnerComp.GetAIOwner()))
	{
		if (APawn* const npc = cont->GetPawn())
		{
			FVector const NpcPos = npc->GetActorLocation();

			if (UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NavLoc;
				if (NavSys->GetRandomPointInNavigableRadius(NpcPos, SearchRadius, NavLoc))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLoc.Location);

					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					return EBTNodeResult::Succeeded;
				}
			}
		}
		return EBTNodeResult::Failed;
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
