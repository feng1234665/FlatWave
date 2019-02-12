// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FlatWave : ModuleRules
{
    public FlatWave(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "FlatWave/Characters",
            "FlatWave/Characters/Player",
            "FlatWave/Characters/Enemy",
            "FlatWave/Weapons",
            "FlatWave/Util",
            "FlatWave/Spawner",
            "FlatWave/GameMode",
        });
    }
}
