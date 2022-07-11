// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class XD_PC : ModuleRules
{
	public XD_PC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "SlateCore", "InputCore"});

		PrivateDependencyModuleNames.AddRange(new string[] { "XDGSDK", "TapCommon", "TapDB", "TapMoment" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
