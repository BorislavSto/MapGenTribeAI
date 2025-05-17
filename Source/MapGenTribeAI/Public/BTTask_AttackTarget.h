// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedNPC.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_AttackTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API UBTTask_AttackTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AttackTarget();

	// Attack range for the AI character
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 200.0f;

	// Damage dealt to the target
	UPROPERTY(EditAnywhere, Category = "Attack")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float CooldownTime = 2.0f;  // Set the cooldown between attacks (in seconds)

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackMontage;

	AAIController* AIController;
	
protected:
	// Called when the task starts
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Check if the target is within the attack range
	bool IsTargetInRange(AActor* TargetActor, ACharacter* ControlledCharacter,UBehaviorTreeComponent& OwnerComp) const;

private:

	FTimerHandle AttackTimerHandle;
	float AttackInterval = 1.0f;
	bool bIsAttacking;
	
};
