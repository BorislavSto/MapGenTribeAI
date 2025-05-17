// Fill out your copyright notice in the Description page of Project Settings.


#include "DetectableObject.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

// Sets default values
ADetectableObject::ADetectableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	UAIPerceptionStimuliSourceComponent* StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliSource->RegisterWithPerceptionSystem();
}

// Called when the game starts or when spawned
void ADetectableObject::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void ADetectableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

