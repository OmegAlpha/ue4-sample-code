// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dom/JsonObject.h"
#include "AvatarConfig.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FAvatarConfig
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	FString Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	class USkeletalMesh* SkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	class UTexture2D* ColorTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	class UTexture2D* CombinationTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	class UTexture2D* NormalTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visualization")
	class UTexture2D* SpecularTexture;

	UPROPERTY()
	bool bDidValidationSucceed;

	UPROPERTY()
	TArray<FString> ValidationFailureReasons;

public:

	void MakeFromJsonObject(TSharedPtr<FJsonObject> JSonObject);

	bool ValidateSourceFilesExist(TSharedPtr<FJsonObject> JSonObject);

private:

	void ValidateConfigField(FString ConfigKey, TSharedPtr<FJsonObject> JSonObject, TArray<FString>& SearchedDirectories, TArray<FString>& LoadedAssetFilepaths);

	void LoadAssetData(FString& Path, TArray<FString>& SearchedDirectories, TArray<FString>& LoadedAssetFilepaths);

	void LogValidationError(FString FailureReason);
};
