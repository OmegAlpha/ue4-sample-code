// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealString.h"
#include "Factories/FbxFactory.h"

class FRigImporter
{
private:
	const FString AssetClass;
	const FString RigFilePath;

public:
	FRigImporter(FString AssetClass, FString RigFileName);

	void ImportRig() const;

private:
	static UFbxFactory* CreateModelImportFactory();
};
