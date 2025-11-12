// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdvMovSys : ModuleRules
{
	public AdvMovSys(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AdvMovSys",
			"AdvMovSys/Variant_Platforming",
			"AdvMovSys/Variant_Platforming/Animation",
			"AdvMovSys/Variant_Combat",
			"AdvMovSys/Variant_Combat/AI",
			"AdvMovSys/Variant_Combat/Animation",
			"AdvMovSys/Variant_Combat/Gameplay",
			"AdvMovSys/Variant_Combat/Interfaces",
			"AdvMovSys/Variant_Combat/UI",
			"AdvMovSys/Variant_SideScrolling",
			"AdvMovSys/Variant_SideScrolling/AI",
			"AdvMovSys/Variant_SideScrolling/Gameplay",
			"AdvMovSys/Variant_SideScrolling/Interfaces",
			"AdvMovSys/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
