using UnrealBuildTool;

public class BoidEditorVisualisations : ModuleRules
{
    public BoidEditorVisualisations(ReadOnlyTargetRules Target) : base(Target)
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
                "UnrealEd",
                "ComponentVisualizers",
                "Boids"
            }
        );
    }
}