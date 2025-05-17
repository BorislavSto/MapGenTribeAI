// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class UnrealElectiveV3 : ModuleRules
{
    public UnrealElectiveV3(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "NavigationSystem",
            "AIModule",
            "Niagara",
            "EnhancedInput",
            "Landscape",
            "LandscapeEditor",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "ProceduralMeshComponent",
            "MeshDescription",
            "StaticMeshDescription",
        });
        
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/FastNoise/Cpp"));
    }
}
