// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DemoPortfolio : ModuleRules
{
	public DemoPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

    PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput",
      "GameplayAbilities",
      "GameplayTags",
      "GameplayTasks",
    });

    PublicIncludePaths.AddRange(new string[] {
      "DemoPortfolio/AbilitySystem",
      "DemoPortfolio/AbilitySystem/Abilities",
      "DemoPortfolio/Player",
      "DemoPortfolio/Character",
    });

    PrivateIncludePaths.AddRange(new string[] {
      "DemoPortfolio/AbilitySystem",
      "DemoPortfolio/AbilitySystem/Abilities",
      "DemoPortfolio/Character",
      "DemoPortfolio/Player",
    });
  }
}
