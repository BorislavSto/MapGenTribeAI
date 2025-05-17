// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedNPC_AIController.h"
#include "AdvancedNPC.h"
#include "DetectableObject.h"
#include "Tribe.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Sight.h"

AAdvancedNPC_AIController::AAdvancedNPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AAdvancedNPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AAdvancedNPC* const npc = Cast<AAdvancedNPC>(InPawn)) 
	{
		if(UBehaviorTree* const tree = npc->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
			Blackboard->SetValueAsEnum("State", static_cast<uint8>(ENPCState::Idle));
			UE_LOG(LogTemp, Warning, TEXT("BehaviorTree or BlackboardComponent is not missing!"));
		}
	}
}

void AAdvancedNPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	UE_LOG(LogTemp, Log, TEXT("AAdvancedNPC_AIController before SightConfig"));
	if (SightConfig)
	{
		UE_LOG(LogTemp, Log, TEXT("AAdvancedNPC_AIController after SightConfig"));

		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(
			TEXT("PerceptionComponent")));
		SightConfig->SightRadius = 500.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 90.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAdvancedNPC_AIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AAdvancedNPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (Actor->IsA(ADetectableObject::StaticClass())) // Check if it's a bush
	{
		UE_LOG(LogTemp, Log, TEXT("Berry bush detected!"));
		// Add logic for AI to go to the bush and gather berries
		return;
	}
	
    if (AAdvancedNPC* const DetectedNPC = Cast<AAdvancedNPC>(Actor))
    {
        if (OwnerNPC && OwnerNPC->Tribe && DetectedNPC->Tribe)
        {
            ERelationType relation = OwnerNPC->Tribe->GetRelationWithTribe(DetectedNPC->Tribe);

            if (Stimulus.WasSuccessfullySensed()) // Actor is currently in sight
            {
            	if (relation == ERelationType::Enemy && !DetectedEnemies.Contains(DetectedNPC))
            	{
            		DetectedEnemies.Add(DetectedNPC);
            		DetectedNPC->OnNPCDeath.AddDynamic(this, &AAdvancedNPC_AIController::OnEnemyDeath);

            		UE_LOG(LogTemp, Warning, TEXT("Enemy detected and added to list. EnemiesCount: %d"), DetectedEnemies.Num());

            		// Set the attack target if no current target
            		if (DetectedEnemies.Num() == 1)
            		{
            			Blackboard->SetValueAsObject("AttackTarget", DetectedEnemies[0]);
            		}
            	}
            }
            else // Actor lost or out of sight
            {
                if (relation == ERelationType::Enemy && DetectedEnemies.Contains(DetectedNPC))
                {
                	DetectedNPC->OnNPCDeath.RemoveDynamic(this, &AAdvancedNPC_AIController::OnEnemyDeath);
                	
                    DetectedEnemies.Remove(DetectedNPC);
                    UE_LOG(LogTemp, Warning, TEXT("Enemy lost and removed from list. EnemiesCount: %d"), DetectedEnemies.Num());

                    // Update attack target to the next enemy in line if available
                    if (DetectedEnemies.Num() > 0)
                    {
                        Blackboard->SetValueAsObject("AttackTarget", DetectedEnemies[0]);
                    }
                    else
                    {
                        // Clear attack target if no more enemies are in sight
                        Blackboard->SetValueAsObject("AttackTarget", nullptr);
                    }
                }
            }

            // Decision logic based on ally and enemy counts
            if (DetectedEnemies.Num() > AlliesCount + 1)
            {
                Blackboard->SetValueAsEnum("State", static_cast<uint8>(ENPCState::Retreat));
                Blackboard->SetValueAsVector("TargetLocation", OwnerNPC->Tribe->TribeLoc + FVector(0,100,0));
            	
                for (AAdvancedNPC* EnemyNPC : DetectedEnemies)
                	EnemyNPC->OnNPCDeath.RemoveDynamic(this, &AAdvancedNPC_AIController::OnEnemyDeath);
            	DetectedEnemies.Empty();

                UE_LOG(LogTemp, Warning, TEXT("Retreating to Tribe Location."));
            }
            else if (AlliesCount + 1 >= DetectedEnemies.Num() && DetectedEnemies.Num() > 0)
            {
                Blackboard->SetValueAsEnum("State", static_cast<uint8>(ENPCState::Attack));
                Blackboard->SetValueAsVector("TargetLocation", DetectedEnemies[0]->GetActorLocation());
                UE_LOG(LogTemp, Warning, TEXT("Attacking enemy."));
            }
            else if (DetectedEnemies.Num() == 0)
            {
                Blackboard->SetValueAsEnum("State", static_cast<uint8>(OwnerNPC->NPCRole == ENPCRole::Defender ? ENPCState::Defend : ENPCState::Explore));
                Blackboard->SetValueAsObject("AttackTarget", nullptr);
                UE_LOG(LogTemp, Warning, TEXT("Switching to default state."));
            }
        }
    }
}

void AAdvancedNPC_AIController::OnEnemyDeath(AAdvancedNPC* DeadNPC)
{
	if (DetectedEnemies.Contains(DeadNPC))
	{
		DetectedEnemies.Remove(DeadNPC);
		DeadNPC->OnNPCDeath.RemoveDynamic(this, &AAdvancedNPC_AIController::OnEnemyDeath);
		UE_LOG(LogTemp, Warning, TEXT("Enemy died and removed from list. EnemiesCount: %d"), DetectedEnemies.Num());

		// Update the attack target to the next enemy if any are left
		if (DetectedEnemies.Num() > 0)
		{
			Blackboard->SetValueAsObject("AttackTarget", DetectedEnemies[0]);
		}
		else
		{
			Blackboard->SetValueAsObject("AttackTarget", nullptr);
			
			UBlackboardComponent* BlackboardComp = this	->GetBlackboardComponent();
			if (!BlackboardComp)
				return;

			AAdvancedNPC* ControlledNPC = Cast<AAdvancedNPC>(this->GetPawn());
			if (!ControlledNPC)
				return;
			
			if (BlackboardComp->GetValueAsEnum("State") ==  static_cast<uint8>(ENPCState::Attack))
			{
				switch (ControlledNPC->NPCRole)
				{
				case ENPCRole::Defender:
					BlackboardComp->SetValueAsEnum("State",  static_cast<uint8>(ENPCState::Defend));
					break;
					case ENPCRole::Explorer:
					BlackboardComp->SetValueAsEnum("State",  static_cast<uint8>(ENPCState::Explore));
					break;
				}
			}
		}
	}
}
