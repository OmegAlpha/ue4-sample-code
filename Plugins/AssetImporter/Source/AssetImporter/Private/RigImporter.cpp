// Fill out your copyright notice in the Description page of Project Settings.

#include "RigImporter.h"
#include "AssetToolsModule.h"
#include "ImportUtils.h"
#include "AssetImporter.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxSkeletalMeshImportData.h"

FRigImporter::FRigImporter(FString AssetClass, FString RigFilePath)
	: AssetClass(AssetClass)
	, RigFilePath(RigFilePath)
{
	UE_LOG(LogAssetImporter, Verbose, TEXT("Rig Importer Created: %s"), *AssetClass);
}

void FRigImporter::ImportRig() const
{
	TArray<FString> Filenames;
	Filenames.Add(RigFilePath);

	UAutomatedAssetImportData* ImportDataModels = NewObject<UAutomatedAssetImportData>();
	ImportDataModels->DestinationPath = FImportUtils::GetClassContentFolder(AssetClass);
	ImportDataModels->Filenames = Filenames;
	ImportDataModels->Factory = FRigImporter::CreateModelImportFactory();
	ImportDataModels->bReplaceExisting = true;
	TArray<UObject*> ImportedAssets = FImportUtils::GetAssetToolsModule().Get().ImportAssetsAutomated(ImportDataModels);

	FImportUtils::SaveDirtyAssets();
}

UFbxFactory* FRigImporter::CreateModelImportFactory()
{
	UFbxFactory* ModelFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass(), FName("FactoryModel"), RF_NoFlags);
	ModelFactory->ImportUI->bImportMaterials = false;
	ModelFactory->ImportUI->bCreatePhysicsAsset = false;
	ModelFactory->ImportUI->bIsObjImport = false;
	ModelFactory->ImportUI->MeshTypeToImport = FBXIT_SkeletalMesh;
	ModelFactory->ImportUI->bImportMesh = true;
	ModelFactory->ImportUI->bImportAsSkeletal = true;
	ModelFactory->ImportUI->bImportAnimations = false;
	ModelFactory->ImportUI->SkeletalMeshImportData->bImportMeshesInBoneHierarchy = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->bConvertScene = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->bImportMorphTargets = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->bTransformVertexToAbsolute = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->bBakePivotInVertex = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->bTransformVertexToAbsolute = true;
	ModelFactory->ImportUI->SkeletalMeshImportData->SaveConfig();
	ModelFactory->ImportUI->SaveConfig();

	return ModelFactory;
}
