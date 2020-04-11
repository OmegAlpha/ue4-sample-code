// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericPlatformFile.h"
#include "AssetCodeImporter.h"

struct FAvatarsDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	FAvatarsDirectoryVisitor(IPlatformFile& InPlatformFile);
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;

private:
	IPlatformFile& PlatformFile;
};
