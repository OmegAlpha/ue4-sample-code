// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealString.h"
#include "AssetToolsModule.h"

// Characters are organized in Classes (Brawler, Hero, etc)
const FString CHARACTER_IMPORT_PATH = FString("/Game/Art/Characters/Classes");

static class FImportUtils
{

public:

	static bool SaveDirtyAssets();

	static FAssetToolsModule& GetAssetToolsModule()
	{
		return FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	}

	static FString GetClassContentFolder(FString CharacterClass)
	{
		return FString::Format(TEXT("{0}/{1}"), { *CHARACTER_IMPORT_PATH, *CharacterClass });
	}

	static FString GetClassSkeletonPath(FString CharacterClass)
	{
		FString ClassContentFolder = GetClassContentFolder(CharacterClass);
		return FString::Format(TEXT("{0}/{1}_Skeleton"), { *ClassContentFolder, *CharacterClass });
	}
};
