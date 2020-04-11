// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UnrealString.h"
#include "Factories/FbxFactory.h"
#include "Animation/Skeleton.h"

class FAnimationImporter
{
private:
	const FString AssetClass;
	const FString ImportDirectory;

	USkeleton* Skeleton;

public:
	FAnimationImporter(FString AssetClass, FString ImportDirectory);

	void ImportAnimations();

private:
	bool PopulateSkeleton();

	UFbxFactory* CreateAnimationImportFactory();
};