// Fill out your copyright notice in the Description page of Project Settings.

#include "BiomeTerrainGen.h"
#include "StaticMeshAttributes.h"
#include "MeshDescription.h"
#include "NavigationSystem.h"
#include "Tribe.h"
#include "NavMesh/NavMeshBoundsVolume.h"


// Sets default values
ABiomeTerrainGen::ABiomeTerrainGen()
{
    PrimaryActorTick.bCanEverTick = false;
    
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));

    RootComponent = ProceduralMesh;
    ProceduralMesh->SetupAttachment(RootComponent);
}

void ABiomeTerrainGen::BeginPlay()
{
    Super::BeginPlay();

    int32 StepSize = 1; // Start with 1 step in each direction
    int32 StepsTaken = 0; // Counter for steps in the current direction
    int32 CurrentDirection = 0; // 0: Forward, 1: Right, 2: Down, 3: Left
    int32 DirectionChangeCounter = 0; // Tracks when to increase StepSize

    if (Biomes.Num() == 0)
        return;

    GenerateInitialMesh(Biomes[0]);

    for (int32 i = 1; i < Biomes.Num(); i++)
    {
        FBiomeData& Biome = Biomes[i];

        GenerateNextMesh(CurrentDirection, Biome);

        StepsTaken++;

        if (StepsTaken >= StepSize)
        {
            StepsTaken = 0;
            CurrentDirection = (CurrentDirection + 1) % 4; // Cycle through directions (0-3)

            DirectionChangeCounter++;
            if (DirectionChangeCounter >= 2)
            {
                DirectionChangeCounter = 0;
                StepSize++;
            }
        }
    }
}

void ABiomeTerrainGen::GenerateNextMesh(int32 Direction, FBiomeData Biome)
{
    switch (Direction)
    {
    case 0: // Forward
        CurrentOffsetX += (Width - 1) * TileSize;
        GenerateConnectedMesh(CurrentOffsetX, CurrentOffsetY, Biome);
        break;
    case 1: // Right
        CurrentOffsetY += (Height - 1) * TileSize;
        GenerateConnectedMesh(CurrentOffsetX, CurrentOffsetY, Biome);
        break;
    case 2: // Down
        CurrentOffsetX -= (Width - 1) * TileSize;
        GenerateConnectedMesh(CurrentOffsetX, CurrentOffsetY, Biome);
        break;
    case 3: // Left
        CurrentOffsetY -= (Height - 1) * TileSize;
        GenerateConnectedMesh(CurrentOffsetX, CurrentOffsetY, Biome);
        break;
    default:
        break;;
    }
}

void ABiomeTerrainGen::GenerateInitialMesh(FBiomeData Biome)
{
    GenerateConnectedMesh(0.0f, 0.0f, Biome);
    CurrentOffsetX = 0.0f;
    CurrentOffsetY = 0.0f;
}

void ABiomeTerrainGen::GenerateConnectedMesh(float OffsetX, float OffsetY, FBiomeData Biome)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FColor> VertexColors;

    // Generate vertices
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            FVector NewVertex;

            // Create FVector as the unique position key
            // Z is initially set to 0.0f for they Position that will act as a key
            FVector Position = FVector(X * TileSize + OffsetX, Y * TileSize + OffsetY, 0.0f);

            if (AllVerticesUsed.Contains(Position))
            {
                // If occupied, get the existing vertex position
                NewVertex = AllVerticesUsed[Position];
            }
            else
            {
                // If not occupied, generate a new vertex
                float NoiseScale = Biome.NoiseScale;
                float MaxHeight = Biome.MaxHeight;
                float Exponent = Biome.Exponent;
                
                // If not occupied, generate a new vertex
                // Generate noise-based height
                float NoiseValue = FMath::PerlinNoise2D(FVector2D((X + OffsetX) * NoiseScale, (Y + OffsetY) * NoiseScale));
                float Z = FMath::Pow(FMath::Abs(NoiseValue), Exponent) * MaxHeight;
                NewVertex = FVector(Position.X, Position.Y, Z);

                // Store the new vertex in the map, using Position as the key
                AllVerticesUsed.Add(Position, NewVertex);
            }
            
            Vertices.Add(NewVertex);

            float UVScale = Biome.UVScale;
            
            FVector2D UVCoord = FVector2D(X * TileSize * UVScale, Y * TileSize * UVScale);
            UVs.Add(UVCoord);
            UE_LOG(LogTemp, Warning, TEXT("Noise Value at %f %f"), UVCoord.X, UVCoord.Y);
            VertexColors.Add(FColor(0, 255, 0, 255));
        }
    }

    // Generate triangles
    for (int32 Y = 0; Y < Height - 1; ++Y)
    {
        for (int32 X = 0; X < Width - 1; ++X)
        {
            int32 Index = X + Y * Width;
            Triangles.Add(Index);
            Triangles.Add(Index + Width);
            Triangles.Add(Index + 1);

            Triangles.Add(Index + 1);
            Triangles.Add(Index + Width);
            Triangles.Add(Index + Width + 1);
        }
    }

    ProceduralMesh->CreateMeshSection(
        CurrentSectionIndex,
        Vertices,
        Triangles,
        Normals,
        UVs,
        VertexColors,
        TArray<FProcMeshTangent>(),
        true
    );
    
    ProceduralMesh->SetMaterial(CurrentSectionIndex, Biome.BiomeMaterial);

    FVector TribeLoc = CalculateMeshCenter(Vertices);
    InitializeTribe(TribeLoc, Biome.TribePreset);
    
    if (Biome.BiomeGenBP)
    {
        // Convert procedural mesh section to static mesh
            UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), NAME_None, RF_Transient);
            FMeshDescription MeshDescription;
            FStaticMeshAttributes Attributes(MeshDescription);
            Attributes.Register();

            // Create vertices
            FVertexArray& MeshVertices = MeshDescription.Vertices();
            for (const FVector& Vertex : Vertices)
            {
                FVertexID VertexID = MeshVertices.Add();
                Attributes.GetVertexPositions()[VertexID] = FVector3f(Vertex.X, Vertex.Y, Vertex.Z);
            }

            // Create polygon group
            FPolygonGroupID PolygonGroupID = MeshDescription.CreatePolygonGroup();

            // Create triangles
            for (int32 i = 0; i < Triangles.Num(); i += 3)
            {
                TArray<FVertexInstanceID> TriVertexInstances;
            
                for (int32 j = 0; j < 3; j++)
                {
                    FVertexInstanceID InstanceID = MeshDescription.CreateVertexInstance(FVertexID(Triangles[i + j]));
                
                    // Set UVs with correct type conversion
                    if (UVs.IsValidIndex(Triangles[i + j]))
                    {
                        const FVector2D& UV = UVs[Triangles[i + j]];
                        Attributes.GetVertexInstanceUVs()[InstanceID] = FVector2f(UV.X, UV.Y);
                    }
                
                    TriVertexInstances.Add(InstanceID);
                }

                MeshDescription.CreatePolygon(PolygonGroupID, TriVertexInstances);
            }

            // Build static mesh
            StaticMesh->BuildFromMeshDescriptions({ &MeshDescription });
        
            // Spawn PCG generator and pass the mesh
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            APCGBiomeGen* BiomeGen = GetWorld()->SpawnActor<APCGBiomeGen>(
                Biome.BiomeGenBP, 
                GetActorLocation(), 
                GetActorRotation(), 
                SpawnParams
            );

            if (BiomeGen)
            {
                BiomeGen->OnMeshGenerated(StaticMesh);
            }
    }

    CurrentSectionIndex++;
}

FVector ABiomeTerrainGen::CalculateMeshCenter(const TArray<FVector>& Vertices)
{
    if (Vertices.Num() == 0) return FVector::ZeroVector;

    float SumX = 0.0f, SumY = 0.0f, SumZ = 0.0f;
    for (const FVector& Vertex : Vertices)
    {
        SumX += Vertex.X;
        SumY += Vertex.Y;
        SumZ += Vertex.Z;
    }

    int32 VertexCount = Vertices.Num();
    return FVector(SumX / VertexCount, SumY / VertexCount, SumZ / VertexCount);
}

void ABiomeTerrainGen::InitializeTribe(FVector Location, FTribePreset& TribePreset)
{
    FActorSpawnParameters SpawnParams;
    TribePreset.SpawnLocation = Location;
    ATribe* NewTribe = GetWorld()->SpawnActor<ATribe>(TribePreset.TribeClass, Location, FRotator::ZeroRotator, SpawnParams);
    
    if (NewTribe)
    {
        NewTribe->InitializeFromPreset(TribePreset, NewTribe);
        Tribes.Add(NewTribe);
    }
}
