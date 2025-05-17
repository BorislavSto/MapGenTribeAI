// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_WalkToTarget.h"

#include "AdvancedNPC.h"
#include "AdvancedNPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_WalkToTarget::UBTTask_WalkToTarget(const FObjectInitializer& ObjectInitializer)
{
	NodeName = "Walk To Target";
}

EBTNodeResult::Type UBTTask_WalkToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAdvancedNPC_AIController* const AIController = Cast<AAdvancedNPC_AIController>(OwnerComp.GetAIOwner());
    if (!AIController)
        return EBTNodeResult::Failed;

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
        return EBTNodeResult::Failed;

    FVector const TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector("TargetLocation");

    UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;

    //OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENPCState::Attack));
    
    // // Check if the NPC is already within attack range
    // if (FVector::Dist(ControlledPawn->GetActorLocation(), TargetLocation) <= AttackRange)
    // {
    //     AIController->StopMovement();
    //     
    //     ControlledPawn->FaceRotation((TargetLocation - ControlledPawn->GetActorLocation()).Rotation(), 0.2f);
    //
    //     // Update the state to 'Attack' in the blackboard
    //     OwnerComp.GetBlackboardComponent()->SetValueAsEnum("State", static_cast<uint8>(ENPCState::Attack));
    //
    //     // Finish the task, NPC is now attacking
    //     FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    //     return EBTNodeResult::Succeeded;
    // }
    //
    // // Otherwise, move to the target location
    // UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);
    //
    // // Face the target while moving (smooth rotation)
    // ControlledPawn->FaceRotation((TargetLocation - ControlledPawn->GetActorLocation()).Rotation(), 0.2f);
    //
    // // Keep updating until the NPC reaches the target or gets within attack range
    // return EBTNodeResult::InProgress;
}

