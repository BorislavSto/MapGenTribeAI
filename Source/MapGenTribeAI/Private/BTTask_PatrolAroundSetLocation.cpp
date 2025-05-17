// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PatrolAroundSetLocation.h"

#include "AdvancedNPC.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_PatrolAroundSetLocation::UBTTask_PatrolAroundSetLocation(const FObjectInitializer& ObjectInitializer)
{
    NodeName = "Patrol Around Set Location";
}

EBTNodeResult::Type UBTTask_PatrolAroundSetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI Controller and Controlled Pawn
    AAIController* AIController = OwnerComp.GetAIOwner();
    ControlledNPC = AIController ? Cast<AAdvancedNPC>(AIController->GetPawn()) : nullptr;

    if (!ControlledNPC)
    {
        UE_LOG(LogTemp, Warning, TEXT("Controlled NPC is null in Random Patrol Task"));
        return EBTNodeResult::Failed;
    }

    // Get the Patrol Center from the Blackboard
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Warning, TEXT("Blackboard Component is null in Random Patrol Task"));
        return EBTNodeResult::Failed;
    }

    FVector PatrolCenter = BlackboardComp->GetValueAsVector("TribeLocation");
    
    // Generate a random location around the patrol center
    FVector RandomLocation = GenerateRandomLocation(PatrolCenter, PatrolRadius);

    // Check if the random location is valid (on the NavMesh)
    if (IsLocationValid(RandomLocation))
    {
        
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), RandomLocation);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
    // If the random location is not valid, fail the task and try again
    return EBTNodeResult::Failed;
}

FVector UBTTask_PatrolAroundSetLocation::GenerateRandomLocation(FVector PatrolCenter, float Radius)
{
    // Generate a random point around the PatrolCenter within the given radius
    FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(PatrolCenter, FVector(Radius, Radius, 0.0f));
    return RandomPoint;
}

bool UBTTask_PatrolAroundSetLocation::IsLocationValid(FVector Location) const
{
    // Check if the generated location is valid and on the navigation mesh
    FNavLocation NavLocation;
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(ControlledNPC);
    if (NavSystem && NavSystem->ProjectPointToNavigation(Location, NavLocation))
        return true;  // Location is valid

    return false;  // Location is invalid (not on the NavMesh)
}
