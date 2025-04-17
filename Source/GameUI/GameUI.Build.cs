using UnrealBuildTool;

public class GameUI : ModuleRules
{
    public GameUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                
                "Core",
                "InputCore",
                "UMG",
                "GameplayTags"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameCore",
                "Gameplay",
                "Sockets",
                "Networking",
                "OnlineSubsystem"
            }
        );
    }
}