// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MoonlitRPG : ModuleRules
{
	public MoonlitRPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "SlateCore", "ActorSequence", "MovieScene", "Niagara", "LevelSequence" });
	}
}
