// Copyright Fluppisoft, 2023

using UnrealBuildTool;

public class BrickRigsEditorTarget : BrickRigsTarget
{
	public BrickRigsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange(new[] { "BrickRigsEditor", "SimpleUGCEditor" });

		// These are not allowed to be set in editor targets, need to be cleared after the super constructor has set them
		GlobalDefinitions.Clear();

	}
}