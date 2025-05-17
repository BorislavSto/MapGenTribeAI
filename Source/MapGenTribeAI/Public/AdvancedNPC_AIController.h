// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AdvancedNPC_AIController.generated.h"

class AAdvancedNPC;
/**
 * 
 */
UCLASS()
class UNREALELECTIVEV3_API AAdvancedNPC_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit AAdvancedNPC_AIController(FObjectInitializer const& ObjectInitializer);

	void SetOwnerNPC(AAdvancedNPC* NPC) { OwnerNPC = NPC; }

	UFUNCTION()
	void OnEnemyDeath(AAdvancedNPC* DeadNPC);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
private:
	class UAISenseConfig_Sight* SightConfig;

	AAdvancedNPC* OwnerNPC;
	
	void SetupPerceptionSystem();

	int AlliesCount;

	int EnemiesCount;

	FTimerHandle SurroundingsCheckTimerHandle;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	TArray<AAdvancedNPC*> DetectedEnemies;

};
