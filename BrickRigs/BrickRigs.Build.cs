// Copyright Fluppisoft, 2023

using UnrealBuildTool;

public class BrickRigs : ModuleRules
{
	public BrickRigs(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Latest;

		PublicIncludePaths.AddRange(new[]
		{
			"BrickRigs"
		});

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"UMG",
			"PhysX", "PhysicsCore", "APEX", "CableComponent",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Slate", "SlateCore", "RenderCore",
			"AnimationCore", "AnimGraphRuntime",
			"OnlineSubsystem", "OnlineSubsystemUtils",
			"Steamworks", "SteamSockets",
			"Sockets", "Networking",
			"HTTP",
			"MoviePlayer", "ImageWrapper",
			"GameplayTags",
			"EngineSettings",
			"Landscape",
			"ApplicationCore",
			"GameplayCameras",
			"Projects",
			"Water",
			"PakFile",
			"FluMoveSync",
			"PacketHandler",
			"NetCore",
			"MeshDescription",
			"StaticMeshDescription",
			"FluUGC"
		});


		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

		if (Target.bBuildEditor)
			PrivateDependencyModuleNames.AddRange(new[]
			{
				"UnrealEd"
			});
	}
}