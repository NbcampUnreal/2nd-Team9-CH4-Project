using UnrealBuildTool;

public class Gameplay : ModuleRules
{
    public Gameplay(ReadOnlyTargetRules Target) : base(Target)
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
                "GameCore",
                "MessageBus",
                "OnlineSubsystem",
                "OnlineSubsystemUtils",
                "OnlineSubsystemEOS"
                "GameplayTags", "Niagara",
            }
        );
        
        PrivateDefinitions.Add("P2PMODE=1");
    }
}