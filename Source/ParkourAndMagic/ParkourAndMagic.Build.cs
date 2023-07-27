// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ParkourAndMagic : ModuleRules
{
	public ParkourAndMagic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
