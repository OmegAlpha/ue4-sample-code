// Fill out your copyright notice in the Description page of Project Settings.

#include "AvatarConfig.h"
#include "Misc/Paths.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/Engine.h"

const FString Avatar_NAME_CONFIG_KEY = "AvatarName";
const FString Avatar_CLASS_CONFIG_KEY = "AvatarClass";
const FString SKELETAL_MESH_CONFIG_KEY = "SKEL_Mesh";
const FString COLOR_TEXTURE_CONFIG_KEY = "TXT_Color";
const FString COMBINATION_TEXTURE_CONFIG_KEY = "Txt_Combination";
const FString NORMAL_TEXTURE_CONFIG_KEY = "TXT_Normal";
const FString SPECULAR_TEXTURE_CONFIG_KEY = "TXT_Specular";

void FAvatarConfig::MakeFromJsonObject(TSharedPtr<FJsonObject> JSonObject)
{
	Name = *JSonObject->GetStringField(Avatar_NAME_CONFIG_KEY);
	Class = *JSonObject->GetStringField(Avatar_CLASS_CONFIG_KEY);

	SkeletalMesh = LoadObject<USkeletalMesh>(NULL, *JSonObject->GetStringField(SKELETAL_MESH_CONFIG_KEY));

	ColorTexture = LoadObject<UTexture2D>(NULL, *JSonObject->GetStringField(COLOR_TEXTURE_CONFIG_KEY));
	CombinationTexture = LoadObject<UTexture2D>(NULL, *JSonObject->GetStringField(COMBINATION_TEXTURE_CONFIG_KEY));
	NormalTexture = LoadObject<UTexture2D>(NULL, *JSonObject->GetStringField(NORMAL_TEXTURE_CONFIG_KEY));
	SpecularTexture = LoadObject<UTexture2D>(NULL, *JSonObject->GetStringField(SPECULAR_TEXTURE_CONFIG_KEY));
}

bool FAvatarConfig::ValidateSourceFilesExist(TSharedPtr<FJsonObject> JSonObject)
{
	bDidValidationSucceed = true;

	TArray<FString> SearchedDirectories;
	TArray<FString> LoadedAssetPaths;

	ValidateConfigField(SKELETAL_MESH_CONFIG_KEY, JSonObject, SearchedDirectories, LoadedAssetPaths);
	ValidateConfigField(COLOR_TEXTURE_CONFIG_KEY, JSonObject, SearchedDirectories, LoadedAssetPaths);
	ValidateConfigField(COMBINATION_TEXTURE_CONFIG_KEY, JSonObject, SearchedDirectories, LoadedAssetPaths);
	ValidateConfigField(NORMAL_TEXTURE_CONFIG_KEY, JSonObject, SearchedDirectories, LoadedAssetPaths);
	ValidateConfigField(SPECULAR_TEXTURE_CONFIG_KEY, JSonObject, SearchedDirectories, LoadedAssetPaths);

	return bDidValidationSucceed;
}

void FAvatarConfig::ValidateConfigField(FString ConfigKey, TSharedPtr<FJsonObject> JSonObject, TArray<FString>& SearchedDirectories, TArray<FString>& LoadedAssetFilepaths)
{
	FString AssetPath;

	FString CharacterName = "No Avatar Name";
	if (JSonObject->HasField(Avatar_NAME_CONFIG_KEY))
	{
		JSonObject->TryGetStringField(Avatar_NAME_CONFIG_KEY, CharacterName);
	}

	if (JSonObject->HasField(ConfigKey) && JSonObject->TryGetStringField(ConfigKey, AssetPath))
	{
		LoadAssetData(AssetPath, SearchedDirectories, LoadedAssetFilepaths);

		bool IsFound = false;
		for (FString& Filepath : LoadedAssetFilepaths)
		{
			if (Filepath.Equals(AssetPath))
			{
				IsFound = true;
				break;
			}
		}

		if (!IsFound)
		{
			LogValidationError(FString::Printf(TEXT("Missing UAsset; Avatar: %s; Config Key: %s; Path: %s;"), *CharacterName, *ConfigKey, *AssetPath));
		}
	}
	else
	{
		LogValidationError(FString::Printf(TEXT("Config key is missing or is wrong type; Avatar: %s; Config Key: %s"), *CharacterName, *ConfigKey));
	}
}

void FAvatarConfig::LoadAssetData(FString& Path, TArray<FString>& SearchedDirectories, TArray<FString>& LoadedAssetFilepaths)
{
	UObjectLibrary* AssetLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, false);

	FString Directory = FPaths::GetPath(Path);

	if (!SearchedDirectories.Contains(Directory))
	{
		TArray<FAssetData> AssetData;
		AssetLibrary->LoadAssetDataFromPath(Directory);
		AssetLibrary->GetAssetDataList(AssetData);

		for (int i = 0; i < AssetData.Num(); i++)
		{
			FString Filepath = FPaths::GetBaseFilename(AssetData[i].ObjectPath.ToString(), false);
			LoadedAssetFilepaths.Add(Filepath);
		}

		SearchedDirectories.Add(Directory);
	}
}

void FAvatarConfig::LogValidationError(FString FailureReason)
{
	bDidValidationSucceed = false;
	ValidationFailureReasons.Add(FailureReason);

	UE_LOG(LogTemp, Error, TEXT("%s"), *FailureReason);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 300.0f, FColor::Red, FailureReason);
	}
}