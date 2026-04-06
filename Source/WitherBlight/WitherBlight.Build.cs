// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class WitherBlight : ModuleRules
{
	public WitherBlight(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
            "SlateCore",
            "GeometryCollectionEngine",
            "Chaos",
            "PhysicsCore",
            "ChaosSolverEngine",
            "FieldSystemEngine",
            "Niagara",
            "GameplayAbilities",
			"GameplayTags",
			"GameplayTasks",
            "NavigationSystem"
			//"CAD_MenuSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
			"WitherBlight",
            //"WitherBlight/Private",
            "WitherBlight/Variant_Platforming",
			"WitherBlight/Variant_Platforming/Animation",
			"WitherBlight/Variant_Combat/Public",
            "WitherBlight/Variant_Combat/Public/AI",
            "WitherBlight/Variant_Combat/Public/Animation",
            "WitherBlight/Variant_Combat/Public/Gameplay",
            "WitherBlight/Variant_Combat/Public/Interfaces",
            "WitherBlight/Variant_Combat/Public/UI",
            "WitherBlight/Variant_SideScrolling",
			"WitherBlight/Variant_SideScrolling/AI",
			"WitherBlight/Variant_SideScrolling/Gameplay",
			"WitherBlight/Variant_SideScrolling/Interfaces",
            "WitherBlight/Variant_SideScrolling/UI"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
