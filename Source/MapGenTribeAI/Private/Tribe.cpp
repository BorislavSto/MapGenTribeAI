// Fill out your copyright notice in the Description page of Project Settings.


#include "Tribe.h"
#include "AdvancedNPC.h"
#include "AdvancedNPC_AIController.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

// Sets default values
ATribe::ATribe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ATribe::BeginPlay()
{
	Super::BeginPlay();
}

void ATribe::InitializeFromPreset(const FTribePreset& Preset, ATribe* Tribe)
{
    MemberCount = Preset.MemberCount;
    HutMesh = Preset.HutMesh;
    NPCBlueprint = Preset.NPCBlueprint;
    SpawnLoc = Preset.SpawnLocation;

    TArray<FVector> OccupiedPositions;

    // Spawn huts
    int32 HutCount = FMath::Max(1, MemberCount / 4); // Amount of huts spawned per members
    for (int32 i = 0; i < HutCount; ++i)
    {
        FVector HutLocation;
        if (FindSuitablePosition(SpawnLoc, AreaRadius, MinSpaceBetweenHuts, OccupiedPositions, HutLocation))
        {
            // Spawn hut at HutLocation
            FActorSpawnParameters SpawnParams;
            AActor* Hut = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), HutLocation, FRotator::ZeroRotator, SpawnParams);
            if (Hut)
            {
                UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(Hut);
                MeshComponent->SetStaticMesh(HutMesh);
                MeshComponent->SetWorldLocation(HutLocation);
                MeshComponent->SetMobility(EComponentMobility::Static);  // Mark the hut as static so it's recognised by the NavMesh System
                MeshComponent->SetupAttachment(Hut->GetRootComponent());
                MeshComponent->RegisterComponent();
                OccupiedPositions.Add(HutLocation);
                TribeLoc = HutLocation;
                DrawDebugBox(GetWorld(), HutLocation, FVector(100, 100, 100), FColor::Blue, false, 2.0f);
            }
        }
    }

    // Spawn NPCs
    for (int32 i = 0; i < MemberCount; ++i)
    {
        FVector NPCLocation;
        if (FindSuitablePosition(SpawnLoc, AreaRadius, MinSpaceBetweenMembers, OccupiedPositions, NPCLocation))
        {
            FActorSpawnParameters SpawnParams;
            AAdvancedNPC* npc = GetWorld()->SpawnActor<AAdvancedNPC>(NPCBlueprint, NPCLocation, FRotator::ZeroRotator, SpawnParams);
            if (npc)
            {
                AAdvancedNPC_AIController* controller = GetWorld()->SpawnActor<AAdvancedNPC_AIController>(AAdvancedNPC_AIController::StaticClass(), NPCLocation, FRotator::ZeroRotator, SpawnParams);
                if (controller)
                {
                    controller->Possess(npc);
                    controller->SetOwnerNPC(npc);  // Set the owner reference
                }
                OccupiedPositions.Add(NPCLocation);
                TribeMembers.Add(npc);
                npc->Tribe = Tribe;
                npc->SetTribeLoc(TribeLoc);
                int32 RandomRoleIndex = FMath::RandRange(0, 1);
                npc->NPCRole = static_cast<ENPCRole>(RandomRoleIndex);
                DrawDebugSphere(GetWorld(), NPCLocation, 50.0f, 12, FColor::Green, false, 2.0f);
            }
        }
    }
}

bool ATribe::FindSuitablePosition(const FVector& Center, float Radius, float MinDistance, const TArray<FVector>& OccupiedPositions, FVector& OutPosition) const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    const float TraceHeight = 2000.0f; // Height for both upward and downward traces
    const int32 MaxAttempts = 50;
    const float SpawnOffset = 100.0f; // Offset above the ground for spawning

    for (int32 Attempt = 0; Attempt < MaxAttempts; ++Attempt)
    {
        FVector RandomPoint = Center + FMath::VRand() * Radius;
        RandomPoint.Z = Center.Z; // Start from the center's Z position

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);

        // Downward trace
        FVector TraceStart = RandomPoint + FVector(0, 0, TraceHeight / 2);
        FVector TraceEnd = RandomPoint - FVector(0, 0, TraceHeight / 2);
        bool bHitDown = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

        // Upward trace
        if (!bHitDown)
        {
            TraceStart = RandomPoint - FVector(0, 0, TraceHeight / 2);
            TraceEnd = RandomPoint + FVector(0, 0, TraceHeight / 2);
            bool bHitUp = World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

            if (!bHitUp)
                continue; // No hit in either direction, skip this attempt
        }

        FVector PotentialPosition = HitResult.Location + FVector(0, 0, SpawnOffset); // Add offset here
        
        // Check slope
        if (FVector::DotProduct(HitResult.Normal, FVector::UpVector) < 0.7f) // Adjusted for steeper terrain
            continue; // Too steep

        // Check distance from other occupied positions
        bool TooClose = false;
        for (const FVector& Pos : OccupiedPositions)
        {
            if (FVector::Dist(Pos, PotentialPosition) < MinDistance)
            {
                TooClose = true;
                break;
            }
        }

        if (!TooClose)
        {
            // Perform an additional trace to ensure we're not spawning inside anything
            FHitResult FinalCheckResult;
            FVector CheckStart = PotentialPosition + FVector(0, 0, 50.0f);
            FVector CheckEnd = PotentialPosition - FVector(0, 0, 50.0f);
            bool bHitAnything = World->LineTraceSingleByChannel(FinalCheckResult, CheckStart, CheckEnd, ECC_Visibility, QueryParams);

            if (!bHitAnything)
            {
                OutPosition = PotentialPosition;
                
                // Optional: Draw debug lines to visualize the trace
                DrawDebugLine(World, TraceStart, TraceEnd, FColor::Yellow, false, 2.0f);
                DrawDebugSphere(World, PotentialPosition, 50.0f, 12, FColor::Green, false, 2.0f);
                
                return true;
            }
        }
    }

    return false;
}

void ATribe::SetRelationWithTribe(ATribe* OtherTribe, ERelationType Relation)
{
	if (OtherTribe && OtherTribe != this)
		Relations.Add(OtherTribe, Relation);
}

ERelationType ATribe::GetRelationWithTribe(ATribe* OtherTribe) const
{
	if (OtherTribe && Relations.Contains(OtherTribe))
		return Relations[OtherTribe];

    return ERelationType::Neutral;
}
