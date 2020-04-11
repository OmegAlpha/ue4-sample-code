// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AvatarValidatorSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig, meta=(DefaultName="Avatar Import Settings"))
class ASSETIMPORTER_API UAvatarValidatorSettings : public UDeveloperSettings
{
	GENERATED_UCLASS_BODY()
	
public:

	UPROPERTY(EditAnywhere, config, Category = Custom)
	FString DefaultImportDirectory;
};
