// Fill out your copyright notice in the Description page of Project Settings.

#include "AvatarsDirectoryVisitor.h"
#include "Paths.h"
#include "AssetCodeDirectoryVisitor.h"
#include "AssetImporter.h"

FAvatarsDirectoryVisitor::FAvatarsDirectoryVisitor(IPlatformFile& InPlatformFile) : PlatformFile(InPlatformFile)
{
}

bool FAvatarsDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		FString DirectoryName = FPaths::GetBaseFilename(FilenameOrDirectory, true);
		UE_LOG(LogAssetImporter, Verbose, TEXT("Process Asset Class Folder: %s"), *DirectoryName);

		FAssetCodeDirectoryVisitor AssetCodeDirectoryVisitor(DirectoryName);
		PlatformFile.IterateDirectory(FilenameOrDirectory, AssetCodeDirectoryVisitor);

		if (AssetCodeDirectoryVisitor.RigImporter.IsValid())
		{
			AssetCodeDirectoryVisitor.RigImporter->ImportRig();
		}

		for (TSharedPtr<FAssetCodeImporter> AssetCode : AssetCodeDirectoryVisitor.AssetCodesToImport)
		{
			if (AssetCode.IsValid())
			{
				AssetCode->ImportAssets();
			}
		}

		for (TSharedPtr<FAnimationImporter> AnimationImporter : AssetCodeDirectoryVisitor.ShotsDirectoryVisitor.AnimationsToImport)
		{
			if (AnimationImporter.IsValid())
			{
				AnimationImporter->ImportAnimations();
			}
		}
	}

	return true;
}
