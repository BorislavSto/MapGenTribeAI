// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackTarget.h"

#include "AdvancedNPC.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AttackTarget::UBTTask_AttackTarget()
{
	NodeName = "Attack Target";
}

EBTNodeResult::Type UBTTask_AttackTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI Controller and Controlled Pawn
    AIController = OwnerComp.GetAIOwner();
    AAdvancedNPC* ControlledNPC = AIController ? Cast<AAdvancedNPC>(AIController->GetPawn()) : nullptr;
    if (!ControlledNPC)
        return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
        return EBTNodeResult::Failed;

    AAdvancedNPC* TargetNPC = Cast<AAdvancedNPC>(BlackboardComp->GetValueAsObject("AttackTarget"));
    if (!TargetNPC)
        return EBTNodeResult::Failed;

    if (IsTargetInRange(TargetNPC, ControlledNPC, OwnerComp))
    {
        UAnimInstance* AnimInstance = ControlledNPC->GetMesh()->GetAnimInstance();
        if (AnimInstance && AttackMontage)
            AnimInstance->Montage_Play(AttackMontage);

        TargetNPC->TakeDamage(Damage);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}

bool UBTTask_AttackTarget::IsTargetInRange(AActor* TargetActor, ACharacter* ControlledCharacter, UBehaviorTreeComponent& OwnerComp) const
{
    if (!TargetActor || !ControlledCharacter)
        return false;
    
    DrawDebugSphere(ControlledCharacter->GetWorld(), ControlledCharacter->GetActorLocation(), 10.0f, 12, FColor::Red, false, 0.1f);

    float Distance = FVector::Dist(ControlledCharacter->GetActorLocation(), TargetActor->GetActorLocation());
    bool bInRange = Distance <= AttackRange;

    if (bInRange)
    {
        UE_LOG(LogTemp, Log, TEXT("IsTargetInRange: Target is within range. Proceeding with attack."));
    }
    else
    {
         UE_LOG(LogTemp, Warning, TEXT("IsTargetInRange: Target is out of range. Cannot attack."));
        
    }

    return bInRange;
}
