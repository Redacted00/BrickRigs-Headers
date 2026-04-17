// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleUGCEditor : ModuleRules
{
	public SimpleUGCEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"PluginBrowser"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Projects",
			"InputCore",
			"UnrealEd",
			"LevelEditor",
			"CoreUObject",
			"Engine",
			"PluginBrowser",
			"Slate",
			"SlateCore",
		});
	}
}