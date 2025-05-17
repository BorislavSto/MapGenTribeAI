// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldGen.h"

UWorldGen::UWorldGen() 
{
    MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
}

void UWorldGen::GenerateWorldPerlinNoise(int32 Width, int32 Height, float Scale)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            float PerlinValue = FMath::PerlinNoise2D(FVector2D(X * Scale, Y * Scale)) * 0.5f + 0.5f;            
            Vertices.Add(FVector(X * 100, Y * 100, PerlinValue * 1000));

            if (X < Width - 1 && Y < Height - 1)
            {
                int32 V = X + Y * Width;
                Triangles.Add(V);
                Triangles.Add(V + Width);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + Width);
                Triangles.Add(V + Width + 1);
            }
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("No other WorldGenerator instances detected."));
    CreateMesh(Vertices, Triangles);
}

void UWorldGen::GenerateWorldDiamondSquare(int32 Size, float Roughness)
{
    int32 GridSize = FMath::Pow(2.0f, Size) + 1;
    TArray<TArray<float>> HeightMap;
    HeightMap.SetNum(GridSize);
    for (auto& Row : HeightMap)
    {
        Row.SetNum(GridSize);
    }

    // Initialize corners
    HeightMap[0][0] = HeightMap[0][GridSize - 1] = HeightMap[GridSize - 1][0] = HeightMap[GridSize - 1][GridSize - 1] = 0;

    // Diamond-Square algorithm
    for (int32 Step = GridSize - 1; Step > 1; Step /= 2)
    {
        int32 HalfStep = Step / 2;

        // Diamond step
        for (int32 Y = HalfStep; Y < GridSize; Y += Step)
        {
            for (int32 X = HalfStep; X < GridSize; X += Step)
            {
                float Average = (HeightMap[Y - HalfStep][X - HalfStep] + HeightMap[Y - HalfStep][X + HalfStep] +
                    HeightMap[Y + HalfStep][X - HalfStep] + HeightMap[Y + HalfStep][X + HalfStep]) / 4.0f;
                HeightMap[Y][X] = Average + FMath::RandRange(-Roughness, Roughness);
            }
        }

        // Square step
        for (int32 Y = 0; Y < GridSize; Y += HalfStep)
        {
            for (int32 X = (Y + HalfStep) % Step; X < GridSize; X += Step)
            {
                float Average = 0;
                int32 Count = 0;

                if (X >= HalfStep) { Average += HeightMap[Y][X - HalfStep]; Count++; }
                if (X + HalfStep < GridSize) { Average += HeightMap[Y][X + HalfStep]; Count++; }
                if (Y >= HalfStep) { Average += HeightMap[Y - HalfStep][X]; Count++; }
                if (Y + HalfStep < GridSize) { Average += HeightMap[Y + HalfStep][X]; Count++; }

                Average /= Count;
                HeightMap[Y][X] = Average + FMath::RandRange(-Roughness, Roughness);
            }
        }

        Roughness /= 2.0f;
    }

    // Create mesh from heightmap
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < GridSize; ++Y)
    {
        for (int32 X = 0; X < GridSize; ++X)
        {
            Vertices.Add(FVector(X * 100, Y * 100, HeightMap[Y][X] * 1000));

            if (X < GridSize - 1 && Y < GridSize - 1)
            {
                int32 V = X + Y * GridSize;
                Triangles.Add(V);
                Triangles.Add(V + GridSize);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + GridSize);
                Triangles.Add(V + GridSize + 1);
            }
        }
    }

    CreateMesh(Vertices, Triangles);
}

void UWorldGen::GenerateWorldCellularAutomata(int32 Width, int32 Height, float FillProbability)
{
    TArray<TArray<bool>> Grid;
    Grid.SetNum(Height);
    for (auto& Row : Grid)
    {
        Row.SetNum(Width);
        for (auto& Cell : Row)
        {
            Cell = FMath::RandRange(0.0f, 1.0f) < FillProbability;
        }
    }

    // Apply cellular automata rules
    for (int32 Iteration = 0; Iteration < 5; ++Iteration)
    {
        TArray<TArray<bool>> NewGrid = Grid;

        for (int32 Y = 0; Y < Height; ++Y)
        {
            for (int32 X = 0; X < Width; ++X)
            {
                int32 AliveNeighbors = 0;
                for (int32 NY = -1; NY <= 1; ++NY)
                {
                    for (int32 NX = -1; NX <= 1; ++NX)
                    {
                        if (NX == 0 && NY == 0) continue;
                        int32 CheckX = X + NX;
                        int32 CheckY = Y + NY;
                        if (CheckX >= 0 && CheckX < Width && CheckY >= 0 && CheckY < Height)
                        {
                            AliveNeighbors += Grid[CheckY][CheckX] ? 1 : 0;
                        }
                    }
                }

                if (Grid[Y][X])
                {
                    NewGrid[Y][X] = AliveNeighbors >= 4;
                }
                else
                {
                    NewGrid[Y][X] = AliveNeighbors >= 5;
                }
            }
        }

        Grid = NewGrid;
    }

    // Create mesh from grid
    TArray<FVector> Vertices;
    TArray<int32> Triangles;

    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            Vertices.Add(FVector(X * 100, Y * 100, Grid[Y][X] ? 100 : 0));

            if (X < Width - 1 && Y < Height - 1)
            {
                int32 V = X + Y * Width;
                Triangles.Add(V);
                Triangles.Add(V + Width);
                Triangles.Add(V + 1);
                Triangles.Add(V + 1);
                Triangles.Add(V + Width);
                Triangles.Add(V + Width + 1);
            }
        }
    }

    CreateMesh(Vertices, Triangles);
}

void UWorldGen::CreateMesh(const TArray<FVector>& Vertices, const TArray<int32>& Triangles)
{
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors;

    //MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
}