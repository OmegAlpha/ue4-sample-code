// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetCodeDirectoryVisitor.h"
#include "Paths.h"
#include "AssetImporter.h"
#include "ShotsDirectoryVisitor.h"
#include "PlatformFilemanager.h"

FAssetCodeDirectoryVisitor::FAssetCodeDirectoryVisitor(FString AssetClass)
	: ShotsDirectoryVisitor(AssetClass),
	AssetClass(AssetClass)
	
{
	UE_LOG(LogAssetImporter, Verbose, TEXT("Asset Code Directory Visited: %s"), *AssetClass);
}

bool FAssetCodeDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	FString BaseFileName = FPaths::GetCleanFilename(FilenameOrDirectory);

	if (bIsDirectory)
	{
		if (BaseFileName.Equals("shots"))	// TODO "shots" should be a const string
		{
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			PlatformFile.IterateDirectory(FilenameOrDirectory, ShotsDirectoryVisitor);
		}
		else
		{
			AssetCodesToImport.Add(MakeShareable(new FAssetCodeImporter(AssetClass, BaseFileName, FilenameOrDirectory)));
		}
	}
	else 
	{
		if (IsFileNameARig(BaseFileName))
		{
			RigImporter = MakeShareable(new FRigImporter(AssetClass, FilenameOrDirectory));
		}		
	}

	return true;
}

bool FAssetCodeDirectoryVisitor::IsFileNameARig(FString FileName)
{
	return FileName.Equals(AssetClass + TEXT("_rig.fbx"));
}
