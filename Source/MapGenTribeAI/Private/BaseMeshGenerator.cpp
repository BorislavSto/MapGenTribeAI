// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeshGenerator.h"


ABaseMeshGenerator::ABaseMeshGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComponent;
}

void ABaseMeshGenerator::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseMeshGenerator::GenerateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FLinearColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	// Generate vertices
	for (int32 Y = 0; Y < Height; ++Y)
	{
		for (int32 X = 0; X < Width; ++X)
		{
			float Z = GetVertexHeight(X, Y);
			Vertices.Add(FVector(X * TileSize, Y * TileSize, Z));
			Normals.Add(FVector(0, 0, 1));
			UV0.Add(FVector2D(X / (float)(Width - 1), Y / (float)(Height - 1)));
			VertexColors.Add(FLinearColor::White);
		}
	}

	// Generate triangles
	for (int32 Y = 0; Y < Height - 1; ++Y)
	{
		for (int32 X = 0; X < Width - 1; ++X)
		{
			int32 Current = X + Y * Width;
			int32 Next = Current + 1;
			int32 Bottom = Current + Width;
			int32 BottomNext = Bottom + 1;

			Triangles.Add(Current);
			Triangles.Add(Bottom);
			Triangles.Add(Next);

			Triangles.Add(Next);
			Triangles.Add(Bottom);
			Triangles.Add(BottomNext);
		}
	}

	MeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	MeshComponent->SetMobility(EComponentMobility::Static);  // Mark the hut as static
}
