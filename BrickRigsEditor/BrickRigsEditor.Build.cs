// Copyright Fluppisoft, 2023

using UnrealBuildTool;

public class BrickRigsEditor : ModuleRules
{
	public BrickRigsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Latest;
		OverridePackageType = PackageOverrideType.GameUncookedOnly;

		PublicIncludePaths.AddRange(new[]
		{
			"BrickRigsEditor"
		});

		PublicDependencyModuleNames.AddRange(new[]
		{
			"BrickRigs",
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(new[]
			{
				"AnimGraphRuntime",
				"AnimationCore",
				"BlueprintGraph",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"PropertyEditor",
				"Foliage",
				"Landscape"
			});
	}
}