// Fill out your copyright notice in the Description page of Project Settings.


#include "ImporterObject.h"
#include "Paths.h"
#include "DesktopPlatformModule.h"
#include "PlatformFilemanager.h"
#include "AvatarsDirectoryVisitor.h"
#include "AvatarValidatorSettings.h"
#include "../Public/AssetImporter.h"
#include "ImportUtils.h"

void ImporterObject::OpenBulkImportDialog()
{
	UAvatarValidatorSettings* AvatarSettings = const_cast<UAvatarValidatorSettings*>(GetDefault<UAvatarValidatorSettings>());

	FString SelectionPath = FPaths::ProjectDir();
	if (FPaths::DirectoryExists(AvatarSettings->DefaultImportDirectory))
	{
		SelectionPath = AvatarSettings->DefaultImportDirectory;
	}

	FString OutDirectoryName;

	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform)
	{
		if (DesktopPlatform->OpenDirectoryDialog(nullptr, TEXT("Select Avatar Import Folder"), SelectionPath, OutDirectoryName))
		{						
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			FAvatarsDirectoryVisitor AvatarsDirectoryVisitor(PlatformFile);
			PlatformFile.IterateDirectory(*OutDirectoryName, AvatarsDirectoryVisitor);

			OutDirectoryName = FPaths::GetPath(OutDirectoryName);

			AvatarSettings->DefaultImportDirectory = OutDirectoryName;
			const_cast<UAvatarValidatorSettings*>(AvatarSettings)->SaveConfig();

			FImportUtils::SaveDirtyAssets();
		}
	}
}
