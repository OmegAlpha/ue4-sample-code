// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationImporter.h"
#include "AssetToolsModule.h"
#include "ImportUtils.h"
#include "FileManager.h"
#include "Paths.h"
#include "AssetImporter.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxSkeletalMeshImportData.h"
#include "Factories/FbxAnimSequenceImportData.h"

FAnimationImporter::FAnimationImporter(FString AssetClass, FString ImportDirectory)
	: AssetClass(AssetClass)
	, ImportDirectory(ImportDirectory)
{

}

bool FAnimationImporter::PopulateSkeleton()
{
	Skeleton = Cast<USkeleton>(StaticLoadObject(USkeleton::StaticClass(), NULL, *FImportUtils::GetClassSkeletonPath(AssetClass)));

	bool bSkeletonLoaded = Skeleton != nullptr;
	if (!bSkeletonLoaded)
	{
		UE_LOG(LogAssetImporter, Warning, TEXT("Unable to load the Unreal Skeleton; Class: %s;"), *AssetClass);
	}

	return bSkeletonLoaded;
}

void FAnimationImporter::ImportAnimations()
{
	bool bIsSkeletonValid = PopulateSkeleton();

	TArray<FString> Files;
	IFileManager& FileManager = IFileManager::Get();

	FileManager.FindFiles(Files, *ImportDirectory);

	for (int i = 0; i < Files.Num(); i++) {
		Files[i] = FPaths::Combine(ImportDirectory, Files[i]);
	}

	FAssetToolsModule& AssetToolsModule = FImportUtils::GetAssetToolsModule();

	UAutomatedAssetImportData* ImportDataAnims = NewObject<UAutomatedAssetImportData>();
	ImportDataAnims->DestinationPath = FImportUtils::GetClassContentFolder(AssetClass);
	ImportDataAnims->Filenames = Files;
	ImportDataAnims->Factory = CreateAnimationImportFactory();
	ImportDataAnims->bReplaceExisting = true;
	AssetToolsModule.Get().ImportAssetsAutomated(ImportDataAnims);

	FImportUtils::SaveDirtyAssets();
}

UFbxFactory* FAnimationImporter::CreateAnimationImportFactory()
{
	UFbxFactory* AnimationFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass(), FName("FactoryModel"), RF_NoFlags);
	AnimationFactory->ImportUI->Skeleton = Skeleton;
	AnimationFactory->ImportUI->bImportMaterials = false;
	AnimationFactory->ImportUI->bCreatePhysicsAsset = false;
	AnimationFactory->ImportUI->bIsObjImport = false;
	AnimationFactory->ImportUI->bImportAnimations = true;
	AnimationFactory->ImportUI->MeshTypeToImport = FBXIT_Animation;
	AnimationFactory->ImportUI->bImportMesh = false;
	AnimationFactory->ImportUI->bImportAsSkeletal = false;
	AnimationFactory->ImportUI->AnimSequenceImportData->bImportMeshesInBoneHierarchy = true;
	AnimationFactory->ImportUI->bAutomatedImportShouldDetectType = false;
	AnimationFactory->ImportUI->AnimSequenceImportData->SaveOptions();
	AnimationFactory->ImportUI->SaveConfig();

	return AnimationFactory;
}