// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeGenerator.h"

ALandscapeGenerator::ALandscapeGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LandscapeMesh"));
    RootComponent = MeshComponent;
}

void ALandscapeGenerator::BeginPlay()
{
    Super::BeginPlay();
    GenerateLandscape();
}

void ALandscapeGenerator::GenerateLandscape() const
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    for (int32 Y = 0; Y < MapHeight; ++Y)
    {
        for (int32 X = 0; X < MapWidth; ++X)
        {
            float NoiseValue = GetNoiseValue(X, Y);
            FVector Position(X * 100.0f, Y * 100.0f, NoiseValue * 1000.0f);
            Vertices.Add(Position);

            FLinearColor BiomeColor = GetBiomeColor(NoiseValue);
            VertexColors.Add(BiomeColor);

            if (X < MapWidth - 1 && Y < MapHeight - 1)
            {
                int32 Current = X + Y * MapWidth;
                int32 Next = Current + 1;
                int32 Bottom = Current + MapWidth;
                int32 BottomNext = Bottom + 1;

                Triangles.Add(Current);
                Triangles.Add(Bottom);
                Triangles.Add(Next);

                Triangles.Add(Next);
                Triangles.Add(Bottom);
                Triangles.Add(BottomNext);
            }
        }
    }

    MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}

float ALandscapeGenerator::GetNoiseValue(int32 X, int32 Y) const
{
    static FastNoiseLite Noise;
    static bool NoiseInitialized = false;

    if (!NoiseInitialized)
    {
        Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        Noise.SetFrequency(NoiseScale);
        Noise.SetFractalType(FastNoiseLite::FractalType_FBm);
        Noise.SetFractalOctaves(4);
        Noise.SetFractalLacunarity(2.0f);
        Noise.SetFractalGain(0.5f);
        NoiseInitialized = true;
    }

    return (Noise.GetNoise(static_cast<float>(X), static_cast<float>(Y)) + 1.0f) / 2.0f;
}

FLinearColor ALandscapeGenerator::GetBiomeColor(float NoiseValue) const
{
    if (NoiseValue > MountainThreshold)
        return FLinearColor(0.5f, 0.5f, 0.5f); // Gray for mountains
    else if (NoiseValue > PlainThreshold)
        return FLinearColor(0.0f, 0.8f, 0.0f); // Green for plains
    else if (NoiseValue > RiverThreshold)
        return FLinearColor(0.0f, 0.0f, 1.0f); // Blue for rivers
    else
        return FLinearColor(0.0f, 0.5f, 0.0f); // Dark green for swamps
}


void ALandscapeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
