// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarConfigsLoader.h"
#include "FileManager.h"
#include "Paths.h"
#include "FileHelper.h"
#include "JsonObject.h"
#include "JsonReader.h"
#include "JsonSerializer.h"
#include "Engine/Engine.h"

void UAvatarConfigsLoader::LoadAll()
{
	IFileManager& FileManager = IFileManager::Get();

	TArray<FString> AllConfigFilePaths;
	FString BaseSearchPath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Avatars"));
	FileManager.FindFilesRecursive(AllConfigFilePaths, *BaseSearchPath, TEXT("config.json"), true, false);

	if (GEngine)
	{
		GEngine->ClearOnScreenDebugMessages();
	}

	for (auto ConfigFilePath : AllConfigFilePaths)
	{
		FString JsonContent;
		FFileHelper::LoadFileToString(JsonContent, *ConfigFilePath);

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonContent);

		// TODO: add ANOTHER error raise here
		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("[AvatarConfigsLoader] Could NOT deserialize Json Config for: %s"), *ConfigFilePath);
			continue;
		}

		TSharedPtr<FJsonObject> JSON_ConfigObjectParent = JsonObject->GetObjectField("Config");

		FAvatarConfig AvatarConfig = FAvatarConfig();
		AvatarConfig.ValidateSourceFilesExist(JSON_ConfigObjectParent);
		// TODO: We will want to fall back on a "Default Avatar" asset package here
		AvatarConfig.MakeFromJsonObject(JSON_ConfigObjectParent);

		AllConfigs.Add(AvatarConfig);

		AllConfigsByAvatarId.Add(AvatarConfig.Name, AvatarConfig);
	}
}

TArray<FAvatarConfig> UAvatarConfigsLoader::GetConfigsForTheseIds(TArray<FString> AvatarIds) const
{
	TArray<FAvatarConfig> FilteredAvatars;

	for (FString AvatarID : AvatarIds)
	{
		if(AllConfigsByAvatarId.Contains(AvatarID))
		{
			FilteredAvatars.Add(AllConfigsByAvatarId[AvatarID]);
		}
	}

	return FilteredAvatars;
}
