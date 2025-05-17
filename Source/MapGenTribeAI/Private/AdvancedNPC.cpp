// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedNPC.h"

#include "AdvancedNPC_AIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AAdvancedNPC::AAdvancedNPC()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
}

void AAdvancedNPC::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Advanced NPC Begin"));

	AIController = Cast<AAdvancedNPC_AIController>(GetController());
	if (AIController)
		BlackboardComp = AIController->GetBlackboardComponent();
	
	GetWorld()->GetTimerManager().SetTimer(NeedsTimerHandle, this, &AAdvancedNPC::TickNeeds, 5.0f, true);
	GetWorld()->GetTimerManager().SetTimer(NPCTick, this, &AAdvancedNPC::TickNPC, 1.0f, true);
}

void AAdvancedNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AAdvancedNPC::TakeDamage(int Damage)
{
	LifeData.Health -= Damage;
	UE_LOG(LogTemp, Log, TEXT("NPC Health: %f"), LifeData.Health);

	if (LifeData.Health <= MinHealthThreshold)
	{
		BlackboardComp->SetValueAsBool("NeedsHelp", true);
		GetWorld()->GetTimerManager().SetTimer(LocationTimeHandler, this, &AAdvancedNPC::TickLocation, 0.5f, true);
	}
	
	if (LifeData.Health <= 0)
	{
		OnNPCDeath.Broadcast(this);
		Destroy();
		return true;
	}
	return false;
}

void AAdvancedNPC::SetTribeLoc(FVector Location)
{
	if (AIController == nullptr)
		AIController = Cast<AAdvancedNPC_AIController>(GetController());

	if (AIController && BlackboardComp == nullptr)
		BlackboardComp = AIController->GetBlackboardComponent();
	
	TribeLocation = Location;
	if (AIController)
	{
		if (BlackboardComp)
			BlackboardComp->SetValueAsVector("TribeLocation", Location);
	}
}

void AAdvancedNPC::TickLocation()
{
	float DistanceToTribe = FVector::Dist(GetActorLocation(), TribeLocation);
	UE_LOG(LogTemp, Log, TEXT("NPC Distance to Tribe: %f"), DistanceToTribe);
	if (DistanceToTribe < 300.0f)
	{
		BlackboardComp->SetValueAsBool("IsAtTribe", true);
		GetWorldTimerManager().ClearTimer(LocationTimeHandler);
		
		if (BlackboardComp->GetValueAsBool("NeedsRest"))
		{
			LifeData.Energy = 100.0f;  // Fulfill the energy need
			BlackboardComp->SetValueAsBool("NeedsRest", false);
		}
		if (BlackboardComp->GetValueAsBool("NeedsFood"))
		{
			LifeData.Hunger = 100.0f;  // Fulfill the hunger need
			BlackboardComp->SetValueAsBool("NeedsFood", false);
		}
		if (BlackboardComp->GetValueAsBool("NeedsHelp"))
		{
			LifeData.Health = 50.0f;  // Fulfill the health need
			BlackboardComp->SetValueAsBool("NeedsHelp", false);
		}
	}
	else
		BlackboardComp->SetValueAsBool("IsAtTribe", false);
	
}

void AAdvancedNPC::TickNeeds()
{
	LifeData.Hunger -= HungerRate * 5.0f;
	LifeData.Energy -= EnergyRate * 5.0f;

	if (LifeData.Hunger <= 0 || LifeData.Health <= 0)
	{
		LifeData.Health -= HealthDecayRate * 5.0f;
	}

	UE_LOG(LogTemp, Log, TEXT("NPC Life Data - Energy: %f, Hunger: %f, Health: %f"), LifeData.Energy, LifeData.Hunger, LifeData.Health);

	if (AIController == nullptr)
		AIController = Cast<AAdvancedNPC_AIController>(GetController());

	if (AIController && BlackboardComp == nullptr)
		BlackboardComp = AIController->GetBlackboardComponent();

	if (AIController)
	{
		if (BlackboardComp)
		{
			if (LifeData.Energy <= MinEnergyThreshold)
			{
	
				BlackboardComp->SetValueAsBool("NeedsRest", true);
				UE_LOG(LogTemp, Log, TEXT("MinEnergyThreshold"));
				GetWorld()->GetTimerManager().SetTimer(LocationTimeHandler, this, &AAdvancedNPC::TickLocation, 0.5f, true);
				// Go to hut to rest
				//GoToHutForRest();
			}
			
			if (LifeData.Hunger <= MinHungerThreshold)
			{

				BlackboardComp->SetValueAsBool("NeedsFood", true);
				UE_LOG(LogTemp, Log, TEXT("MinFoodThreshold"));
				GetWorld()->GetTimerManager().SetTimer(LocationTimeHandler, this, &AAdvancedNPC::TickLocation, 0.5f, true);
				// Go to hut to eat
				//GoToHutForFood();
			}
		}
	}
}

void AAdvancedNPC::TickNPC()
{
	if (AIController == nullptr)
		AIController = Cast<AAdvancedNPC_AIController>(GetController());

	if (AIController && BlackboardComp == nullptr)
		BlackboardComp = AIController->GetBlackboardComponent();

	if (BlackboardComp)
	{
		ENPCState CurrentState = static_cast<ENPCState>(BlackboardComp->GetValueAsEnum("State"));
		switch (CurrentState)
		{
		case ENPCState::Idle:
			switch (NPCRole)
			{
			case ENPCRole::Defender:
				BlackboardComp->SetValueAsEnum("State",  static_cast<uint8>(ENPCState::Defend));
					break;
			case ENPCRole::Explorer:
				BlackboardComp->SetValueAsEnum("State",  static_cast<uint8>(ENPCState::Explore));
					break;
			}
			break;
		case ENPCState::Attack:
				UE_LOG(LogTemp, Log, TEXT("NPC is in Attack state"));
			break;
            
		default:
			break;
		}
	}
}

void AAdvancedNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* AAdvancedNPC::GetBehaviorTree() const
{
	return Tree;
}
