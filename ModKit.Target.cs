// Copyright Fluppisoft, 2023

using UnrealBuildTool;

public class ModKitTarget : BrickRigsEditorTarget
{
	public ModKitTarget(TargetInfo Target) : base(Target)
	{
		ProjectDefinitions.Add("BR_BUILD_MODKIT=1");
	}
}