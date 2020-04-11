// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AvatarValidator : ModuleRules
{
	public AvatarValidator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "UMG",
            "Json", "JsonUtilities",
            "AssetImporter",
            "MovieSceneCapture",
            "DesktopPlatform"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}