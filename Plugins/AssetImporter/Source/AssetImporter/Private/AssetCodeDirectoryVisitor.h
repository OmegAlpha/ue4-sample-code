// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericPlatformFile.h"
#include "AssetCodeImporter.h"
#include "RigImporter.h"
#include "ShotsDirectoryVisitor.h"

struct FAssetCodeDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	TSharedPtr<FRigImporter> RigImporter;
	TArray<TSharedPtr<FAssetCodeImporter>> AssetCodesToImport;
	FShotsDirectoryVisitor ShotsDirectoryVisitor;

private:
	const FString AssetClass;

public:
	FAssetCodeDirectoryVisitor(FString AssetClass);
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;

private:
	bool IsFileNameARig(FString FileName);
};
