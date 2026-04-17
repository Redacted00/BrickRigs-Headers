// Copyright Fluppisoft, 2023

using UnrealBuildTool;

public class BrickRigsTarget : TargetRules
{
	protected bool bBuildModKit = true;

	public BrickRigsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange(new[] { "BrickRigs" });
		DefaultBuildSettings = BuildSettingsVersion.V2;


		ProjectDefinitions.Add("FLU_BUILD_MODKIT=" + (bBuildModKit ? "1" : "0"));
	}
}