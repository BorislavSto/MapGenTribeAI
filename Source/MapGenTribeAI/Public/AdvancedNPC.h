// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tribe.h"
#include "WalkToPointsAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "AdvancedNPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCDeath, AAdvancedNPC*, DeadNPC);

class AAdvancedNPC_AIController;

UENUM(BlueprintType)
enum class ENPCRole : uint8
{
	Defender    UMETA(DisplayName = "Defender"),
	Explorer    UMETA(DisplayName = "Explorer"),
};

USTRUCT(BlueprintType)
struct FLifeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Energy = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float Health = 100.0f;
};

UENUM(BlueprintType)
enum class ENPCState : uint8 {
	Idle,
	Explore,
	Defend,
	Attack,
	Retreat,
};

UCLASS()
class UNREALELECTIVEV3_API AAdvancedNPC : public ACharacter
{
	GENERATED_BODY()

public:
	AAdvancedNPC();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	ATribe* Tribe; // could be a problem the way its stored

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	ENPCRole NPCRole = ENPCRole::Defender;

	// Delegate that other NPCs can bind to for NPC death
	FOnNPCDeath OnNPCDeath;

	// Function that gets called when the NPC takes damage and possibly dies
	bool TakeDamage(int Damage);
	
	void SetTribeLoc(FVector Location);
	
	void TickLocation();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float HungerRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float EnergyRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float HealthDecayRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float MinEnergyThreshold = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float MinHealthThreshold = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	float MinHungerThreshold = 20.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	UBehaviorTree* Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	AWalkToPointsAI* WalkToPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Needs")
	FLifeData LifeData;
	
	void TickNeeds();
	
	void TickNPC();

private:

	AAdvancedNPC_AIController* AIController = nullptr;
	UBlackboardComponent* BlackboardComp = nullptr;

	FTimerHandle NPCTick;
	
	FTimerHandle NeedsTimerHandle;

	FTimerHandle LocationTimeHandler;

	FVector TribeLocation;
};
