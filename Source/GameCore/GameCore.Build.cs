﻿using UnrealBuildTool;

public class GameCore : ModuleRules
{
    public GameCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "MessageBus",
                "GameplayTags",
                "Niagara"
            }
        );
    }
}