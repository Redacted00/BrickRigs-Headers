// Copyright Fluppisoft, 2023

using UnrealBuildTool;
using System.Collections.Generic;

public class BrickRigsTarget : TargetRules
{
	public BrickRigsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		bUsesSteam = true;
		ExtraModuleNames.AddRange(new string[] { "BrickRigs" });
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// Setup the steam definitions
		GlobalDefinitions.Add("UE4_PROJECT_STEAMSHIPPINGID=552100");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMPRODUCTNAME=\"BrickRigs\"");
		// This has to be empty, used for mod directories
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDIR=\"BrickRigs\"");
		GlobalDefinitions.Add("UE4_PROJECT_STEAMGAMEDESC=\"Brick Rigs\"");
		// Allows the console to be used in shipping builds
		//GlobalDefinitions.Add("ALLOW_CONSOLE_IN_SHIPPING=1");

		ProjectDefinitions.Add("BR_STEAMAPPID=552100");
	}
}