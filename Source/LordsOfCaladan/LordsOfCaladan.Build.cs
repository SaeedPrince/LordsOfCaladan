// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LordsOfCaladan : ModuleRules
{
	public LordsOfCaladan(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");	
	}
}
