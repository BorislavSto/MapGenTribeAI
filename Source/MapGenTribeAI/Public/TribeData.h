// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TribeData.generated.h"

class AAdvancedNPC;

UENUM(BlueprintType)
enum class ERelationType : uint8
{
	Friend UMETA(DisplayName = "Friend"),
	Enemy UMETA(DisplayName = "Enemy"),
	Trade UMETA(DisplayName = "Trade"),
	Neutral UMETA(DisplayName = "Neutral")
};

USTRUCT(BlueprintType)
struct FTribePreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATribe> TribeClass;

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemberCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* HutMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAdvancedNPC> NPCBlueprint;

};

UCLASS(Blueprintable)
class UNREALELECTIVEV3_API UTribeData : public UObject 
{
	GENERATED_BODY()
	
public:
	UTribeData();
	~UTribeData();
};
