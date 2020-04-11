// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealString.h"
#include "Factories/FbxFactory.h"
#include "Animation/Skeleton.h"
#include "Factories/TextureFactory.h"

class FAssetCodeImporter
{
private:
	const FString AssetClass;
	const FString AssetCode;
	const FString ImportDirectory;

	USkeleton* Skeleton;

public:
	FAssetCodeImporter(FString AssetClass, FString AssetCode, FString AssetCodeDirectory);

	void ImportAssets();

private:

	bool PopulateSkeleton();
	bool PopulateTexturePathsToImport(TArray<FString>& OutTexturePaths) const;
	bool PopulateSkeletalMeshToImport(FString& OutSkeletalMeshPath) const;

	void ImportSkeletalMesh(FString SkeletalMeshPath, FString DestinationPath) const;
	void ImportAnimations(TArray<FString> AnimationFilePaths, FString DestinationPath) const;
	void ImportTextures(TArray<FString> TexuresFilePaths, FString DestinationPath) const;

	UFbxFactory* CreateAnimationImportFactory() const;
	UFbxFactory* CreateModelImportFactory() const;
	UTextureFactory* CreateTextureImportFactory() const;
	
	bool WriteConfigFile() const;

	bool DoesImportFileExistOnDisk(FString FilePathToCheck) const;

	FString GetContentDirectoryPathAbsolute() const;

	FString GetContentDirectoryPath() const;

	FString GetImportSkeletalMeshFileName() const;
	FString GetImportColorTextureFileName() const;
	FString GetImportCombinationTextureFileName() const;
	FString GetImportNormalTextureFileName() const;
	FString GetImportSpecularTextureFileName() const;

	FString GetSkeletalMeshFileName() const;
	FString GetColorTextureFileName() const;
	FString GetCombinationTextureFileName() const;
	FString GetNormalTextureFileName() const;
	FString GetSpecularTextureFileName() const;
};
