// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetCodeImporter.h"
#include "JsonWriter.h"
#include "Paths.h"
#include "AssetToolsModule.h"
#include "Factories/FbxFactory.h"
#include "Factories/FbxImportUI.h"
#include "Factories/FbxSkeletalMeshImportData.h"
#include "Factories/FbxAnimSequenceImportData.h"
#include "JsonObject.h"
#include "JsonSerializer.h"
#include "PlatformFilemanager.h"
#include "GenericPlatformFile.h"
#include "AssetRegistryModule.h"
#include "FileHelpers.h"
#include "AssetImporter.h"
#include "AssetData.h"
#include "FileManager.h"
#include "ImportUtils.h"
#include "GenericPlatformProcess.h"
#include "ModuleManager.h"

typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FPrettyJsonStringWriterFactory;
typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FPrettyJsonStringWriter;

FAssetCodeImporter::FAssetCodeImporter(FString AssetClass, FString AssetCode, FString AssetCodeDirectory)
	: AssetClass(AssetClass)
	, AssetCode(AssetCode)
	, ImportDirectory(FPaths::Combine(AssetCodeDirectory, TEXT("asset")))
{
	UE_LOG(LogAssetImporter, Verbose, TEXT("Asset Code Struct Created: %s : %s"), *AssetClass, *AssetCode);
}

void FAssetCodeImporter::ImportAssets()
{
	bool bImportValidationPassed = PopulateSkeleton();

	FString SkeletalMeshToImport;
	bImportValidationPassed = PopulateSkeletalMeshToImport(SkeletalMeshToImport) && bImportValidationPassed;

	TArray<FString> TexturesToImport;
	bImportValidationPassed = PopulateTexturePathsToImport(TexturesToImport) && bImportValidationPassed;

	if (bImportValidationPassed)
	{
		FString DestinationPath = GetContentDirectoryPath();

		ImportSkeletalMesh(SkeletalMeshToImport, DestinationPath);		
		ImportTextures(TexturesToImport, DestinationPath);

		// TODO Search asset Directory for specific animation files; Note: These are optional
		// ImportAnimations(AnimationsToImport, DestinationPath);

		UE_LOG(LogAssetImporter, Log, TEXT("Assets Imported; Class: %s; Code: %s"), *AssetClass, *AssetCode);

		WriteConfigFile();
	}
	else
	{
		UE_LOG(LogAssetImporter, Error, TEXT("Validation Checks Failed; Class: %s; Code: %s"), *AssetClass, *AssetCode);
		UE_LOG(LogAssetImporter, Warning, TEXT("To Import a Avatar it Requires the Following Files in the Import asset Folder;"));
		UE_LOG(LogAssetImporter, Warning, TEXT("	Skeletal Mesh: %s;"), *GetSkeletalMeshFileName());
		UE_LOG(LogAssetImporter, Warning, TEXT("	Color Texture: %s;"), *GetColorTextureFileName());
		UE_LOG(LogAssetImporter, Warning, TEXT("	Combination Texture: %s;"), *GetCombinationTextureFileName());
		UE_LOG(LogAssetImporter, Warning, TEXT("	Normal Texture: %s;"), *GetNormalTextureFileName());
		UE_LOG(LogAssetImporter, Warning, TEXT("	Specular Texture: %s;"), *GetSpecularTextureFileName());
	}
}

bool FAssetCodeImporter::PopulateSkeleton()
{
	Skeleton = Cast<USkeleton>(StaticLoadObject(USkeleton::StaticClass(), NULL, *FImportUtils::GetClassSkeletonPath(AssetClass)));

	bool bSkeletonLoaded = Skeleton != nullptr;
	if (!bSkeletonLoaded)
	{
		UE_LOG(LogAssetImporter, Warning, TEXT("Unable to load the Unreal Skeleton; Class: %s; Code: %s"), *AssetClass, *AssetCode);
	}

	return bSkeletonLoaded;
}

bool FAssetCodeImporter::PopulateSkeletalMeshToImport(FString& OutSkeletalMeshPath) const
{
	OutSkeletalMeshPath = FPaths::Combine(ImportDirectory, GetImportSkeletalMeshFileName());
	return DoesImportFileExistOnDisk(OutSkeletalMeshPath);
}

bool FAssetCodeImporter::PopulateTexturePathsToImport(TArray<FString>& OutTexturePaths) const
{
	TArray<FString> TexturePaths;
	TexturePaths.Push(FPaths::Combine(ImportDirectory, GetImportColorTextureFileName()));
	TexturePaths.Push(FPaths::Combine(ImportDirectory, GetImportCombinationTextureFileName()));
	TexturePaths.Push(FPaths::Combine(ImportDirectory, GetImportNormalTextureFileName()));
	TexturePaths.Push(FPaths::Combine(ImportDirectory, GetImportSpecularTextureFileName()));
	OutTexturePaths = TexturePaths;

	bool bAllTexturesExistOnDisk = true;
	for (FString TexturePath : TexturePaths)
	{
		bAllTexturesExistOnDisk = DoesImportFileExistOnDisk(TexturePath) && bAllTexturesExistOnDisk;
	}

	return bAllTexturesExistOnDisk;
}

void FAssetCodeImporter::ImportSkeletalMesh(FString SkeletalMeshPath, FString DestinationPath) const
{
	TArray<FString> Filenames;
	Filenames.Add(SkeletalMeshPath);

	UAutomatedAssetImportData* ImportDataModels = NewObject<UAutomatedAssetImportData>();
	ImportDataModels->DestinationPath = DestinationPath;
	ImportDataModels->Filenames = Filenames;
	ImportDataModels->Factory = CreateModelImportFactory();
	ImportDataModels->bReplaceExisting = true;
	FImportUtils::GetAssetToolsModule().Get().ImportAssetsAutomated(ImportDataModels);	
}

void FAssetCodeImporter::ImportAnimations(TArray<FString> AnimationFilePaths, FString DestinationPath) const
{
	UAutomatedAssetImportData* ImportDataAnims = NewObject<UAutomatedAssetImportData>();
	ImportDataAnims->DestinationPath = DestinationPath;
	ImportDataAnims->Filenames = AnimationFilePaths;
	ImportDataAnims->Factory = CreateAnimationImportFactory();
	ImportDataAnims->bReplaceExisting = true;
	FImportUtils::GetAssetToolsModule().Get().ImportAssetsAutomated(ImportDataAnims);
}

void FAssetCodeImporter::ImportTextures(TArray<FString> TexuresFilePaths, FString DestinationPath) const
{
	UAutomatedAssetImportData* ImportDataTextures = NewObject<UAutomatedAssetImportData>();
	ImportDataTextures->DestinationPath = DestinationPath;
	ImportDataTextures->Filenames = TexuresFilePaths;
	ImportDataTextures->Factory = CreateTextureImportFactory();
	ImportDataTextures->bReplaceExisting = true;
	FImportUtils::GetAssetToolsModule().Get().ImportAssetsAutomated(ImportDataTextures);
}

UFbxFactory* FAssetCodeImporter::CreateAnimationImportFactory() const
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
	AnimationFactory->ImportUI->AnimSequenceImportData->SaveOptions();
	AnimationFactory->ImportUI->SaveConfig();

	return AnimationFactory;
}

UFbxFactory* FAssetCodeImporter::CreateModelImportFactory() const
{
	UFbxFactory* ModelFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass(), FName("FactoryModel"), RF_NoFlags);
	ModelFactory->ImportUI->Skeleton = Skeleton;
	ModelFactory->ImportUI->bImportMaterials = false;
	ModelFactory->ImportUI->bCreatePhysicsAsset = false;
	ModelFactory->ImportUI->bIsObjImport = false;
	ModelFactory->ImportUI->MeshTypeToImport = FBXIT_SkeletalMesh;
	ModelFactory->ImportUI->bImportMesh = true;
	ModelFactory->ImportUI->bImportAsSkeletal = true;
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

UTextureFactory* FAssetCodeImporter::CreateTextureImportFactory() const
{
	UTextureFactory* TextureFactory = NewObject<UTextureFactory>(UTextureFactory::StaticClass(), FName("FactoryTexture"), RF_NoFlags);
	return TextureFactory;
}

bool FAssetCodeImporter::WriteConfigFile() const
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	TSharedPtr<FJsonObject> ConfigField = MakeShareable(new FJsonObject);  //-- parent config field
	JsonObject->SetObjectField("Config", ConfigField);

	FString ContentDirectoryPath = GetContentDirectoryPath();
	ConfigField->SetStringField("AvatarName", AssetCode);
	ConfigField->SetStringField("AvatarClass", AssetClass);
	ConfigField->SetStringField("SKEL_Mesh", FPaths::Combine(ContentDirectoryPath, GetSkeletalMeshFileName()));
	ConfigField->SetStringField("TXT_Color", FPaths::Combine(ContentDirectoryPath, GetColorTextureFileName()));
	ConfigField->SetStringField("TXT_Combination", FPaths::Combine(ContentDirectoryPath, GetCombinationTextureFileName()));
	ConfigField->SetStringField("TXT_Normal", FPaths::Combine(ContentDirectoryPath, GetNormalTextureFileName()));
	ConfigField->SetStringField("TXT_Specular", FPaths::Combine(ContentDirectoryPath, GetSpecularTextureFileName()));

	FString OutputJsonString;

	TSharedRef<FPrettyJsonStringWriter> Writer = FPrettyJsonStringWriterFactory::Create(&OutputJsonString);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString JsonFilePath = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("Avatars"), AssetCode, TEXT("config.json"));

	IFileHandle* FileHandle = PlatformFile.OpenWrite(*JsonFilePath);
	if (FileHandle)
	{
		FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*OutputJsonString), OutputJsonString.Len());
		delete FileHandle;
	}
	return true;
}

bool FAssetCodeImporter::DoesImportFileExistOnDisk(FString FilePathToCheck) const
{
	bool bFileExistsOnDisk = FPaths::FileExists(FilePathToCheck);

	if (!bFileExistsOnDisk)
	{
		UE_LOG(LogAssetImporter, Error, TEXT("Import File Does Not Exist; Class: %s; Code: %s; File: %s"), *AssetClass, *AssetCode, *FPaths::GetCleanFilename(FilePathToCheck));
	}

	return bFileExistsOnDisk;
}

FString FAssetCodeImporter::GetContentDirectoryPathAbsolute() const
{
	return FString::Format( *(FPaths::ProjectContentDir() / "/Avatars/{0}"), { *AssetCode });
}

FString FAssetCodeImporter::GetContentDirectoryPath() const
{
	return FString::Format(TEXT("/Game/Avatars/{0}"), { *AssetCode });
}

FString FAssetCodeImporter::GetImportSkeletalMeshFileName() const
{
	return FString::Format(TEXT("{0}.fbx"), { GetSkeletalMeshFileName() });
}

FString FAssetCodeImporter::GetImportColorTextureFileName() const
{
	return FString::Format(TEXT("{0}.png"), { GetColorTextureFileName() });
}

FString FAssetCodeImporter::GetImportCombinationTextureFileName() const
{
	return FString::Format(TEXT("{0}.png"), { GetCombinationTextureFileName() });
}

FString FAssetCodeImporter::GetImportNormalTextureFileName() const
{
	return FString::Format(TEXT("{0}.png"), { GetNormalTextureFileName() });
}

FString FAssetCodeImporter::GetImportSpecularTextureFileName() const
{
	return FString::Format(TEXT("{0}.png"), { GetSpecularTextureFileName() });
}

FString FAssetCodeImporter::GetSkeletalMeshFileName() const
{
	return FString::Format(TEXT("{0}_{1}_base_mdl"), { AssetClass, AssetCode });
}

FString FAssetCodeImporter::GetColorTextureFileName() const
{
	return FString::Format(TEXT("{0}_{1}_color"), { AssetClass, AssetCode });
}

FString FAssetCodeImporter::GetCombinationTextureFileName() const
{
	return FString::Format(TEXT("{0}_{1}_combination"), { AssetClass, AssetCode });
}

FString FAssetCodeImporter::GetNormalTextureFileName() const
{
	return FString::Format(TEXT("{0}_{1}_normal"), { AssetClass, AssetCode });
}
  
FString FAssetCodeImporter::GetSpecularTextureFileName() const
{
	return FString::Format(TEXT("{0}_{1}_specular"), { AssetClass, AssetCode });
}
